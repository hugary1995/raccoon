//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
//* By: @Sina-av

#include "PowerLawHardeningPlasticEnergyDensity.h"

registerMooseObject("raccoonApp", PowerLawHardeningPlasticEnergyDensity);

InputParameters
PowerLawHardeningPlasticEnergyDensity::validParams()
{
  InputParameters params = PlasticEnergyDensityBase::validParams();
  params.addClassDescription("The plastic energy density that describes a power law hardening.");
  params.addRequiredParam<MaterialPropertyName>("yield_stress", "The yield stress $\\sigma_y$");
  params.addRequiredParam<MaterialPropertyName>("exponent",
                                                "The exponent n in the power law hardening $n$");
  params.addRequiredParam<MaterialPropertyName>(
      "reference_plastic_strain", "The $\\epsilon_0$ parameter in the power law hardening");
  return params;
}

PowerLawHardeningPlasticEnergyDensity::PowerLawHardeningPlasticEnergyDensity(
    const InputParameters & params)
  : PlasticEnergyDensityBase(params),
    _sigma_y(getADMaterialProperty<Real>(prependBaseName("yield_stress", true))),
    _n(getADMaterialProperty<Real>(prependBaseName("exponent", true))),
    _ep0(getADMaterialProperty<Real>(prependBaseName("reference_plastic_strain", true)))
{
}

ADReal
PowerLawHardeningPlasticEnergyDensity::flowStress(const ADReal & ep)
{
  return _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp]);
}

ADReal
PowerLawHardeningPlasticEnergyDensity::flowStressDerivative(const ADReal & ep)
{
  return _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp] - 1) / _n[_qp] / _ep0[_qp];
}

void
PowerLawHardeningPlasticEnergyDensity::computeQpPlasticEnergyDensity()
{
  _psip[_qp] = _n[_qp] * _sigma_y[_qp] * _ep0[_qp] / (_n[_qp] + 1) *
               (std::pow(1 + _ep[_qp] / _ep0[_qp], 1 / _n[_qp] + 1) - 1);
}

void
PowerLawHardeningPlasticEnergyDensity::finalizeQpPlasticEnergyDensity()
{
  _dpsip_dep[_qp] = flowStress(_ep[_qp]);
}
