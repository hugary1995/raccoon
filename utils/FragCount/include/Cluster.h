#pragma once

#include "T3.h"

class Cluster
{
public:
  Cluster(size_t num_elems);
  Cluster(std::vector<T3 *> elems);
  ~Cluster() {}
  T3 *& operator[](size_t i) { return _elems[i]; }
  bool empty() { return _num_active_elems == 0; }
  size_t size() { return _num_active_elems; }
  size_t first();
  size_t next(size_t i);
  bool contain(T3 * e);
  void moveElemTo(size_t i, Cluster * to);
  void copyElemTo(size_t i, Cluster * to);
  void removeElem(size_t i);
  double area() { return _area; }
  void computeArea();
  void computeCentroid();
  double xc();
  double yc();
  double distanceToElem(T3 * e);
  void clear();
  void PCA();
  double xp();
  double yp();

protected:
private:
  std::vector<T3 *> _elems;
  size_t _num_elems;
  size_t _num_active_elems;
  double _xc;
  double _yc;
  double _xp;
  double _yp;
  double _area;
};
