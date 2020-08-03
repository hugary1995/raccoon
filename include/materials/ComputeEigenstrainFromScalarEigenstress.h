//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"
#include "ADRankTwoTensorForward.h"

template <bool is_ad>
class ComputeEigenstrainFromScalarEigenstressTempl : public Material
{
public:
  static InputParameters validParams();

  ComputeEigenstrainFromScalarEigenstressTempl(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  ///Base name prepended to material property name
  const std::string _base_name;

  ///Material property name for the eigenstrain tensor
  std::string _eigenstrain_name;

  ///Stores the current total eigenstrain
  GenericMaterialProperty<RankTwoTensor, is_ad> & _eigenstrain;

  /// elasticity tensor used to convert stress to strain
  const GenericMaterialProperty<RankFourTensor, is_ad> & _elasticity_tensor;

  /// initial stress components
  std::vector<const VariableValue *> _eigen_stress_scalar;
};

typedef ComputeEigenstrainFromScalarEigenstressTempl<false> ComputeEigenstrainFromScalarEigenstress;
typedef ComputeEigenstrainFromScalarEigenstressTempl<true>
    ADComputeEigenstrainFromScalarEigenstress;
