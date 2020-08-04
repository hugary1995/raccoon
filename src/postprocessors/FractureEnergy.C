//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureEnergy.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", FractureEnergy);
registerMooseObject("raccoonApp", ADFractureEnergy);

template <bool is_ad>
InputParameters
FractureEnergyTempl<is_ad>::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the total fracture energy of the form $\\int_\\body M (w(d) "
                             "+ 0.5 \\kappa \\grad d \\cdot \\grad d) \\diff{V}$.");
  params.addRequiredCoupledVar("d", "damage variable");
  params.addParam<MaterialPropertyName>(
      "kappa_name", "kappa", "name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "mobility", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name",
      "w",
      "name of the material that holds the local dissipation function");
  return params;
}
template <bool is_ad>
FractureEnergyTempl<is_ad>::FractureEnergyTempl(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _kappa(getGenericMaterialProperty<Real, is_ad>(getParam<MaterialPropertyName>("kappa_name"))),
    _M(getGenericMaterialProperty<Real, is_ad>(getParam<MaterialPropertyName>("mobility_name"))),
    _grad_d(coupledGenericGradient<is_ad>("d")),
    _w(getGenericMaterialProperty<Real, is_ad>(
        getParam<MaterialPropertyName>("local_dissipation_name")))
{
}

template <bool is_ad>
Real
FractureEnergyTempl<is_ad>::computeQpIntegral()
{
  return MetaPhysicL::raw_value(_M[_qp] *
                                (_w[_qp] + 0.5 * _kappa[_qp] * _grad_d[_qp] * _grad_d[_qp]));
}

template class FractureEnergyTempl<false>;
template class FractureEnergyTempl<true>;
