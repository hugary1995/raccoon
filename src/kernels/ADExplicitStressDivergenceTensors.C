//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADExplicitStressDivergenceTensors.h"

registerADMooseObject("raccoonApp", ADExplicitStressDivergenceTensors);

InputParameters
ADExplicitStressDivergenceTensors::validParams()
{
  InputParameters params = ADStressDivergenceTensors::validParams();
  return params;
}

ADExplicitStressDivergenceTensors::ADExplicitStressDivergenceTensors(
    const InputParameters & parameters)
  : ADStressDivergenceTensors(parameters),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress"))
{
}

ADReal
ADExplicitStressDivergenceTensors::computeQpResidual()
{
  return _stress_old[_qp].row(_component) * _grad_test[_i][_qp];
}
