#pragma once

#include "mesh.h"

class problem
{
public:
  problem(const char * filename);
  void plot_damage();

protected:
private:
  mesh * _mesh;
};
