//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ThermalAblationVolume.h"

registerMooseObject("raccoonApp", ThermalAblationVolume);

template <>
InputParameters
validParams<ThermalAblationVolume>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addRequiredCoupledVar("d", "damage variable");
  params.addRequiredCoupledVar("temperature", "temperature variable");
  params.addParam<Real>("ablation_threshold", 0.0, "temperature threshold for ablation in K");
  return params;
}

ThermalAblationVolume::ThermalAblationVolume(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    MooseVariableInterface<Real>(
        this, false, "d", Moose::VarKindType::VAR_ANY, Moose::VarFieldType::VAR_FIELD_STANDARD),
    _d(coupledValue("d")),
    _T(coupledValue("temperature")),
    _threshold(getParam<Real>("ablation_threshold"))
{
  addMooseVariableDependency(mooseVariable());
}

Real
ThermalAblationVolume::computeQpIntegral()
{
  if (_T[_qp] > _threshold)
    return _d[_qp];

  return 0.0;
}
