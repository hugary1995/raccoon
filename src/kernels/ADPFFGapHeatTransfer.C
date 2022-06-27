//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFGapHeatTransfer.h"

registerMooseObject("raccoonApp", ADPFFGapHeatTransfer);

InputParameters
ADPFFGapHeatTransfer::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params += BaseNameInterface::validParams();
  params.addRequiredCoupledVar("level_set", "The levelset variable");
  params.addRequiredCoupledVar("T_jump", "temperature jump in each direction");
  params.addRequiredCoupledVar("u_jump", "displacement jump");
  params.addRequiredCoupledVar("phase_field", "The phase field variable");
  params.addRequiredParam<MaterialPropertyName>("gap_conductivity", "Gap conductivity");
  params.addRequiredParam<Real>("min_gap", "Minimum gap");
  return params;
}

ADPFFGapHeatTransfer::ADPFFGapHeatTransfer(const InputParameters & parameters)
  : ADKernelValue(parameters),
    BaseNameInterface(parameters),
    _T_jump(adCoupledValues("T_jump")),
    _grad_phi(adCoupledGradient("level_set")),
    _gapK(getADMaterialPropertyByName<Real>(prependBaseName("gap_conductivity", true))),
    _u_jump(adCoupledValue("u_jump")),
    _grad_d(adCoupledGradient("phase_field")),
    _min_gap(getParam<Real>("min_gap"))
{
  _T_jump.resize(3, &_ad_zero);
}

ADReal
ADPFFGapHeatTransfer::precomputeQpResidual()
{
  ADReal value = 0;

  if (_grad_phi[_qp].norm() > 1e-12)
  {
    ADRealVectorValue n = _grad_phi[_qp] / _grad_phi[_qp].norm();
    ADRealVectorValue wT((*_T_jump[0])[_qp], (*_T_jump[1])[_qp], (*_T_jump[2])[_qp]);
    value = -wT * n * _gapK[_qp] / std::max(_u_jump[_qp], _min_gap) * _grad_d[_qp].norm();
  }

  return value;
}
