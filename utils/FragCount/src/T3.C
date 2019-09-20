#include "T3.h"

T3::T3(unsigned int id, Node * n1, Node * n2, Node * n3, double cutoff)
  : _id(id), _n1(n1), _n2(n2), _n3(n3), _cutoff(cutoff), _cluster(0)
{
  binarize();

  _area = std::abs(0.5 * (_n1->x() * (_n2->y() - _n3->y()) + _n2->x() * (_n3->y() - _n1->y()) +
                          _n3->x() * (_n1->y() - _n2->y())));

  _xc = (_n1->x() + _n2->x() + _n3->x()) / 3;
  _yc = (_n1->y() + _n2->y() + _n3->y()) / 3;
}

void
T3::binarize()
{
  _intact = true;
  if (_n1->d() > _cutoff || _n2->d() > _cutoff || _n3->d() > _cutoff)
    _intact = false;
}

double
T3::distance(T3 * e)
{
  return std::sqrt((_xc - e->xc()) * (_xc - e->xc()) + (_yc - e->yc()) * (_yc - e->yc()));
}

bool
T3::isConnectedTo(T3 * e)
{
  if (!e)
    return false;

  if (containsNode(e->n1()))
    return true;
  else if (containsNode(e->n2()))
    return true;
  else if (containsNode(e->n3()))
    return true;

  return false;
}

bool
T3::commonEdge(T3 * e, std::vector<double> & x, std::vector<double> & y)
{
  Node * n1 = NULL;
  Node * n2 = NULL;
  if (containsNode(e->n1()) && containsNode(e->n2()))
  {
    n1 = e->n1();
    n2 = e->n2();
  }
  else if (containsNode(e->n1()) && containsNode(e->n3()))
  {
    n1 = e->n1();
    n2 = e->n3();
  }
  else if (containsNode(e->n2()) && containsNode(e->n3()))
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

bool
T3::containsNode(Node * n)
{
  if (_n1 == n || _n2 == n || _n3 == n)
    return true;
  return false;
}
