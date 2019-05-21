//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ComputeEigenstrainFromInitialStress.h"

class ComputeEigenstrainFromFunctionEigenstress;

template <>
InputParameters validParams<ComputeEigenstrainFromFunctionEigenstress>();

class ComputeEigenstrainFromFunctionEigenstress : public ComputeEigenstrainFromInitialStress
{
public:
  ComputeEigenstrainFromFunctionEigenstress(const InputParameters & parameters);

protected:
  virtual void computeQpEigenstrain() override;
};

