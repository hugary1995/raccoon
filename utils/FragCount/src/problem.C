#include "problem.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

problem::problem(const char * filename) { _mesh = new mesh(filename); }

void
problem::plot_damage()
{
  plt::backend("TkAgg");
  plt::clf();
  std::vector<double> x, y, d;
  std::vector<node *> nodes = _mesh->nodes();
  for (unsigned int i = 0; i < nodes.size(); i++)
  {
    x.push_back(nodes[i]->x());
    y.push_back(nodes[i]->y());
    d.push_back(nodes[i]->d());
    // std::cout << "[" << std::setw(3) << (int)((double)i / (double)_elems.size() * 100) << "%] "
    //           << "Element #" << std::setw(6) << _elems[i]->id() << " plotted.\n";
  }
  plt::scatter(x, y, 1, d);
  plt::show();
}
