#include "SideIntegralPositiveMaxPrincipalStressPostprocessor.h"

registerMooseObject("raccoonApp", SideIntegralPositiveMaxPrincipalStressPostprocessor);

template <>
InputParameters
validParams<SideIntegralPositiveMaxPrincipalStressPostprocessor>()
{
  InputParameters params = validParams<SideIntegralPostprocessor>();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  return params;
}

SideIntegralPositiveMaxPrincipalStressPostprocessor::
    SideIntegralPositiveMaxPrincipalStressPostprocessor(const InputParameters & parameters)
  : SideIntegralPostprocessor(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialProperty<RankTwoTensor>(_base_name + "stress"))
{
}

Real
SideIntegralPositiveMaxPrincipalStressPostprocessor::computeQpIntegral()
{
  // get maximum eigenvalue of the stress tensor
  std::vector<Real> eigvals(LIBMESH_DIM);
  RankTwoTensor eigvecs;
  _stress[_qp].symmetricEigenvaluesEigenvectors(eigvals, eigvecs);
  return std::max(eigvals[LIBMESH_DIM - 1], 0.0);
}
