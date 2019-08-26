#include "mesh.h"
#include "exodusII.h"

// macros
#define MAX_STRING_LEN 128
#define OUTPUT_WIDTH 35

mesh::mesh(const char * filename)
{
  // open exodus
  int CPU_word_size = sizeof(float);
  int IO_word_size = 0;
  int num_node_sets, num_side_sets;
  char title[MAX_STRING_LEN + 1], type[MAX_STRING_LEN + 1];

  _exoid = ex_open(filename, EX_READ, &CPU_word_size, &IO_word_size, &_version);

  if (_exoid < 0)
  {
    std::cout << "Cannot open " << filename << std::endl;
    exit(_exoid);
  }

  _error = ex_get_init(_exoid,
                       title,
                       &_num_dim,
                       &_num_nodes,
                       &_num_elems,
                       &_num_blks,
                       &num_node_sets,
                       &num_side_sets);

  if (_error)
  {
    std::cout << "After ex_get_init, _error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Exodus file ID: " << _exoid << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Title: " << title << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Dimension: " << _num_dim << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of nodes: " << _num_nodes << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << _num_elems << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of blocks: " << _num_blks << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of node sets: " << num_node_sets << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of side sets: " << num_side_sets << std::endl;

  std::cout << "================================================================\n";

  _error = ex_get_variable_param(_exoid, EX_NODAL, &_num_nodal_vars);
  if (_error)
  {
    std::cout << "After ex_get_variable_param, _error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of nodal variables: " << _num_nodal_vars
            << std::endl;

  char * var_names[_num_nodal_vars];
  for (int i = 0; i < _num_nodal_vars; i++)
    var_names[i] = (char *)calloc((MAX_STR_LENGTH + 1), sizeof(char));
  _error = ex_get_variable_names(_exoid, EX_NODAL, _num_nodal_vars, var_names);
  std::cout << std::setw(OUTPUT_WIDTH) << "Variable names: ";
  for (int i = 0; i < _num_nodal_vars; i++)
    std::cout << var_names[i] << " ";
  std::cout << std::endl;

  // read time steps
  _num_time_steps = ex_inquire_int(_exoid, EX_INQ_TIME);
  _time_values = (float *)calloc(_num_time_steps, sizeof(float));
  _error = ex_get_all_times(_exoid, _time_values);
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of time steps: " << _num_time_steps << std::endl;

  // read damage
  char * var_name = (char *)calloc((MAX_STR_LENGTH + 1), sizeof(char));
  float * d = (float *)calloc(_num_nodes, sizeof(float));
  for (int i = 0; i < _num_nodal_vars; i++)
  {
    _error = ex_get_variable_name(_exoid, EX_NODAL, i + 1, var_name);
    if (strcmp(var_name, "d") == 0)
      _error = ex_get_var(_exoid, _num_time_steps, EX_NODAL, i + 1, 0, _num_nodes, d);
  }

  std::cout << "================================================================\n";

  float * x = (float *)calloc(_num_nodes, sizeof(float));
  float * y = _num_dim >= 2 ? (float *)calloc(_num_nodes, sizeof(float)) : NULL;
  float * z = _num_dim >= 3 ? (float *)calloc(_num_nodes, sizeof(float)) : NULL;

  _error = ex_get_coord(_exoid, x, y, z);
  if (_error)
  {
    std::cout << "After ex_get_coord, _error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }
  std::cout << std::setw(OUTPUT_WIDTH) << "First node coord: ";
  std::cout << "(" << x[0] << ", " << y[0] << ")\n";

  for (int i = 0; i < _num_nodes; i++)
  {
    node * tmp = new node(i + 1, x[i], y[i], d[i]);
    _nodes.push_back(tmp);
  }

  free(x);
  free(y);
  free(z);
  free(d);

  std::cout << std::setw(OUTPUT_WIDTH) << "Allocated: " << _nodes.size() << " nodes.\n";
  std::cout << "================================================================\n";

  int * block_ids = (int *)calloc(_num_blks, sizeof(int));
  _error = ex_get_ids(_exoid, EX_ELEM_BLOCK, block_ids);
  if (_error)
  {
    std::cout << "After ex_get_ids, _error = " << _error << std::endl;
    ex_close(_exoid);
    exit(_error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Block IDs: ";
  for (int i = 0; i < _num_blks; i++)
    std::cout << block_ids[i] << " ";
  std::cout << std::endl;

  std::cout << "================================================================\n";

  int * _num_elems_in_blk = (int *)calloc(_num_blks, sizeof(int));
  int * _num_nodes_per_elem = (int *)calloc(_num_blks, sizeof(int));
  int * num_attrs = (int *)calloc(_num_blks, sizeof(int));

  for (int i = 0; i < _num_blks; i++)
  {
    _error = ex_get_block(_exoid,
                          EX_ELEM_BLOCK,
                          block_ids[i],
                          type,
                          &_num_elems_in_blk[i],
                          &_num_nodes_per_elem[i],
                          NULL,
                          NULL,
                          &num_attrs[i]);

    if (_error)
    {
      std::cout << "After ex_get_elem_block, _error = " << _error << std::endl;
      ex_close(_exoid);
      exit(_error);
    }

    std::cout << "In block " << block_ids[0] << ": \n";
    std::cout << std::setw(OUTPUT_WIDTH) << "Element type: " << type << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << _num_elems_in_blk[i]
              << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH)
              << "Number of nodes per element: " << _num_nodes_per_elem[i] << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of attributes per element: " << num_attrs[i]
              << std::endl;

    std::cout << "================================================================\n";
  }

  for (int i = 0; i < _num_blks; i++)
  {
    int * connectivity =
        (int *)calloc((_num_nodes_per_elem[i] * _num_elems_in_blk[i]), sizeof(int));

    _error = ex_get_conn(_exoid, EX_ELEM_BLOCK, block_ids[i], connectivity, NULL, NULL);
    if (_error)
    {
      std::cout << "After ex_get_conn, _error = " << _error << std::endl;
      ex_close(_exoid);
      exit(_error);
    }

    std::cout << std::setw(OUTPUT_WIDTH) << "First element in block: " << block_ids[i] << ": ";
    for (int j = 0; j < _num_nodes_per_elem[i]; j++)
      std::cout << connectivity[j] << " ";
    std::cout << std::endl;

    for (int j = 0; j < _num_elems_in_blk[i]; j++)
    {
      T3 * tmp = new T3(_elems.size() + 1,
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 0] - 1],
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 1] - 1],
                        _nodes[connectivity[j * _num_nodes_per_elem[i] + 2] - 1],
                        0.75);
      _elems.push_back(tmp);
    }

    free(connectivity);
    std::cout << std::setw(OUTPUT_WIDTH) << "Allocated: " << _elems.size() << " elements.\n";
    std::cout << "================================================================\n";
  }
}

// void
// mesh::plot_mesh()
// {
//   plt::clf();
//   std::vector<double> x, y;
//   for (unsigned int i = 0; i < _elems.size(); i++)
//   {
//     x.push_back(_elems[i]->xc());
//     y.push_back(_elems[i]->yc());
//     // std::cout << "[" << std::setw(3) << (int)((double)i / (double)_elems.size() * 100) << "%] "
//     //           << "Element #" << std::setw(6) << _elems[i]->id() << " plotted.\n";
//   }
//   plt::plot(x, y, "k.");
//   plt::show();
// }
