//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureMaterial.h"

registerMooseObject("raccoonApp", FractureMaterial);

InputParameters
FractureMaterial::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription(
      "Compute interface coefficient kappa and mobility for a damage field based on provided "
      "energy release rate Gc and crack length scale L");
  params.addRequiredParam<FunctionName>("local_dissipation_norm",
                                        "norm of the local dissipation ||w(d)||");

  params.addParam<MaterialPropertyName>(
      "kappa_name", "kappa", "name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "mobility", "name of the material that holds the mobility");

  params.addParam<bool>(
      "constant_in_time", true, "whether the fracture properties are stationary in time");

  return params;
}

FractureMaterial::FractureMaterial(const InputParameters & parameters)
  : ADMaterial(parameters),
    _stationary(getParam<bool>("constant_in_time")),
    _Gc(getMaterialPropertyByName<Real>("energy_release_rate")),
    _L(getMaterialPropertyByName<Real>("phase_field_regularization_length")),
    _w_norm(getFunction("local_dissipation_norm")),
    _kappa(declareProperty<Real>(getParam<MaterialPropertyName>("kappa_name"))),
    _kappa_old(_stationary ? &getMaterialPropertyOldByName<Real>(
                                 getParam<MaterialPropertyName>("kappa_name"))
                           : nullptr),
    _M(declareADProperty<Real>(getParam<MaterialPropertyName>("mobility_name"))),
    _M_old(_stationary ? &getMaterialPropertyOldByName<Real>(
                             getParam<MaterialPropertyName>("mobility_name"))
                       : nullptr)
{
}

void
FractureMaterial::initQpStatefulProperties()
{
  // evaluate functions at t = 0
  Real L = _L[_qp];
  Real Gc = _Gc[_qp];
  Real c0 = _w_norm.value(0.0, _q_point[_qp]);

  _kappa[_qp] = 2.0 * L * L;
  _M[_qp] = Gc / c0 / L;
}

void
FractureMaterial::computeQpProperties()
{
  if (_stationary)
  {
    _kappa[_qp] = (*_kappa_old)[_qp];
    _M[_qp] = (*_M_old)[_qp];
  }
  else
  {
    Real L = _L[_qp];
    Real Gc = _Gc[_qp];
    Real c0 = _w_norm.value(_t, _q_point[_qp]);

    _kappa[_qp] = 2.0 * L * L;
    _M[_qp] = Gc / c0 / L;
  }
}
