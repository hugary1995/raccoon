//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "BetterCriticalTimeStep.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", BetterCriticalTimeStep);
registerMooseObject("raccoonApp", ADBetterCriticalTimeStep);

template <bool is_ad>
InputParameters
BetterCriticalTimeStepTempl<is_ad>::validParams()
{
  InputParameters params = ElementPostprocessor::validParams();
  params.addClassDescription(
      "Computes and reports the critical time step for the explicit solver.");
  params.addParam<MaterialPropertyName>(
      "density_name", "density", "Name of Material Property defining the density of the material.");
  params.addParam<MaterialPropertyName>("effective_stiffness_name",
                                        "effective_stiffness",
                                        "Name of material property defining material stiffness");
  params.addParam<Real>("factor", 1.0, "Factor to mulitply to the critical time step.");
  return params;
}

template <bool is_ad>
BetterCriticalTimeStepTempl<is_ad>::BetterCriticalTimeStepTempl(const InputParameters & parameters)
  : ElementPostprocessor(parameters),
    _material_density(getGenericMaterialProperty<Real, is_ad>("density_name")),
    _effective_stiffness(getGenericMaterialProperty<Real, is_ad>("effective_stiffness_name")),
    _factor(getParam<Real>("factor")),
    _critical_time(std::numeric_limits<Real>::max())
{
}

template <bool is_ad>
void
BetterCriticalTimeStepTempl<is_ad>::execute()
{
  _c = std::numeric_limits<Real>::min();
  for (unsigned qp = 0; qp < _q_point.size(); ++qp)
  {
    _c = std::max(
        _c, MetaPhysicL::raw_value(_effective_stiffness[qp] / std::sqrt(_material_density[qp])));
  }
  _critical_time = std::min(_factor * _current_elem->hmin() / _c, _critical_time);
}

template <bool is_ad>
void
BetterCriticalTimeStepTempl<is_ad>::finalize()
{
  gatherMin(_critical_time);
}

template <bool is_ad>
PostprocessorValue
BetterCriticalTimeStepTempl<is_ad>::getValue()
{
  return _critical_time;
}

template <bool is_ad>
void
BetterCriticalTimeStepTempl<is_ad>::threadJoin(const UserObject & y)
{
  const BetterCriticalTimeStepTempl<is_ad> & pps =
      static_cast<const BetterCriticalTimeStepTempl<is_ad> &>(y);
  _critical_time = std::min(_critical_time, pps._critical_time);
}

template class BetterCriticalTimeStepTempl<false>;
template class BetterCriticalTimeStepTempl<true>;
