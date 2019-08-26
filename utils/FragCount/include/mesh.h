#pragma once

#include "T3.h"
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

class mesh
{
public:
  mesh(const char * filename);
  std::vector<node *> nodes() { return _nodes; }

protected:
private:
  int _exoid;
  int _error;
  int _num_dim;
  int _num_nodes;
  int _num_elems;
  int _num_blks;
  float _version;
  int _num_nodal_vars;
  int _num_time_steps;
  float * _time_values;

  std::vector<node *> _nodes;
  std::vector<T3 *> _elems;
};
