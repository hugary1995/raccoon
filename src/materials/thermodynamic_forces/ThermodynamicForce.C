//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ThermodynamicForce.h"

InputParameters
ThermodynamicForce::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "energy_densities",
      "Names of the energy densities. It is assumed that the total thermodynamic potential "
      "is additively decomposed into the sum of energy densities and dissipation densities.");
  return params;
}

ThermodynamicForce::ThermodynamicForce(const InputParameters & params)
  : DerivativeMaterialInterface<Material>(params),
    BaseNameInterface(params),
    _psi_names(prependBaseName(getParam<std::vector<MaterialPropertyName>>("energy_densities")))
{
}

template <typename T>
std::vector<const ADMaterialProperty<T> *>
ThermodynamicForce::getADThermodynamicConjugates(
    const std::vector<MaterialPropertyName> & potential_names,
    const MaterialPropertyName variable_name)
{
  std::vector<const ADMaterialProperty<T> *> conjugates;
  for (auto potential_name : potential_names)
    conjugates.push_back(
        &getMaterialPropertyDerivativeByName<T, true>(potential_name, {variable_name}));
  return conjugates;
}
