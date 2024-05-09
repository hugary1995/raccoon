//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "RaccoonUtils.h"
#include "ADReal.h"
#include "MooseUtils.h"
#include "RankTwoTensorImplementation.h"

namespace RaccoonUtils
{

ADReal
Macaulay(const ADReal x, const bool deriv)
{
  if (deriv)
    return x > 0 ? 1 : 0;
  return 0.5 * (x + std::abs(x));
}

std::vector<ADReal>
Macaulay(const std::vector<ADReal> & v, const bool deriv)
{
  std::vector<ADReal> m = v;
  for (auto & x : m)
    x = Macaulay(x, deriv);
  return m;
}

ADRankTwoTensor
spectralDecomposition(const ADRankTwoTensor & r2t)
{
  ADRankTwoTensor eigvecs;
  std::vector<ADReal> eigvals(LIBMESH_DIM);
  r2t.symmetricEigenvaluesEigenvectors(eigvals, eigvecs);

  ADRankTwoTensor eigvals_pos;
  eigvals_pos.fillFromInputVector(Macaulay(eigvals));
  return eigvecs * eigvals_pos * eigvecs.transpose();
}

ADRankTwoTensor
log(const ADRankTwoTensor & r2t)
{
  std::vector<ADReal> d;
  ADRankTwoTensor V, D;
  r2t.symmetricEigenvaluesEigenvectors(d, V);
  for (auto & di : d)
    di = std::log(di);
  D.fillFromInputVector(d);
  return V * D * V.transpose();
}

ADRankTwoTensor
exp(const ADRankTwoTensor & r2t)
{
  std::vector<ADReal> d;
  ADRankTwoTensor V, D;
  r2t.symmetricEigenvaluesEigenvectors(d, V);
  for (auto & di : d)
    di = std::exp(di);
  D.fillFromInputVector(d);
  return V * D * V.transpose();
}

} // end namespace MooseUtils
