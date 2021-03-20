//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADDarcyDiffusion.h"
#include "Assembly.h"

registerADMooseObject("raccoonApp", ADDarcyDiffusion);

InputParameters
ADDarcyDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Kernel that computes the diffusion term in the Darcy Law for porous flow");
  params.addParam<MaterialPropertyName>("permeability_name", "permeability", "material property that holds the permeability tensor");
  params.addParam<MaterialPropertyName>("density_name", "density", "material property that holds the density (compressibility equation)");
  return params;
}

ADDarcyDiffusion::ADDarcyDiffusion(
    const InputParameters & parameters)
  : ADKernel(parameters),
    _permeability(getADMaterialProperty<RankTwoTensor>("permeability_name")),
    _density(getADMaterialProperty<Real>("density_name"))
{
}

ADReal
ADDarcyDiffusion::computeQpResidual()
{
  ADReal residual = -_density[_qp] * _grad_test[_i][_qp] * (_permeability[_qp] *  _grad_u[_qp]);

  return residual;
}
