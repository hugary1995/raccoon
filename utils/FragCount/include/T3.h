#pragma once

#include "Node.h"
#include <math.h>
#include <vector>

class T3
{
public:
  T3(unsigned int id, Node * n1, Node * n2, Node * n3, double cutoff);
  ~T3() {}

  void binarize();

  unsigned int id() { return _id; }
  Node * n1() { return _n1; }
  Node * n2() { return _n2; }
  Node * n3() { return _n3; }
  double area() { return _area; }
  bool good() { return _intact; }
  double xc() { return _xc; }
  double yc() { return _yc; }
  size_t cluster() { return _cluster; }
  std::vector<T3 *> & neighbors() { return _neighbors; }
  void setCluster(size_t Cluster) { _cluster = Cluster; }
  double distance(T3 * e);
  bool isConnectedTo(T3 * e);
  bool commonEdge(T3 * e, std::vector<double> & x, std::vector<double> & y);
  bool containsNode(Node * n);

protected:
private:
  unsigned int _id;
  Node * _n1;
  Node * _n2;
  Node * _n3;
  double _cutoff;
  bool _intact;
  double _area;
  double _xc;
  double _yc;
  size_t _cluster;
  std::vector<T3 *> _neighbors;
};
