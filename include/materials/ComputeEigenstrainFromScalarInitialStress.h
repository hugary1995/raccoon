//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ComputeEigenstrainBase.h"
#include "RankFourTensor.h"

/**
 * ComputeEigenstrain computes an Eigenstrain that results from an initial stress
 * The initial stress is defined in terms of scalars
 */
class ComputeEigenstrainFromScalarInitialStress : public ComputeEigenstrainBase
{
public:
  static InputParameters validParams();

  ComputeEigenstrainFromScalarInitialStress(const InputParameters & parameters);

protected:
  virtual void computeQpEigenstrain() override;

  /// base_name for elasticity tensor to use to convert stress to strain
  const std::string _base_name;

  /// elasticity tensor used to convert stress to strain
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;

  /// AuxVariables defining the initial stress
  const VariableValue & _sigma00;
  const VariableValue & _sigma01;
  const VariableValue & _sigma02;
  const VariableValue & _sigma10;
  const VariableValue & _sigma11;
  const VariableValue & _sigma12;
  const VariableValue & _sigma20;
  const VariableValue & _sigma21;
  const VariableValue & _sigma22;
};
