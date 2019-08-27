#pragma once

#include "T3.h"
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>

class mesh
{
public:
  mesh(const char * filename);
  ~mesh()
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

  std::vector<node *> nodes() { return _nodes; }
  std::vector<T3 *> elems() { return _elems; }
  int num_time_steps() { return _num_time_steps; }
  std::set<size_t> read_damage_at_step(int step);

protected:
private:
  void read_exodus();
  void read_mesh_parameters();
  void read_nodes();
  void read_blocks();
  void read_elements();
  void read_times();

  const char * _filename;
  int _exoid;
  int _error;
  int _num_dim;
  int _num_nodes;
  int _num_elems;
  int _num_blks;
  int * _blk_ids;
  int * _num_nodes_per_elem;
  int * _num_elems_in_blk;
  float _version;
  int _num_nodal_vars;
  int _num_time_steps;
  float * _time_values;

  std::vector<node *> _nodes;
  std::vector<T3 *> _elems;
};
