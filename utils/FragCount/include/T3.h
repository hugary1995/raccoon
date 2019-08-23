#include "node.h"
#include <math.h>

class T3
{
public:
  T3(unsigned int id, node * n1, node * n2, node * n3, double cutoff)
    : _id(id), _n1(n1), _n2(n2), _n3(n3), _cutoff(cutoff), _cluster(-1)
  {
    _good = true;
    if (_n1->d() > _cutoff || _n2->d() > _cutoff || _n3->d() > _cutoff)
      _good = false;

    _area = std::abs(0.5 * (_n1->x() * (_n2->y() - _n3->y()) + _n2->x() * (_n3->y() - _n1->y()) +
                            _n3->x() * (_n1->y() - _n2->y())));
  }

  unsigned int id() { return _id; }
  node * n1() { return _n1; }
  node * n2() { return _n2; }
  node * n3() { return _n3; }
  double area() { return _area; }
  bool good() { return _good; }
  int cluster() { return _cluster; }
  void set_cluster(int cluster) { _cluster = cluster; }
  double distance(T3 * e)
  {
    double x1 = (_n1->x() + _n2->x() + _n3->x()) / 3.0;
    double y1 = (_n1->y() + _n2->y() + _n3->y()) / 3.0;
    double x2 = (e->n1()->x() + e->n2()->x() + e->n3()->x()) / 3.0;
    double y2 = (e->n1()->y() + e->n2()->y() + e->n3()->y()) / 3.0;
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  }
  bool is_connected_to(T3 * e)
  {
    if (_n1 == e->n1())
      return true;
    if (_n1 == e->n2())
      return true;
    if (_n1 == e->n3())
      return true;
    if (_n2 == e->n1())
      return true;
    if (_n2 == e->n2())
      return true;
    if (_n2 == e->n3())
      return true;
    if (_n3 == e->n1())
      return true;
    if (_n3 == e->n2())
      return true;
    if (_n3 == e->n3())
      return true;

    return false;
  }
  bool common_edge(T3 * e, std::vector<double> & x, std::vector<double> & y)
  {
    node * n1 = NULL;
    node * n2 = NULL;
    if (contains_node(e->n1()) && contains_node(e->n2()))
    {
      n1 = e->n1();
      n2 = e->n2();
    }
    else if (contains_node(e->n1()) && contains_node(e->n3()))
    {
      n1 = e->n1();
      n2 = e->n3();
    }
    else if (contains_node(e->n2()) && contains_node(e->n3()))
    {
      n1 = e->n2();
      n2 = e->n3();
    }
    if (n1 && n2)
    {
      x.push_back(n1->x());
      x.push_back(n2->x());
      y.push_back(n1->y());
      y.push_back(n2->y());
      return true;
    }
    return false;
  }
  bool contains_node(node * n)
  {
    if (_n1 == n || _n2 == n || _n3 == n)
      return true;
    return false;
  }

protected:
private:
  unsigned int _id;
  node * _n1;
  node * _n2;
  node * _n3;
  double _cutoff;
  bool _good;
  double _area;
  int _cluster;
};
