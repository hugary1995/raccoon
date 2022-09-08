//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"
#include "Function.h"

// This class computes the deformation gradient

class ComputeThermalExpansionEigenDeformationGradient : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeThermalExpansionEigenDeformationGradient(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpProperties() override;

  // The thermal eigen deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fg;

  // The thermal expansion coefficient
  const Function & _alpha;

  // The current temperature
  const ADVariableValue & _T;

  // The reference temperature
  const VariableValue & _T0;
};
