//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Nitsche.h"
#include "Assembly.h"

registerMooseObject("raccoonApp", Nitsche);

InputParameters
Nitsche::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredCoupledVar("levelset", "The levelset variable");
  params.addRequiredCoupledVar("cti", "The crack tip indicator");
  params.addRequiredParam<Real>("alpha", "penalty");
  params.addRequiredParam<Real>("cti_threshold", "percentage of cti");
  params.addRequiredParam<PostprocessorName>("cti_max", "maximum cti value");
  params.addParam<RealVectorValue>("normal", "Cheating");
  return params;
}

Nitsche::Nitsche(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _u_nodal(_var.adDofValues()),
    _grad_phi(adCoupledGradient("levelset")),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _wn(getADMaterialPropertyByName<Real>(prependBaseName("crack_opening_displacement"))),
    _alpha(getParam<Real>("alpha")),
    _cti(adCoupledValue("cti")),
    _cti_threshold(getParam<Real>("cti_threshold")),
    _cti_max(getPostprocessorValue("cti_max")),
    _normal_provided(isParamValid("normal")),
    _normal(_normal_provided ? getParam<RealVectorValue>("normal") : RealVectorValue(0, 0, 0))
{
}

ADReal
Nitsche::computeQpResidual()
{
  if (_cti[_qp] < _cti_threshold * _cti_max)
    return _test[_i][_qp] * _u[_qp];

  ADReal value = _test[_i][_qp] * (_u[_qp] - _wn[_qp]);

  if (_d[_qp] > 1e-6)
  {
    ADRealVectorValue n;
    if (_normal_provided)
      n = _normal;
    else
      n = _grad_phi[_qp] / _grad_phi[_qp].norm();
    value += _alpha * (_grad_test[_i][_qp] * n) * (_grad_u[_qp] * n);
  }

  return value;
}
