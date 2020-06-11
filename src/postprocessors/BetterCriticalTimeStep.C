//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BetterCriticalTimeStep.h"

registerMooseObject("raccoonApp", BetterCriticalTimeStep);

InputParameters
BetterCriticalTimeStep::validParams()
{
  InputParameters params = ElementPostprocessor::validParams();
  params.addClassDescription(
      "Computes and reports the critical time step for the explicit solver.");
  params.addParam<MaterialPropertyName>(
      "density_name", "rho", "Name of Material Property defining the density of the material.");
  params.addParam<MaterialPropertyName>(
      "E_name", "effective_stiffness", "Name of material property defining material stiffness");
  params.addParam<Real>("factor", 1.0, "Factor to mulitply to the critical time step.");
  return params;
}

BetterCriticalTimeStep::BetterCriticalTimeStep(const InputParameters & parameters)
  : ElementPostprocessor(parameters),
    _rho_name(getParam<MaterialPropertyName>("density_name")),
    _material_density(getMaterialPropertyByName<Real>(_rho_name)),
    _E_name(getParam<MaterialPropertyName>("E_name")),
    _effective_stiffness(getMaterialPropertyByName<Real>(_E_name)),
    _factor(getParam<Real>("factor")),
    _critical_time(std::numeric_limits<Real>::max())
{
}

void
BetterCriticalTimeStep::execute()
{
  // Real dens = _material_density[0];
  _c = std::numeric_limits<Real>::min();
  for (unsigned qp = 0; qp < _q_point.size(); ++qp)
  {
    _c = std::max(_c, _effective_stiffness[qp] / std::sqrt(_material_density[qp]));
  }
  // std::cout << _c << std::endl;
  _critical_time = std::min(_factor * _current_elem->hmin() / _c, _critical_time);
}

void
BetterCriticalTimeStep::finalize()
{
  gatherMin(_critical_time);
}

PostprocessorValue
BetterCriticalTimeStep::getValue()
{
  return _critical_time;
}

void
BetterCriticalTimeStep::threadJoin(const UserObject & y)
{
  const BetterCriticalTimeStep & pps = static_cast<const BetterCriticalTimeStep &>(y);
  _critical_time = std::min(_critical_time, pps._critical_time);
}
