//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADIntegratedBC.h"

template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

class FollowerForceBC : public ADIntegratedBC
{
public:
  static InputParameters validParams();

  FollowerForceBC(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  ///{@
  /// forcing functions
  const Function & _function_x;
  const Function & _function_y;
  const Function & _function_z;
  ///@}

  /// component of the mapped forcing
  const unsigned int _component;

  /// base name of the material
  const std::string _base_name;

  /// the deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;
};
