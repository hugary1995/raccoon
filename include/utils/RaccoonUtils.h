//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADRankTwoTensorForward.h"

namespace RaccoonUtils
{
ADReal Macaulay(const ADReal x, const bool deriv = false);

std::vector<ADReal> Macaulay(const std::vector<ADReal> & v, const bool deriv = false);

ADRankTwoTensor spectralDecomposition(const ADRankTwoTensor & r2t);

ADRankTwoTensor log(const ADRankTwoTensor & r2t);

ADRankTwoTensor exp(const ADRankTwoTensor & r2t);
}
