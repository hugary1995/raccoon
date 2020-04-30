//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADStressDivergenceExpTensors.h"

registerADMooseObject("raccoonApp", ADStressDivergenceExpTensors);

InputParameters
ADStressDivergenceExpTensors::validParams()
{
  InputParameters params = ADStressDivergenceTensors::validParams();
  return params;
}

ADStressDivergenceExpTensors::ADStressDivergenceExpTensors(const InputParameters & parameters)
  : ADStressDivergenceTensors(parameters),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress"))
{
}

ADReal
ADStressDivergenceExpTensors::computeQpResidual()
{
  return _stress_old[_qp].row(_component) * _grad_test[_i][_qp];
}
