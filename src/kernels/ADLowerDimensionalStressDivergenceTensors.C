//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADLowerDimensionalStressDivergenceTensors.h"

registerMooseObject("raccoonApp", ADLowerDimensionalStressDivergenceTensors);

InputParameters
ADLowerDimensionalStressDivergenceTensors::validParams()
{
  InputParameters params = ADStressDivergenceTensors::validParams();
  params.addClassDescription("Stress divergence kernel for lower dimensional elements");
  params.addParam<MaterialPropertyName>(
      "thickness", "thickness", "Thickness of the lower dimensional block");
  return params;
}

ADLowerDimensionalStressDivergenceTensors::ADLowerDimensionalStressDivergenceTensors(
    const InputParameters & parameters)
  : ADStressDivergenceTensors(parameters),
    _thickness(
        getADMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("thickness"))),
    _Q(getMaterialProperty<RankTwoTensor>(_base_name +
                                          "lower_dimensional_coordinate_transformation"))
{
}

ADReal
ADLowerDimensionalStressDivergenceTensors::computeQpResidual()
{
  ADRankTwoTensor stress = _Q[_qp] * _stress[_qp] * _Q[_qp].transpose();
  stress(0, 2) = stress(1, 2) = stress(2, 0) = stress(2, 1) = stress(2, 2) = 0;
  stress = _Q[_qp].transpose() * stress * _Q[_qp];
  ADReal residual = stress.row(_component) * _grad_test[_i][_qp];

  // volumetric locking correction
  if (_volumetric_locking_correction)
    residual += (_avg_grad_test[_i] - _grad_test[_i][_qp](_component)) / 3.0 * stress.trace();

  return _thickness[_qp] * residual;
}
