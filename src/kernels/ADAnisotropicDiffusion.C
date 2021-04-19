//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADAnisotropicDiffusion.h"

registerMooseObject("raccoonApp", ADAnisotropicDiffusion);

InputParameters
ADAnisotropicDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Kernel handling fluid diffusion in domain with anisotropic permeability");
  params.addRequiredParam<MaterialPropertyName>(
      "mobility_name", "Name of fluid mobility material property");
  params.addCoupledVar("v", "Coupled variable of the kernel");
  return params;
}

ADAnisotropicDiffusion::ADAnisotropicDiffusion(const InputParameters & parameters)
  : ADKernel(parameters), 
  _grad_v(isCoupled("v") ? adCoupledGradient("v") : _grad_u),
  _fluid_mob(getADMaterialProperty<RankTwoTensor>("mobility_name"))
{
}

ADReal
ADAnisotropicDiffusion::computeQpResidual()
{
  return (_fluid_mob[_qp] * _grad_v[_qp]) * _grad_test[_i][_qp];
}
