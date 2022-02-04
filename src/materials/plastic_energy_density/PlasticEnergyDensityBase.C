//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PlasticEnergyDensityBase.h"

InputParameters
PlasticEnergyDensityBase::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density", "psip", "Name of the plastic energy density");
  return params;
}

PlasticEnergyDensityBase::PlasticEnergyDensityBase(const InputParameters & params)
  : DerivativeMaterialInterface<Material>(params),
    BaseNameInterface(params),
    _ep(getADMaterialProperty<Real>(prependBaseName("effective_plastic_strain"))),
    _psip_name(prependBaseName("plastic_energy_density", true)),
    _psip(declareADProperty<Real>(_psip_name)),
    _dpsip_dep(declarePropertyDerivative<Real, true>(_psip_name,
                                                     {prependBaseName("effective_plastic_strain")}))
{
}

void
PlasticEnergyDensityBase::computeQpProperties()
{
  precomputeQpPlasticEnergyDensity();
  computeQpPlasticEnergyDensity();
  finalizeQpPlasticEnergyDensity();
}
