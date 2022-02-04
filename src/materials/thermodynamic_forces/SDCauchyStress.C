//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SDCauchyStress.h"

registerMooseObject("raccoonApp", SDCauchyStress);

InputParameters
SDCauchyStress::validParams()
{
  InputParameters params = ThermodynamicForce::validParams();
  return params;
}

SDCauchyStress::SDCauchyStress(const InputParameters & params)
  : ThermodynamicForce(params),
    _dpsis_dmechanical_strain(getADThermodynamicConjugates<RankTwoTensor>(
        _psi_names, prependBaseName("mechanical_strain"))),
    _stress(declareADProperty<RankTwoTensor>(prependBaseName("stress")))
{
}

void
SDCauchyStress::computeQpProperties()
{
  _stress[_qp].zero();
  for (const auto * dpsi_dmechanical_strain : _dpsis_dmechanical_strain)
    _stress[_qp] += (*dpsi_dmechanical_strain)[_qp];
}
