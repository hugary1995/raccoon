//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "CNHDegradedElasticPlasticPK1StressBase.h"

class CNHDegradedElasticPlasticPK1Stress_LinearHardening
  : public CNHDegradedElasticPlasticPK1StressBase
{
public:
  static InputParameters validParams();

  CNHDegradedElasticPlasticPK1Stress_LinearHardening(const InputParameters & parameters);

protected:
  virtual ADReal H(ADReal ep) override;
  virtual ADReal dH_dep(ADReal ep) override;
  virtual ADReal d2H_dep2(ADReal ep) override;
  virtual ADReal plastic_dissipation(ADReal ep) override;

  const Real _yield_stress;
  const Real _k;
  const Real _W0;
};
