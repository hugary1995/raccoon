//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
//* By: @Sina-av

#include "LinearHardeningPlasticEnergyDensity.h"

registerMooseObject("raccoonApp", LinearHardeningPlasticEnergyDensity);

InputParameters
LinearHardeningPlasticEnergyDensity::validParams()
{
  InputParameters params = PlasticEnergyDensityBase::validParams();
  params.addClassDescription("The plastic energy density that describes a linear hardening law.");
  params.addRequiredParam<MaterialPropertyName>("yield_stress",
                                                "The initial yield stress $\\sigma_y$");
  params.addRequiredParam<MaterialPropertyName>("hardening_modulus", "The hardening modulus $H$");
  return params;
}

LinearHardeningPlasticEnergyDensity::LinearHardeningPlasticEnergyDensity(
    const InputParameters & params)
  : PlasticEnergyDensityBase(params),
    _sigma_y(getADMaterialProperty<Real>(prependBaseName("yield_stress", true))),
    _H(getADMaterialProperty<Real>(prependBaseName("hardening_modulus", true)))
{
}

ADReal
LinearHardeningPlasticEnergyDensity::flowStress(const ADReal & ep)
{
  return _sigma_y[_qp] + _H[_qp] * ep;
}

ADReal
LinearHardeningPlasticEnergyDensity::flowStressDerivative(const ADReal & /*ep*/)
{
  return _H[_qp];
}

void
LinearHardeningPlasticEnergyDensity::computeQpPlasticEnergyDensity()
{
  _psip[_qp] = _sigma_y[_qp] * _ep[_qp] + 0.5 * _H[_qp] * _ep[_qp] * _ep[_qp];
}

void
LinearHardeningPlasticEnergyDensity::finalizeQpPlasticEnergyDensity()
{
  _dpsip_dep[_qp] = flowStress(_ep[_qp]);
}
