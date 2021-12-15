//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "TrescaStrengthSurface.h"

registerADMooseObject("raccoonApp", TrescaStrengthSurface);

InputParameters
TrescaStrengthSurface::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("This class computes the Tresca's strength surface");

  params.addRequiredParam<Real>("failure_strength",
                                "The strength of the material beyond which the material fails.");

  return params;
}

TrescaStrengthSurface::TrescaStrengthSurface(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _y(getParam<Real>("failure_strength")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress"))),
    _surface(declareADProperty<Real>("strength_surface"))
{
}

void
TrescaStrengthSurface::computeQpProperties()
{
  // Invariants of the stress
  std::vector<ADReal> eigvals;
  ADRankTwoTensor eigvecs;
  _stress[_qp].symmetricEigenvaluesEigenvectors(eigvals, eigvecs);
  _surface[_qp] = (eigvals[2] - eigvals[0]) - _y;
}
