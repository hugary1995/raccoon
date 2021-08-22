//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MatDiffusionBase.h"

/**
 * Anisotropic diffusion kernel that takes a diffusion coefficient of type
 * RankTwoTensor. All logic is implemnted in the MatDiffusionBase class
 * template.
 */
class RankTwoTensorAnisoDiffusion : public MatDiffusionBase<RankTwoTensor>
{
public:
  static InputParameters validParams();

  RankTwoTensorAnisoDiffusion(const InputParameters & parameters);
};
