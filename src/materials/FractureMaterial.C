//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureMaterial.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", FractureMaterial);
registerMooseObject("raccoonApp", ADFractureMaterial);

template <bool is_ad>
InputParameters
FractureMaterialTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "Compute interface coefficient kappa and mobility for a damage field based on provided "
      "energy release rate Gc and crack length scale L");
  params.addRequiredParam<FunctionName>("local_dissipation_norm",
                                        "norm of the local dissipation ||w(d)||");
  // params.addRequiredParam<Real>("local_dissipation_norm", "norm of the local dissipation
  // ||w(d)||");

  params.addParam<MaterialPropertyName>(
      "kappa_name", "kappa", "name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "mobility", "name of the material that holds the mobility");

  params.addParam<bool>(
      "constant_in_time", true, "whether the fracture properties are stationary in time");

  return params;
}
template <bool is_ad>
FractureMaterialTempl<is_ad>::FractureMaterialTempl(const InputParameters & parameters)
  : Material(parameters),
    _stationary(getParam<bool>("constant_in_time")),
    _Gc(getGenericMaterialProperty<Real, is_ad>("energy_release_rate")),
    _L(getGenericMaterialProperty<Real, is_ad>("phase_field_regularization_length")),
    _w_norm(getFunction("local_dissipation_norm")),
    // _w_norm(getParam<Real>("local_dissipation_norm")),
    _kappa(declareGenericProperty<Real, is_ad>(getParam<MaterialPropertyName>("kappa_name"))),
    _kappa_old(_stationary ? &getMaterialPropertyOld<Real>("kappa_name") : nullptr),
    _M(declareGenericProperty<Real, is_ad>(getParam<MaterialPropertyName>("mobility_name"))),
    _M_old(_stationary ? &getMaterialPropertyOld<Real>("mobility_name") : nullptr)
{
}

template <bool is_ad>
void
FractureMaterialTempl<is_ad>::initQpStatefulProperties()
{
  // evaluate functions at t = 0
  GenericReal<is_ad> L = _L[_qp];
  GenericReal<is_ad> Gc = _Gc[_qp];
  // GenericReal<is_ad> c0 = _w_norm;
  Real c0 = _w_norm.value(0.0, _q_point[_qp]);

  _kappa[_qp] = 2.0 * L * L;
  _M[_qp] = Gc / c0 / L;
}

template <bool is_ad>
void
FractureMaterialTempl<is_ad>::computeQpProperties()
{
  if (_stationary)
  {
    _kappa[_qp] = (*_kappa_old)[_qp];
    _M[_qp] = (*_M_old)[_qp];
  }
  else
  {
    GenericReal<is_ad> L = _L[_qp];
    GenericReal<is_ad> Gc = _Gc[_qp];
    // GenericReal<is_ad> c0 = _w_norm;
    Real c0 = _w_norm.value(_t, _q_point[_qp]);

    _kappa[_qp] = 2.0 * L * L;
    _M[_qp] = Gc / c0 / L;
  }
}

template class FractureMaterialTempl<false>;
template class FractureMaterialTempl<true>;
