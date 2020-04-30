//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureContactKKT.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureContactKKT);

InputParameters
PhaseFieldFractureContactKKT::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addParam<MaterialPropertyName>("penetration_name",
                                        "penetration",
                                        "name of the material property to provide the penetration");
  return params;
}

PhaseFieldFractureContactKKT::PhaseFieldFractureContactKKT(const InputParameters & parameters)
  : ADKernelValue(parameters), _penetration(getADMaterialProperty<Real>("penetration_name"))
{
}

ADReal
PhaseFieldFractureContactKKT::precomputeQpResidual()
{
  return _u[_qp] - _penetration[_qp] -
         std::sqrt(_u[_qp] * _u[_qp] + _penetration[_qp] * _penetration[_qp] + 1e-6);
}
