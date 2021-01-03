//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldPressure.h"

registerMooseObject("raccoonApp", PhaseFieldPressure);

InputParameters
PhaseFieldPressure::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the phase-field approximated pressure.");
  params.addRequiredCoupledVar("d", "damage variable");
  params.addParam<MaterialPropertyName>("pressure_mat", "material property that holds pressure.");
  return params;
}

PhaseFieldPressure::PhaseFieldPressure(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _grad_d(coupledGradient("d")),
    _p_mat(getADMaterialProperty<Real>("pressure_mat"))
{
}

Real
PhaseFieldPressure::computeQpIntegral()
{
  return _p_mat[_qp].value() * _grad_d[_qp].norm();
}
