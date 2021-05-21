//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADWeakOutOfPlaneStress.h"

registerMooseObject("raccoonApp", ADWeakOutOfPlaneStress);

InputParameters
ADWeakOutOfPlaneStress::validParams()
{
  InputParameters params = ADWeakPlaneStress::validParams();
  params.addClassDescription(
      "This class weakly enforces the out of plane stress to be a given value.");
  params.addParam<FunctionName>("out_of_plane_stress", "0", "The out of plane stress");

  return params;
}

ADWeakOutOfPlaneStress::ADWeakOutOfPlaneStress(const InputParameters & parameters)
  : ADWeakPlaneStress(parameters), _out_of_plane_stress(getFunction("out_of_plane_stress"))
{
}

ADReal
ADWeakOutOfPlaneStress::precomputeQpResidual()
{
  return ADWeakPlaneStress::precomputeQpResidual() - _out_of_plane_stress.value(_t, _q_point[_qp]);
}
