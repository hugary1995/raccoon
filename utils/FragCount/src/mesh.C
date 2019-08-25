#include "mesh.h"
#include "exodusII.h"

// macros
#define MAX_STRING_LEN 128
#define OUTPUT_WIDTH 35

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

mesh::mesh(const char * filename)
{
  plt::backend("TkAgg");
  // open exodus
  int CPU_word_size = sizeof(float);
  int IO_word_size = 0;
  int exoid, error;
  int num_dim, num_nodes, num_elems, num_blks, num_node_sets, num_side_sets;
  float version;
  char title[MAX_STRING_LEN + 1], type[MAX_STRING_LEN + 1];

  exoid = ex_open(filename, EX_READ, &CPU_word_size, &IO_word_size, &version);

  if (exoid < 0)
  {
    std::cout << "Cannot open " << filename << std::endl;
    exit(exoid);
  }

  error = ex_get_init(
      exoid, title, &num_dim, &num_nodes, &num_elems, &num_blks, &num_node_sets, &num_side_sets);

  if (error)
  {
    std::cout << "After ex_get_init, error = " << error << std::endl;
    ex_close(exoid);
    exit(error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Exodus file ID: " << exoid << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Title: " << title << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Dimension: " << num_dim << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of nodes: " << num_nodes << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << num_elems << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of blocks: " << num_blks << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of node sets: " << num_node_sets << std::endl;
  std::cout << std::setw(OUTPUT_WIDTH) << "Number of side sets: " << num_side_sets << std::endl;

  std::cout << "================================================================\n";

  float * x = (float *)calloc(num_nodes, sizeof(float));
  float * y = num_dim >= 2 ? (float *)calloc(num_nodes, sizeof(float)) : NULL;
  float * z = num_dim >= 3 ? (float *)calloc(num_nodes, sizeof(float)) : NULL;

  error = ex_get_coord(exoid, x, y, z);
  if (error)
  {
    std::cout << "After ex_get_coord, error = " << error << std::endl;
    ex_close(exoid);
    exit(error);
  }
  std::cout << std::setw(OUTPUT_WIDTH) << "First node coord: ";
  std::cout << "(" << x[0] << ", " << y[0] << ")\n";

  for (int i = 0; i < num_nodes; i++)
  {
    node * tmp = new node(i, x[i], y[i], 0);
    _nodes.push_back(tmp);
  }

  free(x);
  free(y);
  free(z);

  std::cout << "allocated " << _nodes.size() << " nodes.\n";
  std::cout << "================================================================\n";

  int * block_ids = (int *)calloc(num_blks, sizeof(int));
  error = ex_get_ids(exoid, EX_ELEM_BLOCK, block_ids);
  if (error)
  {
    std::cout << "After ex_get_ids, error = " << error << std::endl;
    ex_close(exoid);
    exit(error);
  }

  std::cout << std::setw(OUTPUT_WIDTH) << "Block IDs: ";
  for (int i = 0; i < num_blks; i++)
    std::cout << block_ids[i] << " ";
  std::cout << std::endl;

  std::cout << "================================================================\n";

  int * num_elems_in_blk = (int *)calloc(num_blks, sizeof(int));
  int * num_nodes_per_elem = (int *)calloc(num_blks, sizeof(int));
  int * num_attrs = (int *)calloc(num_blks, sizeof(int));

  for (int i = 0; i < num_blks; i++)
  {
    error = ex_get_block(exoid,
                         EX_ELEM_BLOCK,
                         block_ids[i],
                         type,
                         &num_elems_in_blk[i],
                         &num_nodes_per_elem[i],
                         NULL,
                         NULL,
                         &num_attrs[i]);

    if (error)
    {
      std::cout << "After ex_get_elem_block, error = " << error << std::endl;
      ex_close(exoid);
      exit(error);
    }

    std::cout << "In block " << block_ids[0] << ": \n";
    std::cout << std::setw(OUTPUT_WIDTH) << "Element type: " << type << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of elements: " << num_elems_in_blk[i]
              << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of nodes per element: " << num_nodes_per_elem[i]
              << std::endl;
    std::cout << std::setw(OUTPUT_WIDTH) << "Number of attributes per element: " << num_attrs[i]
              << std::endl;

    std::cout << "================================================================\n";
  }

  for (int i = 0; i < num_blks; i++)
  {
    int * connectivity = (int *)calloc((num_nodes_per_elem[i] * num_elems_in_blk[i]), sizeof(int));

    error = ex_get_conn(exoid, EX_ELEM_BLOCK, block_ids[i], connectivity, NULL, NULL);
    if (error)
    {
      std::cout << "After ex_get_conn, error = " << error << std::endl;
      ex_close(exoid);
      exit(error);
    }

    std::cout << std::setw(OUTPUT_WIDTH) << "First element in block " << block_ids[i] << ": ";
    for (int j = 0; j < num_nodes_per_elem[i]; j++)
      std::cout << connectivity[j] << " ";
    std::cout << std::endl;

    for (int j = 0; j < num_elems_in_blk[i]; j++)
    {
      std::cout << "Element " << j << " contains nodes "
                << connectivity[j * num_nodes_per_elem[i] + 0] - 1 << ", "
                << connectivity[j * num_nodes_per_elem[i] + 1] - 1 << ", "
                << connectivity[j * num_nodes_per_elem[i] + 2] - 1 << std::endl;
      T3 * tmp = new T3(0,
                        _nodes[connectivity[j * num_nodes_per_elem[i] + 0] - 1],
                        _nodes[connectivity[j * num_nodes_per_elem[i] + 1] - 1],
                        _nodes[connectivity[j * num_nodes_per_elem[i] + 2] - 1],
                        0.75);
      _elems.push_back(tmp);
    }

    free(connectivity);

    std::cout << "================================================================\n";
  }
}

void
mesh::plot_mesh()
{
  plt::clf();
  std::map<std::string, std::string> keywords;
  keywords["linewidth"] = "1";
  for (unsigned int i = 0; i < _elems.size(); i++)
  {
    std::vector<double> x, y;
    x.push_back(_elems[i]->n1()->x());
    x.push_back(_elems[i]->n2()->x());
    x.push_back(_elems[i]->n3()->x());
    y.push_back(_elems[i]->n1()->y());
    y.push_back(_elems[i]->n2()->y());
    y.push_back(_elems[i]->n3()->y());
    plt::plot(x, y, keywords);
    std::cout << "[" << std::setw(3) << (int)((double)i / (double)_elems.size() * 100) << "%] "
              << "Element #" << std::setw(6) << _elems[i]->id() << " plotted.\n";
  }
  plt::show();
}
