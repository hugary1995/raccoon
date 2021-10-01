//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SideIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"

class LargeDeformationJIntegral : public SideIntegralPostprocessor, public BaseNameInterface
{
public:
  static InputParameters validParams();

  LargeDeformationJIntegral(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// The Cauchy stress
  const ADMaterialProperty<RankTwoTensor> & _stress;

  /// The strain energy density
  const ADMaterialProperty<Real> & _psie;

  /// The mechanical deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _Fm;

  /// direction of J integral
  const RealVectorValue _t;
};
