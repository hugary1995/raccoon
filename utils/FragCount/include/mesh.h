#pragma once

#include "T3.h"
#include <string>
#include <vector>
#include <algorithm>

class mesh
{
public:
  mesh(const char * filename);
  void plot_mesh();

protected:
private:
  std::vector<node *> _nodes;
  std::vector<T3 *> _elems;
};
