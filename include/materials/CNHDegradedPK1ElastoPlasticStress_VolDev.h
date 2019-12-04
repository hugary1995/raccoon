//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADDegradedStressBase.h"

template <ComputeStage>
class CNHDegradedPK1ElastoPlasticStress_VolDev;

declareADValidParams(CNHDegradedPK1ElastoPlasticStress_VolDev);

template <ComputeStage compute_stage>
class CNHDegradedPK1ElastoPlasticStress_VolDev : public ADDegradedStressBase<compute_stage>
{
public:
  CNHDegradedPK1ElastoPlasticStress_VolDev(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;

  const ADMaterialProperty(RankTwoTensor) & _deformation_gradient;
  ADMaterialProperty(RankTwoTensor) & _be_bar;
  ADMaterialProperty(Real) & _alpha;
  const MaterialProperty<RankTwoTensor> & _deformation_gradient_old;
  const MaterialProperty<RankTwoTensor> & _be_bar_old;
  const MaterialProperty<Real> & _alpha_old;
  const Real _yield_stress;
  const Real _k;
  ADMaterialProperty(RankTwoTensor) & _plastic_strain;
  ADMaterialProperty(RankTwoTensor) & _cauchy_stress;
  ADMaterialProperty(Real) & _Wp;
  const MaterialProperty<Real> & _Wp_old;
  ADMaterialProperty(Real) & _Wp_degraded;
  const Real _W0;
  ADMaterialProperty(Real) & _E_el_degraded;

  usingDegradedStressBaseMembers
};
