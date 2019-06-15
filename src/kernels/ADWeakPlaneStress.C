//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADWeakPlaneStress.h"

registerADMooseObject("raccoonApp", ADWeakPlaneStress);

defineADValidParams(
    ADWeakPlaneStress,
    ADKernel,
    params.addClassDescription("Plane stress kernel to provide out-of-plane strain contribution");
    params.addRequiredCoupledVar("displacements",
                                 "The string of displacements suitable for the problem statement");
    params.addParam<std::string>("base_name", "Material property base name");
    MooseEnum direction("x y z", "z");
    params.addParam<MooseEnum>("out_of_plane_strain_direction",
                               direction,
                               "The direction of the out-of-plane strain variable");
    params.set<bool>("use_displaced_mesh") = false;);

template <ComputeStage compute_stage>
ADWeakPlaneStress<compute_stage>::ADWeakPlaneStress(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _base_name(isParamValid("base_name") ? adGetParam<std::string>("base_name") + "_" : ""),
    _stress(adGetADMaterialProperty<RankTwoTensor>(_base_name + "stress")),
    _direction(adGetParam<MooseEnum>("out_of_plane_strain_direction"))
{
}

template <ComputeStage compute_stage>
ADResidual
ADWeakPlaneStress<compute_stage>::computeQpResidual()
{
  return _stress[_qp](_direction, _direction) * _test[_i][_qp];
}

adBaseClass(ADWeakPlaneStress);
