#ifndef GreenStrainLinearElasticStress_H
#define GreenStrainLinearElasticStress_H

#include "ComputeStressBase.h"

class GreenStrainLinearElasticStress;

template <>
InputParameters validParams<GreenStrainLinearElasticStress>();

/**
 * GreenStrainLinearElasticStress computes the stress following linear elasticity theory
 */
class GreenStrainLinearElasticStress : public ComputeStressBase
{
public:
  GreenStrainLinearElasticStress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;

  /// Cauchy stress
  MaterialProperty<RankTwoTensor> * _cauchy_stress;

  /// deformation gradient
  const MaterialProperty<RankTwoTensor> * _deformation_gradient;
};

#endif // GreenStrainLinearElasticStress_H
