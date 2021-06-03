//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADLowerDimensionalWeakOutOfPlaneStress.h"

registerMooseObject("raccoonApp", ADLowerDimensionalWeakOutOfPlaneStress);

InputParameters
ADLowerDimensionalWeakOutOfPlaneStress::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("Out-of-plane stress for embedded lower-dimensional elements.");
  params.addParam<FunctionName>("out_of_plane_stress", "0", "The out of plane stress");
  params.set<bool>("use_displaced_mesh") = false;

  return params;
}

ADLowerDimensionalWeakOutOfPlaneStress::ADLowerDimensionalWeakOutOfPlaneStress(
    const InputParameters & parameters)
  : ADKernelValue(parameters),
    BaseNameInterface(parameters),
    _out_of_plane_stress(getFunction("out_of_plane_stress")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress"))),
    _Q(getMaterialProperty<RankTwoTensor>(
        prependBaseName("lower_dimensional_coordinate_transformation")))
{
}

ADReal
ADLowerDimensionalWeakOutOfPlaneStress::precomputeQpResidual()
{
  ADRankTwoTensor stress = _Q[_qp] * _stress[_qp] * _Q[_qp].transpose();
  return -stress(2, 2) - _out_of_plane_stress.value(_t, _q_point[_qp]);
}
