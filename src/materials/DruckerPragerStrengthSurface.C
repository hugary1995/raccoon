//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "DruckerPragerStrengthSurface.h"

registerADMooseObject("raccoonApp", DruckerPragerStrengthSurface);

InputParameters
DruckerPragerStrengthSurface::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("This class computes the strength surface");

  params.addRequiredParam<Real>(
      "tensile_strength", "The tensile strength of the material beyond which the material fails.");
  params.addRequiredParam<Real>(
      "compressive_strength",
      "The compressive strength of the material beyond which the material fails.");

  return params;
}

DruckerPragerStrengthSurface::DruckerPragerStrengthSurface(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _sigma_ts(getParam<Real>("tensile_strength")),
    _sigma_cs(getParam<Real>("compressive_strength")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress"))),
    _surface(declareADProperty<Real>("strength_surface"))
{
}

void
DruckerPragerStrengthSurface::computeQpProperties()
{
  // Invariants of the stress
  ADReal I1 = _stress[_qp].trace();
  ADRankTwoTensor stress_dev = _stress[_qp].deviatoric();
  ADReal J2 = 0.5 * stress_dev.doubleContraction(stress_dev);

  Real gamma_1 = (_sigma_cs - _sigma_ts) / std::sqrt(3) / (_sigma_cs + _sigma_ts);
  Real gamma_2 = 2 * _sigma_cs * _sigma_ts / std::sqrt(3) / (_sigma_cs + _sigma_ts);

  _surface[_qp] = std::sqrt(J2) + gamma_1 * I1 - gamma_2;
}
