//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "JohnsonCookHardening.h"

registerMooseObject("raccoonApp", JohnsonCookHardening);

InputParameters
JohnsonCookHardening::validParams()
{
  InputParameters params = PlasticHardeningModel::validParams();
  params.addClassDescription("The Johnson-Cook plasticity model.");

  params.addRequiredParam<MaterialPropertyName>("sigma_0",
                                                "The reference yield stress $\\sigma_0$");
  params.addRequiredParam<Real>("n", "The exponent n in the JC Model");
  params.addRequiredParam<Real>("m", "The exponent m in the JC Model");
  params.addRequiredParam<Real>("reference_plastic_strain",
                                "The $\\epsilon_0$ parameter in the JC Model");
  params.addRequiredParam<Real>("reference_plastic_strain_rate",
                                "The ref plastic strain rate parameter in the JC model");
  params.addRequiredParam<Real>("T0", "Reference temperature of the material");
  params.addRequiredCoupledVar("T", "Temperature");
  params.addRangeCheckedParam<Real>(
      "taylor_quinney_factor",
      1,
      "taylor_quinney_factor<=1 & taylor_quinney_factor>=0",
      "The Taylor-Quinney factor. 1 (default) for purely dissipative, 0 for purely energetic.");
  params.addRequiredParam<MaterialPropertyName>("A", "'A' parameter for the JC model");
  params.addRequiredParam<Real>("B", "'B' parameter for the JC model");
  params.addRequiredParam<Real>("C", "'C' parameter for the JC model");
  params.addRequiredParam<Real>("Tm", "The melting temperature of the material");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "psip",
      "Name of the plastic energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "gp", "The degradation function");
  return params;
}

JohnsonCookHardening::JohnsonCookHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _sigma_0(getADMaterialProperty<Real>(prependBaseName("sigma_0", true))),
    _n(getParam<Real>("n")),
    _m(getParam<Real>("m")),
    _ep0(getParam<Real>("reference_plastic_strain")),
    _epdot0(getParam<Real>("reference_plastic_strain_rate")),
    _T0(getParam<Real>("T0")),
    _T(coupledValueOld("T")),
    _tqf(getParam<Real>("taylor_quinney_factor")),
    _A(getADMaterialProperty<Real>("A")),
    _B(getParam<Real>("B")),
    _C(getParam<Real>("C")),
    _Tm(getParam<Real>("Tm")),
    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psip_name(prependBaseName("plastic_energy_density", true)),
    _psip(declareADProperty<Real>(_psip_name)),
    _psip_active(declareADProperty<Real>(_psip_name + "_active")),
    _dpsip_dd(declareADProperty<Real>(derivativePropertyName(_psip_name, {_d_name}))),

    // The degradation function and its derivatives
    _gp_name(prependBaseName("degradation_function", true)),
    _gp(getADMaterialProperty<Real>(_gp_name)),
    _dgp_dd(getADMaterialProperty<Real>(derivativePropertyName(_gp_name, {_d_name})))
{
}

ADReal // Temperature degradation
JohnsonCookHardening::temperatureDependence()
{
  return 1 - std::pow((_T[_qp] - _T0) / (_Tm - _T0), _m);
}

ADReal
JohnsonCookHardening::initialGuess(const ADReal & effective_trial_stress)
{
  ADReal trial_over_stress =
      effective_trial_stress / _sigma_0[_qp] / temperatureDependence() - _A[_qp];
  if (trial_over_stress < 0)
    trial_over_stress = 0;
  return std::max(_ep0 * std::pow(trial_over_stress / _B, 1 / _n),
                  libMesh::TOLERANCE * libMesh::TOLERANCE);
}

ADReal
JohnsonCookHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _psip_active[_qp] = (1 - _tqf) * _sigma_0[_qp] *
                        (_A[_qp] * ep + _B * _ep0 * std::pow(ep / _ep0, _n + 1) / (_n + 1)) *
                        temperatureDependence();
    _psip[_qp] = _gp[_qp] * _psip_active[_qp];
    _dpsip_dd[_qp] = _dgp_dd[_qp] * _psip_active[_qp];
    return _psip[_qp];
  }

  if (derivative == 1)
  {
    return (1 - _tqf) * _sigma_0[_qp] * (_A[_qp] + _B * std::pow(ep / _ep0, _n)) *
           temperatureDependence();
  }
  if (derivative == 2)
  {
    return (1 - _tqf) * _sigma_0[_qp] * _B * std::pow(ep / _ep0, _n - 1) * _n / _ep0 *
           temperatureDependence();
  }
  mooseError(name(), "internal error: unsupported derivative order.");
}

ADReal
JohnsonCookHardening::plasticDissipation(const ADReal & delta_ep,
                                         const ADReal & ep,
                                         const unsigned int derivative)
{
  ADReal result = 0;

  if (derivative == 0)
  {
    result += (_A[_qp] + _B * std::pow(ep / _ep0, _n)) * _tqf * delta_ep;
    if (_t_step > 0 && delta_ep > libMesh::TOLERANCE * libMesh::TOLERANCE)
      result += (_A[_qp] + _B * std::pow(ep / _ep0, _n)) *
                (_C * std::log(delta_ep / _dt / _epdot0) - _C) * delta_ep;
  }

  if (derivative == 1)
  {
    result += (_A[_qp] + _B * std::pow(ep / _ep0, _n)) * _tqf;
    if (_t_step > 0 && delta_ep > libMesh::TOLERANCE * libMesh::TOLERANCE)
      result +=
          (_A[_qp] + _B * std::pow(ep / _ep0, _n)) * (_C * std::log(delta_ep / _dt / _epdot0));
  }

  if (derivative == 2)
  {
    result += _B * std::pow(ep / _ep0, _n - 1) * _n / _ep0 * _tqf;
    if (_t_step > 0 && delta_ep > libMesh::TOLERANCE * libMesh::TOLERANCE)
      result +=
          (_A[_qp] + _B * std::pow(ep / _ep0, _n)) * _C / delta_ep +
          _B * std::pow(ep / _ep0, _n - 1) * _n / _ep0 * _C * std::log(delta_ep / _dt / _epdot0);
  }

  return result * _sigma_0[_qp] * temperatureDependence();

  mooseError(name(), "internal error: unsupported derivative order.");
}

ADReal // Thermal conjugate term
JohnsonCookHardening::thermalConjugate(const ADReal & ep)
{

  return _T[_qp] * (1 - _tqf) * _sigma_0[_qp] * (_A[_qp] + _B * std::pow(ep / _ep0, _n)) *
         (_m * (std::pow((_T0 - _T[_qp]) / (_T0 - _Tm), _m))) / (_T0 - _T[_qp]);
}
