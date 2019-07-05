//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DamageICBase.h"

template <>
InputParameters
validParams<DamageICBase>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<Real>("l", "Length scale");
  params.addRequiredParam<std::vector<Real>>("x1", "The abscissa values of crack starting points");
  params.addRequiredParam<std::vector<Real>>("y1", "The ordinate values of crack starting points");
  params.addRequiredParam<std::vector<Real>>("z1", "The applicate values of crack starting points");
  params.addRequiredParam<std::vector<Real>>("x2", "The abscissa values of crack ending points");
  params.addRequiredParam<std::vector<Real>>("y2", "The ordinate values of crack ending points");
  params.addRequiredParam<std::vector<Real>>("z2", "The applicate values of crack ending points");
  params.addRequiredParam<Real>("d0", "Initial damage");
  return params;
}

DamageICBase::DamageICBase(const InputParameters & parameters)
  : InitialCondition(parameters),
    _x1(getParam<std::vector<Real>>("x1")),
    _y1(getParam<std::vector<Real>>("y1")),
    _z1(getParam<std::vector<Real>>("z1")),
    _x2(getParam<std::vector<Real>>("x2")),
    _y2(getParam<std::vector<Real>>("y2")),
    _z2(getParam<std::vector<Real>>("z2")),
    _d0(getParam<Real>("d0")),
    _l(getParam<Real>("l"))
{
  mooseAssert(_x1.size() == _y1.size(), "number of cracks mismatch");
  mooseAssert(_x1.size() == _z1.size(), "number of cracks mismatch");
  mooseAssert(_x1.size() == _x2.size(), "number of cracks mismatch");
  mooseAssert(_x1.size() == _y2.size(), "number of cracks mismatch");
  mooseAssert(_x1.size() == _z2.size(), "number of cracks mismatch");
}

Real
DamageICBase::dist(const Point & p)
{
  // number of cracks
  unsigned int n = _x1.size();

  Real dist = -1.0;
  Real dist_new = 0.0;

  for (unsigned i = 0; i < n; ++i)
  {

    // point x1 and point x2 define a line segment
    // point x0 is the current point
    Point x1(_x1[i], _y1[i], _z1[i]);
    Point x2(_x2[i], _y2[i], _z2[i]);
    Point x0 = p;

    Point dx0x1 = x1 - x0;
    Point dx0x2 = x2 - x0;
    Point dx1x2 = x2 - x1;

    // in range indicator, if ind >= 0, in range
    Real ind1 = -dx0x1 * dx1x2 / dx0x1.norm() / dx1x2.norm();
    Real ind2 = dx0x2 * dx1x2 / dx0x2.norm() / dx1x2.norm();
    if (ind1 * ind2 >= 0)
    {
      Point num = dx0x1.cross(dx0x2);
      dist_new = num.norm() / dx1x2.norm();
    }
    else
    {
      Real dist1 = dx0x1.norm();
      Real dist2 = dx0x2.norm();
      if (dist1 < dist2)
        dist_new = dist1;
      else
        dist_new = dist2;
    }
    if (dist == -1.0 || dist_new < dist)
      dist = dist_new;
  }

  return dist;
}
