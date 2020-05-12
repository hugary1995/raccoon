//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADExplicitStressDivergenceRZTensors.h"

registerADMooseObject("raccoonApp", ADExplicitStressDivergenceRZTensors);

InputParameters
ADExplicitStressDivergenceRZTensors::validParams()
{
  InputParameters params = ADStressDivergenceRZTensors::validParams();
  return params;
}

ADExplicitStressDivergenceRZTensors::ADExplicitStressDivergenceRZTensors(
    const InputParameters & parameters)
  : ADStressDivergenceRZTensors(parameters),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress"))
{
}

ADReal
ADExplicitStressDivergenceRZTensors::computeQpResidual()
{
  ADReal div = 0.0;
  if (_component == 0)
  {
    div = _grad_test[_i][_qp](0) * _stress_old[_qp](0, 0) +
          (_test[_i][_qp] / _ad_q_point[_qp](0)) * _stress_old[_qp](2, 2) +
          _grad_test[_i][_qp](1) * _stress_old[_qp](0, 1); // stress_{rz}

    // volumetric locking correction
    if (_volumetric_locking_correction)
      div += (_avg_grad_test[_i] - _grad_test[_i][_qp](0) - _test[_i][_qp] / _ad_q_point[_qp](0)) *
             (_stress_old[_qp].trace()) / 3.0;
  }
  else if (_component == 1)
  {
    div = _grad_test[_i][_qp](1) * _stress_old[_qp](1, 1) +
          _grad_test[_i][_qp](0) * _stress_old[_qp](1, 0); // stress_{zr}

    // volumetric locking correction
    if (_volumetric_locking_correction)
      div += (_avg_grad_test[_i] - _grad_test[_i][_qp](1)) * (_stress_old[_qp].trace()) / 3.0;
  }
  else
    mooseError("Invalid component for this AxisymmetricRZ problem.");

  return div;
}
