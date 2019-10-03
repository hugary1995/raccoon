#include "Mesh.h"
#include "exodusII.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

// macros
#define MAX_STRING_LEN 128
#define OUTPUT_WIDTH 35

Mesh::Mesh(ProblemDefinition config) : _config(config)
{
  readExodus();
  readMeshParameters();
  readBoundaryNodeSet();
  readNodes();
  readBlocks();
  readElements();
  readTimes();
  buildElementToNeighborElementsMap();
}

Mesh::~Mesh()
{
  free(_blk_ids);
  free(_num_nodes_per_elem);
  free(_num_elems_in_blk);
  free(_time_values);
  for (int i = 0; i < _num_nodes; i++)
    delete _nodes[i];
  for (int i = 0; i < _num_elems; i++)
    delete _elems[i];
}

std::set<size_t>
Mesh::readDamageAtStep(int step)
{
  char * var_name = (char *)calloc((MAX_STR_LENGTH + 1), sizeof(char));
  float * d = (float *)calloc(_num_nodes, sizeof(float));
  for (int i = 0; i < _num_nodal_vars; i++)
  {
    _error = ex_get_variable_name(_exoid, EX_NODAL, i + 1, var_name);
    if (_config.exodus.damage_variable == var_name)
      _error = ex_get_var(_exoid, step, EX_NODAL, i + 1, 0, _num_nodes, d);
  }

  for (int i = 0; i < _num_nodes; i++)
    _nodes[i]->d() = d[i];

  std::set<size_t> Clusters_pending_update;
  for (int i = 0; i < _num_elems; i++)
  {
    bool old_state = _elems[i]->good();
    _elems[i]->binarize();
    bool new_state = _elems[i]->good();
    if (old_state != new_state)
      Clusters_pending_update.insert(_elems[i]->cluster());
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Updated damage for time step: " << step << std::endl;

  std::cout << "================================================================\n";

  free(var_name);
  free(d);
  return Clusters_pending_update;
}

bool
Mesh::isBoundaryElem(T3 * e)
{
  if (_boundary_nodes.find(e->n1()->id()) != _boundary_nodes.end())
    return true;
  if (_boundary_nodes.find(e->n2()->id()) != _boundary_nodes.end())
    return true;
  if (_boundary_nodes.find(e->n3()->id()) != _boundary_nodes.end())
    return true;
  return false;
}

bool
Mesh::isBoundaryCluster(Cluster * c)
{
  if (!c)
    return false;
  for (int i = 0; i < _num_elems; i++)
    if ((*c)[i])
      if (isBoundaryElem((*c)[i]))
        return true;
  return false;
}

void
Mesh::readExodus()
{
  // open exodus
  int CPU_word_size = sizeof(float);
  int IO_word_size = 0;

  std::string exodus_name = _config.exodus.file_name + ".e";

  _exoid = ex_open(exodus_name.c_str(), EX_READ, &CPU_word_size, &IO_word_size, &_version);

  if (_exoid < 0)
  {
    std::cout << "Cannot open " << _config.exodus.file_name << std::endl;
    exit(_exoid);
  }
}

void
Mesh::readMeshParameters()
{
  char title[MAX_STR_LENGTH + 1];

  _error = ex_get_init(_exoid,
                       title,
                       &_num_dim,
                       &_num_nodes,
                       &_num_elems,
                       &_num_blks,
                       &_num_node_sets,
                       &_num_side_sets);

  if (_error)
  {
    std::cout << "After ex_get_init, error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  _error = ex_get_variable_param(_exoid, EX_NODAL, &_num_nodal_vars);
  if (_error)
  {
    std::cout << "After ex_get_variable_param, error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Exodus file ID: " << _exoid << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Title: " << title << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Dimension: " << _num_dim << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of Nodes: " << _num_nodes << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << _num_elems << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of blocks: " << _num_blks << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of Node sets: " << _num_node_sets << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of side sets: " << _num_side_sets << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of nodal variables: " << _num_nodal_vars
            << std::endl;

  std::cout << "================================================================\n";
}

void
Mesh::readBoundaryNodeSet()
{
  int * side_set_ids = (int *)calloc(_num_side_sets, sizeof(int));
  _error = ex_get_ids(_exoid, EX_SIDE_SET, side_set_ids);
  for (int i = 0; i < _num_side_sets; i++)
  {
    char set_name[MAX_STR_LENGTH + 1];
    _error = ex_get_name(_exoid, EX_SIDE_SET, side_set_ids[i], set_name);
    std::cout << std::setw(OUTPUT_WIDTH) << "Found set: " << set_name << std::endl;
    bool is_boundary = false;
    for (auto b : _config.exodus.boundaries)
      if (b == set_name)
      {
        is_boundary = true;
        std::cout << std::setw(OUTPUT_WIDTH) << "set: " << set_name
                  << " is on external boundary.\n";
        break;
      }
    if (!is_boundary)
      continue;
    int num_elems_in_set;
    _error = ex_get_set_param(_exoid, EX_SIDE_SET, side_set_ids[i], &num_elems_in_set, NULL);
    int * Node_ctr_list = (int *)calloc(num_elems_in_set, sizeof(int));
    int * Node_list = (int *)calloc(num_elems_in_set * 3, sizeof(int));
    _error = ex_get_side_set_node_list(_exoid, side_set_ids[i], Node_ctr_list, Node_list);
    for (int j = 0; j < num_elems_in_set * 3; j++)
      _boundary_nodes.insert(Node_list[j]);
    free(Node_ctr_list);
    free(Node_list);
  }

  free(side_set_ids);
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of boundary Nodes: " << _boundary_nodes.size()
            << std::endl;
  std::cout << "================================================================\n";
}

void
Mesh::readNodes()
{
  float * x = (float *)calloc(_num_nodes, sizeof(float));
  float * y = _num_dim >= 2 ? (float *)calloc(_num_nodes, sizeof(float)) : NULL;
  float * z = _num_dim >= 3 ? (float *)calloc(_num_nodes, sizeof(float)) : NULL;

  _error = ex_get_coord(_exoid, x, y, z);
  if (_error)
  {
    std::cout << "After ex_get_coord, error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  for (int i = 0; i < _num_nodes; i++)
  {
    Node * tmp = new Node(i, x[i], y[i], 0);
    _nodes.push_back(tmp);
  }

  free(x);
  free(y);
  free(z);

  std::cout << std::setw(OUTPUT_WIDTH) << "Allocated: " << _nodes.size() << " Nodes.\n";
  std::cout << "================================================================\n";
}

void
Mesh::readBlocks()
{
  _blk_ids = (int *)calloc(_num_blks, sizeof(int));
  _error = ex_get_ids(_exoid, EX_ELEM_BLOCK, _blk_ids);
  if (_error)
  {
    std::cout << "After ex_get_ids, error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Block IDs: ";
  for (int i = 0; i < _num_blks; i++)
    std::cout << _blk_ids[i] << " ";
  std::cout << std::endl;

  std::cout << "================================================================\n";

  _num_elems_in_blk = (int *)calloc(_num_blks, sizeof(int));
  _num_nodes_per_elem = (int *)calloc(_num_blks, sizeof(int));
  char type[MAX_STRING_LEN + 1];

  for (int i = 0; i < _num_blks; i++)
  {
    _error = ex_get_block(_exoid,
                          EX_ELEM_BLOCK,
                          _blk_ids[i],
                          type,
                          &_num_elems_in_blk[i],
                          &_num_nodes_per_elem[i],
                          NULL,
                          NULL,
                          NULL);

    if (_error)
    {
      std::cout << "After ex_get_elem_block, error = " << _error << std::endl;
      ex_close(_exoid);
      exit(_error);
    }

    std::cout << std::setw(OUTPUT_WIDTH) << "In block: " << _blk_ids[0] << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Element type: " << type << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << _num_elems_in_blk[i]
              << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH)
              << "Number of Nodes per element: " << _num_nodes_per_elem[i] << std::endl;

    std::cout << "================================================================\n";
  }
}

void
Mesh::readElements()
{
  for (int i = 0; i < _num_blks; i++)
  {
    int * connectivity =
        (int *)calloc((_num_nodes_per_elem[i] * _num_elems_in_blk[i]), sizeof(int));

    _error = ex_get_conn(_exoid, EX_ELEM_BLOCK, _blk_ids[i], connectivity, NULL, NULL);
    if (_error)
    {
      std::cout << "After ex_get_conn, error = " << _error << std::endl;
      ex_close(_exoid);
      exit(_error);
    }

    for (int j = 0; j < _num_elems_in_blk[i]; j++)
    {
      T3 * tmp = new T3(_elems.size(),
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 0] - 1],
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 1] - 1],
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 2] - 1],
                        _config.algorithm.cut_off);
      _elems.push_back(tmp);
    }

    free(connectivity);
    std::cout << std::setw(OUTPUT_WIDTH) << "Allocated: " << _elems.size() << " elements.\n";
    std::cout << "================================================================\n";
  }
}

void
Mesh::readTimes()
{
  // read time steps
  _nun_time_steps = ex_inquire_int(_exoid, EX_INQ_TIME);
  _time_values = (float *)calloc(_nun_time_steps, sizeof(float));
  _error = ex_get_all_times(_exoid, _time_values);
  if (_error)
  {
    std::cout << "After ex_get_all_times, error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of time steps: " << _nun_time_steps << std::endl;

  std::cout << "================================================================\n";
}

void
Mesh::buildElementToNeighborElementsMap()
{
  // initialize elem_to_elems map
  double progress = 0.0;
  double threshold = 0.0;
  for (int i = 0; i < _num_elems; i++)
  {
    for (int j = i + 1; j < _num_elems; j++)
      if (_elems[i]->isConnectedTo(_elems[j]))
      {
        _elems[i]->neighbors().push_back(_elems[j]);
        _elems[j]->neighbors().push_back(_elems[i]);
      }
    progress = (double)i / (double)_num_elems * 100.0;
    if (progress >= threshold)
    {
      std::cout << "[" << std::setw(3) << (int)progress
                << "%] Building element to connected elements map...\n";
      threshold += 10.0;
    }
  }
  std::cout << "[100%] Building element to connected elements map...\n";
}
