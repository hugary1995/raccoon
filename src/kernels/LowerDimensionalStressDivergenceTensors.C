//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LowerDimensionalStressDivergenceTensors.h"

registerMooseObject("raccoonApp", LowerDimensionalStressDivergenceTensors);

InputParameters
LowerDimensionalStressDivergenceTensors::validParams()
{
  InputParameters params = StressDivergenceTensors::validParams();
  params.addClassDescription("Stress divergence kernel for lower dimensional elements");
  params.addParam<MaterialPropertyName>(
      "thickness", "thickness", "Thickness of the lower dimensional block");
  return params;
}

LowerDimensionalStressDivergenceTensors::LowerDimensionalStressDivergenceTensors(
    const InputParameters & parameters)
  : StressDivergenceTensors(parameters),
    _thickness(getMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("thickness"))),
    _Q(getMaterialProperty<RankTwoTensor>(_base_name +
                                          "lower_dimensional_coordinate_transformation"))
{
}

Real
LowerDimensionalStressDivergenceTensors::computeQpResidual()
{
  RankTwoTensor stress = _Q[_qp] * _stress[_qp] * _Q[_qp].transpose();
  stress(0, 2) = stress(1, 2) = stress(2, 0) = stress(2, 1) = stress(2, 2) = 0;
  stress = _Q[_qp].transpose() * stress * _Q[_qp];
  Real residual = stress.row(_component) * _grad_test[_i][_qp];

  // volumetric locking correction
  if (_volumetric_locking_correction)
    residual +=
        (_avg_grad_test[_i][_component] - _grad_test[_i][_qp](_component)) / 3.0 * stress.trace();

  return _thickness[_qp] * residual;
}

Real
LowerDimensionalStressDivergenceTensors::computeQpJacobian()
{
  return _thickness[_qp] * StressDivergenceTensors::computeQpJacobian();
}

Real
LowerDimensionalStressDivergenceTensors::computeQpOffDiagJacobian(unsigned int jvar)
{
  return _thickness[_qp] * StressDivergenceTensors::computeQpOffDiagJacobian(jvar);
}
