//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

#define usingWeakPlaneStressMembers                                                                \
  usingKernelMembers;                                                                              \
  using ADWeakPlaneStress<compute_stage>::_base_name;                                              \
  using ADWeakPlaneStress<compute_stage>::_stress;                                                 \
  using ADWeakPlaneStress<compute_stage>::_direction;

// Forward Declarations
template <ComputeStage>
class ADWeakPlaneStress;

declareADValidParams(ADWeakPlaneStress);

/**
 * ADWeakPlaneStress is the automatic differentiation version of WeakPlaneStress
 */
template <ComputeStage compute_stage>
class ADWeakPlaneStress : public ADKernel<compute_stage>
{
public:
  ADWeakPlaneStress(const InputParameters & parameters);

protected:
  ADResidual computeQpResidual() override;

  const std::string _base_name;

  const ADMaterialProperty(RankTwoTensor) & _stress;

  const unsigned int _direction;

  usingKernelMembers;
};
