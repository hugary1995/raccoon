//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelGrad.h"
#include "ADRankTwoTensorForward.h"

class ADNewtonianViscosity : public ADKernelGrad
{
public:
  static InputParameters validParams();

  ADNewtonianViscosity(const InputParameters & parameters);

protected:
  void initialSetup() override;

  ADRealVectorValue precomputeQpResidual() override;

  /// Base name of the material system that this kernel applies to
  const std::string _base_name;

  /// The deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;
  const MaterialProperty<RankTwoTensor> & _F_old;

  /// An integer corresponding to the direction this kernel acts in
  const unsigned int _component;

  /// The viscosity
  const ADMaterialProperty<Real> & _eta;
};
