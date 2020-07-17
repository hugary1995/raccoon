//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "StrainEnergy.h"

registerMooseObject("raccoonApp", StrainEnergy);

InputParameters
StrainEnergy::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the total strain energy of the form $\\int_\\body 0.5 "
                             "\\sigma \\colon \\varepsilon \\diff{V}$.");
  params.addParam<std::string>("base_name", "base name for stress and strain");
  return params;
}

StrainEnergy::StrainEnergy(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "mechanical_strain"))
{
}

Real
StrainEnergy::computeQpIntegral()
{
  return 0.5 * (_stress[_qp].doubleContraction(_strain[_qp])).value();
}
