#ifndef ComputeEigenstrainFromFunctionEigenstress_H
#define ComputeEigenstrainFromFunctionEigenstress_H

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

#endif // ComputeEigenstrainFromFunctionEigenstress_H
