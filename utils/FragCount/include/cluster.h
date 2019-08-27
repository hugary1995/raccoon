#pragma once

#include "T3.h"

class cluster
{
public:
  cluster(size_t num_elems) : _num_elems(num_elems), _num_active_elems(0)
  {
    _elems.resize(num_elems);
  }
  cluster(std::vector<T3 *> elems) : _elems(elems), _num_elems(elems.size())
  {
    _num_active_elems = 0;
    for (size_t i = 0; i < _num_elems; i++)
      if (_elems[i])
        _num_active_elems++;
  }
  ~cluster() {}
  T3 *& operator[](size_t i) { return _elems[i]; }
  bool empty() { return _num_active_elems == 0; }
  size_t size() { return _num_active_elems; }
  size_t first()
  {
    size_t i;
    for (i = 0; i < _num_elems; i++)
      if (_elems[i])
        return i;
    return i;
  }
  void move_elem_to(size_t i, cluster * to)
  {
    (*to)[i] = _elems[i];
    _elems[i] = NULL;

    _num_active_elems--;
    to->_num_active_elems++;
  }
  double area()
  {
    double a = 0.0;
    for (size_t i = 0; i < _num_elems; i++)
      if (_elems[i])
        a += _elems[i]->area();
    return a;
  }
  void compute_centroid()
  {
    _xc = 0.0;
    _yc = 0.0;
    for (size_t i = 0; i < _num_elems; i++)
      if (_elems[i])
      {
        _xc += _elems[i]->xc() * _elems[i]->area();
        _yc += _elems[i]->yc() * _elems[i]->area();
      }

    _xc /= area();
    _yc /= area();
  }
  double xc() { return _xc; }
  double yc() { return _yc; }
  double distance_to_elem(T3 * e)
  {
    return std::sqrt((_xc - e->xc()) * (_xc - e->xc()) + (_yc - e->yc()) * (_yc - e->yc()));
  }
  void clear()
  {
    _elems.clear();
    _elems.resize(_num_elems);
    _num_active_elems = 0;
    _xc = 0.0;
    _yc = 0.0;
  }

protected:
private:
  std::vector<T3 *> _elems;
  size_t _num_elems;
  size_t _num_active_elems;
  double _xc;
  double _yc;
};
