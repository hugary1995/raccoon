//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADAnisotropicDiffusion.h"

registerMooseObject("raccoonApp", ADAnisotropicDiffusion);

InputParameters
ADAnisotropicDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Kernel handling diffusion with anisotropic diffusivity");
  params.addRequiredParam<MaterialPropertyName>(
      "diffusivity", "The anisotropic diffusivity, should be a second-order tensor.");
  params.addCoupledVar(
      "coupled_var",
      "Coupled variable of the kernel. If a variable is coupled, this kernel will operate on the "
      "coupled variable. If not, this kernel will operate on the variable in the regular way.");
  return params;
}

ADAnisotropicDiffusion::ADAnisotropicDiffusion(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _grad_v(isCoupled("coupled_var") ? adCoupledGradient("coupled_var") : _grad_u),
    _D(getADMaterialProperty<RankTwoTensor>(prependBaseName("diffusivity", true)))
{
}

ADReal
ADAnisotropicDiffusion::computeQpResidual()
{
  return (_D[_qp] * _grad_v[_qp]) * _grad_test[_i][_qp];
}
