//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class HenckyComputeElasticPlasticStress : public Material
{
public:
  static InputParameters validParams();

  HenckyComputeElasticPlasticStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative_order = 0) const;

  void
  evd(const ADRankTwoTensor & A, std::vector<ADReal> & eigvals, ADRankTwoTensor & eigvecs) const;
  ADRankTwoTensor log(const ADRankTwoTensor & A) const;
  ADRankTwoTensor exp(const ADRankTwoTensor & A) const;

  /// Base name of the material system
  const std::string _base_name;

  /// Elasticity tensor material property
  const ADMaterialProperty<RankFourTensor> & _elasticity_tensor;

  /// Degradation function
  const ADMaterialProperty<Real> & _g;

  /// Plastic degradation function
  const ADMaterialProperty<Real> & _gp;

  // @{ Elastic energy
  const std::string _E_el_name;
  ADMaterialProperty<Real> & _E_el_active;
  // @}

  // @{ Plastic energy
  const std::string _E_pl_name;
  ADMaterialProperty<Real> & _E_pl;
  // @}

  /// Deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;

  // @{ Plastic deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fp;
  const MaterialProperty<RankTwoTensor> & _Fp_old;
  // @}

  // @{ Effective plastic strain
  ADMaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;
  // @}

  /// Cauchy stress
  ADMaterialProperty<RankTwoTensor> & _cauchy_stress;

  /// Deviatoric part of Kirchhoff stress
  ADMaterialProperty<RankTwoTensor> & _s;

  /// Norm of deviatoric Kirchhoff stress
  ADMaterialProperty<Real> & _s_norm;

  /// Flow direction
  ADMaterialProperty<RankTwoTensor> & _N;

  // @{ Hardening rule parameters
  const Real _yield_stress;
  const Real _n;
  const Real _ep0;
  // @}
};
