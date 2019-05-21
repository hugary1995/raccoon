//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DynamicPiolaKirchhoffStressDivergence.h"

registerMooseObject("raccoonApp", DynamicPiolaKirchhoffStressDivergence);

template <>
InputParameters
validParams<DynamicPiolaKirchhoffStressDivergence>()
{
  InputParameters params = validParams<PiolaKirchhoffStressDivergence>();
  params.addClassDescription(
      "Piola Kirchhoff stress divergence kernel for the Cartesian coordinate system");
  params.addParam<MaterialPropertyName>("zeta",
                                        0.0,
                                        "Name of material property or a constant real "
                                        "number defining the zeta parameter for the "
                                        "Rayleigh damping.");
  params.addParam<Real>("alpha", 0, "alpha parameter for HHT time integration");
  return params;
}

DynamicPiolaKirchhoffStressDivergence::DynamicPiolaKirchhoffStressDivergence(
    const InputParameters & parameters)
  : PiolaKirchhoffStressDivergence(parameters),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress")),
    _stress_older(getMaterialPropertyOlderByName<RankTwoTensor>(_base_name + "stress")),
    _F_old(_large_deformation
               ? &getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")
               : NULL),
    _F_older(_large_deformation ? &getMaterialPropertyOlderByName<RankTwoTensor>(
                                      _base_name + "deformation_gradient")
                                : NULL),
    _zeta(getMaterialProperty<Real>("zeta")),
    _alpha(getParam<Real>("alpha"))
{
}

Real
DynamicPiolaKirchhoffStressDivergence::computeQpResidual()
{
  RealVectorValue stress, stress_old, stress_older;
  if (_large_deformation)
  {
    stress = _stress[_qp] * (*_F)[_qp].row(_component);
    stress_old = _stress_old[_qp] * (*_F_old)[_qp].row(_component);
    stress_older = _stress_older[_qp] * (*_F_older)[_qp].row(_component);
  }
  else
  {
    stress = _stress[_qp].row(_component);
    stress_old = _stress_old[_qp].row(_component);
    stress_older = _stress_older[_qp].row(_component);
  }

  Real residual = _grad_test_undisplaced[_i][_qp] * stress;
  Real residual_old = _grad_test_undisplaced[_i][_qp] * stress_old;
  Real residual_older = _grad_test_undisplaced[_i][_qp] * stress_older;

  Real coef = 1.0 + _alpha + (1.0 + _alpha) * _zeta[_qp] / _dt;
  Real coef_old = _alpha + (1.0 + 2.0 * _alpha) * _zeta[_qp] / _dt;
  Real coef_older = _alpha * _zeta[_qp] / _dt;

  return coef * residual - coef_old * residual_old + coef_older * residual_older;
}

Real
DynamicPiolaKirchhoffStressDivergence::computeQpJacobian()
{
  Real coef = 1.0 + _alpha + (1.0 + _alpha) * _zeta[_qp] / _dt;

  return coef * PiolaKirchhoffStressDivergence::computeQpJacobian();
}

Real
DynamicPiolaKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real coef = 1.0 + _alpha + (1.0 + _alpha) * _zeta[_qp] / _dt;

  return coef * PiolaKirchhoffStressDivergence::computeQpOffDiagJacobian(jvar);
}
