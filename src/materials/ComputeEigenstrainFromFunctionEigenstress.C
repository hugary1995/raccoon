#include "ComputeEigenstrainFromFunctionEigenstress.h"
#include "RankTwoTensor.h"
#include "Function.h"

registerMooseObject("raccoonApp", ComputeEigenstrainFromFunctionEigenstress);

template <>
InputParameters
validParams<ComputeEigenstrainFromFunctionEigenstress>()
{
  InputParameters params = validParams<ComputeEigenstrainFromInitialStress>();
  return params;
}

ComputeEigenstrainFromFunctionEigenstress::ComputeEigenstrainFromFunctionEigenstress(
    const InputParameters & parameters)
  : ComputeEigenstrainFromInitialStress(parameters)
{
}

void
ComputeEigenstrainFromFunctionEigenstress::computeQpEigenstrain()
{
  RankTwoTensor eigen_stress;
  for (unsigned i = 0; i < LIBMESH_DIM; ++i)
    for (unsigned j = 0; j < LIBMESH_DIM; ++j)
      eigen_stress(i, j) = _initial_stress_fcn[i * LIBMESH_DIM + j]->value(_t, _q_point[_qp]);

  _eigenstrain[_qp] = -_elasticity_tensor[_qp].invSymm() * eigen_stress;
}
