//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "RankTwoTensorAnisoDiffusion.h"

registerMooseObject("raccoonApp", RankTwoTensorAnisoDiffusion);

InputParameters
RankTwoTensorAnisoDiffusion::validParams()
{
  InputParameters params = MatDiffusionBase<RankTwoTensor>::validParams();
  params.addClassDescription(
      "Diffusion equation Kernel that takes an anisotropic Diffusivity from a material property");
  return params;
}

RankTwoTensorAnisoDiffusion::RankTwoTensorAnisoDiffusion(const InputParameters & parameters)
  : MatDiffusionBase<RankTwoTensor>(parameters)
{
}
