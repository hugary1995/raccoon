//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeThermalExpansionEigenDeformationGradient.h"

registerADMooseObject("raccoonApp", ComputeThermalExpansionEigenDeformationGradient);

InputParameters
ComputeThermalExpansionEigenDeformationGradient::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the thermal deformation gradient, $\bfF = (1+\alpha(T-T_0))\bfI$, "
      "where $\alpha$ is the thermal expansion coefficient, $T$ is the temperature, and $T_0$ is "
      "the reference temperature corresponding to zero thermal expansion.");

  params.addRequiredParam<MaterialPropertyName>("eigen_deformation_gradient_name",
                                                "The name of the thermal deformation gradient");
  params.addRequiredParam<FunctionName>(
      "thermal_expansion_function",
      "Function describing the thermal expansion coefficient $\alpha$");
  params.addRequiredCoupledVar("temperature", "The current temperature");
  params.addRequiredCoupledVar("reference_temperature",
                               "The reference temperature corresponding to zero thermal expansion");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeThermalExpansionEigenDeformationGradient::ComputeThermalExpansionEigenDeformationGradient(
    const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _Fg(declareADProperty<RankTwoTensor>(prependBaseName("eigen_deformation_gradient_name", true))),
    _alpha(getFunction("thermal_expansion_function")),
    _T(adCoupledValue("temperature")),
    _T0(coupledValue("reference_temperature"))
{
}

void
ComputeThermalExpansionEigenDeformationGradient::initQpStatefulProperties()
{
  computeQpProperties();
}

void
ComputeThermalExpansionEigenDeformationGradient::computeQpProperties()
{
  const Point p;
  const Real alpha = _alpha.value(MetaPhysicL::raw_value(_T[_qp]), p);

  _Fg[_qp].setToIdentity();
  _Fg[_qp] *= 1 + alpha * (_T[_qp] - _T0[_qp]);
}
