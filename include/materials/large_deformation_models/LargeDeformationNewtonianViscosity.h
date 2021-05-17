//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationViscoelasticityModel.h"

class LargeDeformationNewtonianViscosity : public LargeDeformationViscoelasticityModel
{
public:
  static InputParameters validParams();

  LargeDeformationNewtonianViscosity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeCauchyStress(const ADRankTwoTensor & Fm,
                                              const RankTwoTensor & Fm_old) override;

protected:
  /// The viscosity
  const ADMaterialProperty<Real> & _eta;

  /// The degradation function
  const ADMaterialProperty<Real> & _g;
};
