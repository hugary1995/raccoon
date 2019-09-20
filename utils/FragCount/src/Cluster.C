#include "Cluster.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <limits>
#include <iostream>

Cluster::Cluster(size_t num_elems) : _num_elems(num_elems), _num_active_elems(0)
{
  _elems.resize(num_elems);
}

Cluster::Cluster(std::vector<T3 *> elems) : _elems(elems), _num_elems(elems.size())
{
  _num_active_elems = 0;
  for (size_t i = 0; i < _num_elems; i++)
    if (_elems[i])
      _num_active_elems++;
}

size_t
Cluster::first()
{
  size_t i;
  for (i = 0; i < _num_elems; i++)
    if (_elems[i])
      return i;
  return i;
}

size_t
Cluster::next(size_t i)
{
  for (size_t j = i + 1; j < _num_elems; j++)
    if (_elems[j])
      return j;
  for (size_t j = 0; j < i; j++)
    if (_elems[j])
      return j;
  return _num_elems;
}

bool
Cluster::contain(T3 * e)
{
  for (size_t i = 0; i < _num_elems; i++)
    if (_elems[i] == e)
      return true;
  return false;
}

void
Cluster::moveElemTo(size_t i, Cluster * to)
{
  (*to)[i] = _elems[i];
  _elems[i] = NULL;

  _num_active_elems--;
  to->_num_active_elems++;
}

void
Cluster::copyElemTo(size_t i, Cluster * to)
{
  (*to)[i] = _elems[i];
  to->_num_active_elems++;
}

void
Cluster::removeElem(size_t i)
{
  _elems[i] = NULL;
  _num_active_elems--;
}

void
Cluster::computeArea()
{
  if (_num_active_elems == 0)
    std::cout << "Requesting area of a void Cluster!\n";

  _area = 0.0;
  for (size_t i = 0; i < _num_elems; i++)
    if (_elems[i])
      _area += _elems[i]->area();
}

void
Cluster::computeCentroid()
{
  if (_num_active_elems == 0)
  {
    std::cout << "Requesting centroid of a void Cluster!\n";
    return;
  }

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

double
Cluster::xc()
{
  if (_num_active_elems == 0)
    std::cout << "Requesting centroid of a void Cluster!\n";
  return _xc;
}

double
Cluster::yc()
{
  if (_num_active_elems == 0)
    std::cout << "Requesting centroid of a void Cluster!\n";
  return _yc;
}

double
Cluster::distanceToElem(T3 * e)
{
  double dist = std::numeric_limits<double>::max();
  double new_dist;
  for (size_t i = 0; i < _num_elems; i++)
    if (_elems[i])
    {
      new_dist = _elems[i]->distance(e);
      dist = std::min(dist, new_dist);
    }
  return dist;
}

void
Cluster::clear()
{
  _elems.clear();
  _elems.resize(_num_elems);
  _num_active_elems = 0;
  _xc = 0.0;
  _yc = 0.0;
  _xp = 0.0;
  _yp = 0.0;
  _area = 0.0;
}

void
Cluster::PCA()
{
  if (_num_active_elems == 0)
    return;

  Eigen::EigenSolver<Eigen::MatrixXd> es;
  Eigen::MatrixXd X(_num_active_elems, 2);
  int count = 0;
  for (size_t i = 0; i < _num_elems; i++)
    if (_elems[i])
    {
      double x = (_elems[i]->xc() - _xc) * _elems[i]->area();
      double y = (_elems[i]->yc() - _yc) * _elems[i]->area();
      X(count, 0) = x;
      X(count, 1) = y;
      count++;
    }
  es.compute(X.transpose() * X, true);
  Eigen::MatrixXd D = es.eigenvalues().real();
  // find the largest eigenvalue index
  int index = D(0, 0) > D(1, 0) ? 0 : 1;
  Eigen::MatrixXd V = es.eigenvectors().real();
  Eigen::MatrixXd v1 = V.col(index);
  _xp = v1(0, 0);
  _yp = v1(1, 0);
}

double
Cluster::xp()
{
  if (_num_active_elems == 0)
    std::cout << "Requesting principal component of a void Cluster!\n";
  return _xp;
}

double
Cluster::yp()
{
  if (_num_active_elems == 0)
    std::cout << "Requesting principal component of a void Cluster!\n";
  return _yp;
}
