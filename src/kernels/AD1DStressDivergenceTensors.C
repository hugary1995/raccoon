//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "AD1DStressDivergenceTensors.h"

registerADMooseObject("raccoonApp", AD1DStressDivergenceTensors);

InputParameters
AD1DStressDivergenceTensors::validParams()
{
  InputParameters params = ADStressDivergenceTensors::validParams();
  params.addRequiredParam<MaterialPropertyName>(
      "cross_sectional_area", "material property that provides the cross sectional area");
  return params;
}

AD1DStressDivergenceTensors::AD1DStressDivergenceTensors(const InputParameters & parameters)
  : ADStressDivergenceTensors(parameters), _A(getMaterialProperty<Real>("cross_sectional_area"))
{
}

ADReal
AD1DStressDivergenceTensors::computeQpResidual()
{
  return _A[_qp] * ADStressDivergenceTensors::computeQpResidual();
}
