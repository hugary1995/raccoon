//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "CNHDegradedElasticPlasticPK1StressBase.h"

template <ComputeStage>
class CNHDegradedElasticPlasticPK1Stress_PowerLawHardening;

declareADValidParams(CNHDegradedElasticPlasticPK1Stress_PowerLawHardening);

template <ComputeStage compute_stage>
class CNHDegradedElasticPlasticPK1Stress_PowerLawHardening
  : public CNHDegradedElasticPlasticPK1StressBase<compute_stage>
{
public:
  static InputParameters validParams();

  CNHDegradedElasticPlasticPK1Stress_PowerLawHardening(const InputParameters & parameters);

protected:
  virtual ADReal H(ADReal ep) override;
  virtual ADReal dH_dep(ADReal ep) override;
  virtual ADReal d2H_dep2(ADReal ep) override;

private:
  Real _yield_stress;
  Real _m;
  Real _n;

  usingCNHDegradedElasticPlasticPK1StressMembers
};
