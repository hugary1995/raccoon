#ifndef FollowerForceBC_H
#define FollowerForceBC_H

#include "ADIntegratedBC.h"

template <ComputeStage>
class FollowerForceBC;
template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

declareADValidParams(FollowerForceBC);

template <ComputeStage compute_stage>
class FollowerForceBC : public ADIntegratedBC<compute_stage>
{
public:
  FollowerForceBC(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  Function & _function_x;
  Function & _function_y;
  Function & _function_z;

  const unsigned int _component;

  const std::string _base_name;

  const ADMaterialProperty(RankTwoTensor) & _F;

  usingIntegratedBCMembers;
};

#endif /* FollowerForceBC_H */
