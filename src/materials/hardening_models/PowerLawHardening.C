//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PowerLawHardening.h"

registerMooseObject("raccoonApp", PowerLawHardening);

InputParameters
PowerLawHardening::validParams()
{
  InputParameters params = PlasticHardeningModel::validParams();
  params.addRequiredParam<MaterialPropertyName>("yield_stress", "The yield stress $\\sigma_y$");
  params.addRequiredParam<MaterialPropertyName>("exponent",
                                                "The exponent n in the power law hardening $n$");
  params.addRequiredParam<MaterialPropertyName>(
      "reference_plastic_strain", "The $\\epsilon_0$ parameter in the power law hardening");

  return params;
}

PowerLawHardening::PowerLawHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    _sigma_y(getADMaterialProperty<Real>("yield_stress")),
    _n(getADMaterialProperty<Real>("exponent")),
    _ep0(getADMaterialProperty<Real>("reference_plastic_strain"))
{
}

ADReal
PowerLawHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
    return _n[_qp] * _sigma_y[_qp] * _ep0[_qp] / (_n[_qp] + 1) *
           (std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp] + 1) - 1);

  if (derivative == 1)
    return _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp]);

  if (derivative == 2)
    return _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp] - 1) / _n[_qp] / _ep0[_qp];

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}
