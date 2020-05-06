//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence);

InputParameters
CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1Stress_LinearHardening::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the local dissipation function");
  params.addRequiredParam<Real>("coalescence_coefficient", "coalescence coefficient");
  return params;
}

CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::
    CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence(
        const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1Stress_LinearHardening(parameters),
    _w(getADMaterialProperty<Real>("local_dissipation_name")),
    _c(getParam<Real>("coalescence_coefficient"))
{
}

ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::dH_dep(ADReal ep)
{
  return _gp * (_yield_stress + _c * _w[_qp] + _k * ep);
}
