//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "CNHDegradedElasticPlasticPK1Stress_LinearHardening.h"

class CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence
  : public CNHDegradedElasticPlasticPK1Stress_LinearHardening
{
public:
  static InputParameters validParams();

  CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence(
      const InputParameters & parameters);

protected:
  virtual ADReal dH_dep(ADReal ep) override;
  virtual ADReal d2H_dep2(ADReal ep) override;

  const ADMaterialProperty<Real> & _w;
  const Real _beta;
  const Real _e0;
  const ADMaterialProperty<Real> & _M0;
};
