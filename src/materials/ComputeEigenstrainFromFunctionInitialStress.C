//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeEigenstrainFromFunctionInitialStress.h"

registerMooseObject("raccoonApp", ComputeEigenstrainFromFunctionInitialStress);

InputParameters
ComputeEigenstrainFromFunctionInitialStress::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the eigenstrain given a predefined intial stress. The eigenstrain is "
      "defined as $\\strain_0 = - \\mathbb{C} : \\stress_0$. Isotropic linear elasticity is "
      "assumed.");

  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $\\K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $\\G$");
  params.addRequiredParam<MaterialPropertyName>(
      "eigenstrain_name",
      "Material property name for the eigenstrain tensor computed "
      "by this model. IMPORTANT: The name of this property must "
      "also be provided to the strain calculator.");
  params.addRequiredParam<std::vector<FunctionName>>(
      "initial_stress", "A list of functions describing the eigen stress.");
  return params;
}

ComputeEigenstrainFromFunctionInitialStress::ComputeEigenstrainFromFunctionInitialStress(
    const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _K(getADMaterialPropertyByName<Real>(prependBaseName("bulk_modulus", true))),
    _G(getADMaterialPropertyByName<Real>(prependBaseName("shear_modulus", true))),
    _eigenstrain_name(prependBaseName("eigenstrain_name", true)),
    _eigenstrain(declareADProperty<RankTwoTensor>(_eigenstrain_name))
{
  const std::vector<FunctionName> & fcn_names(
      getParam<std::vector<FunctionName>>("initial_stress"));
  const std::size_t num = fcn_names.size();

  if (num != 1 && num != 3 && num != 6 && num != 9)
    paramError(
        "initial_stress",
        name() + ": " +
            "Either 1, 3, 6, or 9 initial stress functions should be provided.  You supplied " +
            Moose::stringify(num) + ".");

  _initial_stress_fcn.resize(num);
  for (unsigned i = 0; i < num; ++i)
    _initial_stress_fcn[i] = &getFunctionByName(fcn_names[i]);
}

void
ComputeEigenstrainFromFunctionInitialStress::initQpStatefulProperties()
{
  computeQpProperties();
}

void
ComputeEigenstrainFromFunctionInitialStress::computeQpProperties()
{
  const std::size_t num = _initial_stress_fcn.size();
  std::vector<ADReal> initial_stress_vector(num);
  for (unsigned i = 0; i < num; ++i)
    initial_stress_vector[i] = _initial_stress_fcn[i]->value(_t, _q_point[_qp]);
  ADRankTwoTensor initial_stress;
  initial_stress.fillFromInputVector(initial_stress_vector);

  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  _eigenstrain[_qp] =
      -initial_stress.trace() / 9 / _K[_qp] * I2 - initial_stress.deviatoric() / 2 / _G[_qp];
}
