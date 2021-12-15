//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "HydrostaticStrengthSurface.h"

registerADMooseObject("raccoonApp", HydrostaticStrengthSurface);

InputParameters
HydrostaticStrengthSurface::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("This class computes the strength surface");

  params.addRequiredParam<Real>("failure_strength",
                                "The strength of the material beyond which the material fails.");

  return params;
}

HydrostaticStrengthSurface::HydrostaticStrengthSurface(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _y(getParam<Real>("failure_strength")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress"))),
    _surface(declareADProperty<Real>("strength_surface"))
{
}

void
HydrostaticStrengthSurface::computeQpProperties()
{
  // Invariants of the stress
  ADReal I1 = _stress[_qp].trace();
  _surface[_qp] = I1 - _y;
}
