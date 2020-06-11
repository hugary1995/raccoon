//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

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
    _material_density(getMaterialProperty<Real>("density_name")),
    _effective_stiffness(getMaterialProperty<Real>("E_name")),
    _factor(getParam<Real>("factor")),
    _critical_time(std::numeric_limits<Real>::max())
{
}

void
BetterCriticalTimeStep::execute()
{
  _c = std::numeric_limits<Real>::min();
  for (unsigned qp = 0; qp < _q_point.size(); ++qp)
  {
    _c = std::max(_c, _effective_stiffness[qp] / std::sqrt(_material_density[qp]));
  }
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
