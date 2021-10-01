//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"
#include "Function.h"

class ComputeEigenstrainFromFunctionInitialStress : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeEigenstrainFromFunctionInitialStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;

  ///Material property name for the eigenstrain tensor
  std::string _eigenstrain_name;

  ///Stores the current total eigenstrain
  ADMaterialProperty<RankTwoTensor> & _eigenstrain;

  /// initial stress components
  std::vector<const Function *> _initial_stress_fcn;
};
