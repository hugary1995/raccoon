//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeRightCauchyGreenStrain.h"
#include "RaccoonUtils.h"

registerADMooseObject("raccoonApp", ComputeRightCauchyGreenStrain);

InputParameters
ComputeRightCauchyGreenStrain::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the right Cauchy-Green strain $\\strain = \\bfF \\bfF^T$. The total "
      "strain is calculated based on the deformation gradient, and the mechanical strain is "
      "calculated based on the mechanical deformation gradient.");

  params.addParam<bool>("logarithmic", false, "Whether to compute the logarithmic strain.");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeRightCauchyGreenStrain::ComputeRightCauchyGreenStrain(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _F(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _Fm(getADMaterialPropertyByName<RankTwoTensor>(
        prependBaseName("mechanical_deformation_gradient"))),
    _total_strain(declareADProperty<RankTwoTensor>(prependBaseName("total_strain"))),
    _mechanical_strain(declareADProperty<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _logarithmic(getParam<bool>("logarithmic"))
{
}

void
ComputeRightCauchyGreenStrain::computeQpProperties()
{
  _total_strain[_qp] = _F[_qp].transpose() * _F[_qp];
  _mechanical_strain[_qp] = _Fm[_qp].transpose() * _Fm[_qp];

  if (_logarithmic)
  {
    _total_strain[_qp] = 0.5 * RaccoonUtils::log(_total_strain[_qp]);
    _mechanical_strain[_qp] = 0.5 * RaccoonUtils::log(_mechanical_strain[_qp]);
  }
}
