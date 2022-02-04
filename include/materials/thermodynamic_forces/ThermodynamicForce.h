//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DerivativeMaterialInterface.h"
#include "BaseNameInterface.h"

class ThermodynamicForce : public DerivativeMaterialInterface<Material>, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ThermodynamicForce(const InputParameters & params);

protected:
  template <typename T>
  std::vector<const ADMaterialProperty<T> *>
  getADThermodynamicConjugates(const std::vector<MaterialPropertyName> & potential_names,
                               const MaterialPropertyName variable_name);

  /// Name of the energy densities
  const std::vector<MaterialPropertyName> _psi_names;
};
