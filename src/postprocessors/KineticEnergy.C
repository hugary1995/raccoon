//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "KineticEnergy.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", KineticEnergy);
registerMooseObject("raccoonApp", ADKineticEnergy);

template <bool is_ad>
InputParameters
KineticEnergyTempl<is_ad>::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the total kinetic energy of the form $\\int_\\body "
                             "0.5\\rho \\dot{u} \\cdot \\dot{u} \\diff{V}$.");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<MaterialPropertyName>(
      "density", "density", "Name of material property containing density");
  return params;
}

template <bool is_ad>
KineticEnergyTempl<is_ad>::KineticEnergyTempl(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _rho(getGenericMaterialProperty<Real, is_ad>("density")),
    _ndisp(coupledComponents("displacements")),
    _vel_var(_ndisp)
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _vel_var[i] = &coupledDot("displacements", i);
}

template <bool is_ad>
Real
KineticEnergyTempl<is_ad>::computeQpIntegral()
{
  // Unfortunately, if _ndisp < 3, this will produce a segfault...
  // RealVectorValue v((*_vel_var[0])[_qp], (*_vel_var[1])[_qp], (*_vel_var[2])[_qp]);
  // return 0.5 * MetaPhysicL::raw_value(_rho[_qp]) * v * v;
  Real ans = 0;
  for (unsigned i = 0; i < _ndisp; ++i)
  {
    ans += (*_vel_var[i])[_qp] * (*_vel_var[i])[_qp];
  }
  return 0.5 * MetaPhysicL::raw_value(_rho[_qp]) * ans;
}

template class KineticEnergyTempl<false>;
template class KineticEnergyTempl<true>;
