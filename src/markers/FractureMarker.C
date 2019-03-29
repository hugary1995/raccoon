#include "FractureMarker.h"

registerMooseObject("raccoonApp", FractureMarker);

template <>
InputParameters
validParams<FractureMarker>()
{
  InputParameters params = validParams<FractureMarkerBase>();
  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the local dissipation density");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name",
      "E_el",
      "name of the material that holds the elastic energy normalized by mobility");
  return params;
}

FractureMarker::FractureMarker(const InputParameters & parameters)
  : DerivativeMaterialInterface<FractureMarkerBase>(parameters),
    CoupleableMooseVariableDependencyIntermediateInterface(this, true),
    _ndamage(coupledComponents("damage_fields")),
    _dE_el_dd(_ndamage),
    _dw_dd(_ndamage)
{
  for (unsigned int i = 0; i < _ndamage; ++i)
  {
    std::string _var_name = getVar("damage_fields", i)->name();
    _dE_el_dd[i] = &getMaterialPropertyDerivative<Real>(
        getParam<MaterialPropertyName>("elastic_energy_name") + "_" + _var_name, _var_name);
    _dw_dd[i] = &getMaterialPropertyDerivative<Real>(
        getParam<MaterialPropertyName>("local_dissipation_name") + "_" + _var_name, _var_name);
  }
}

Marker::MarkerValue
FractureMarker::computeQpMarker()
{
  for (unsigned int i = 0; i < _ndamage; ++i)
    if ((*_dE_el_dd[i])[_qp] + (*_dw_dd[i])[_qp] > 0)
      return REFINE;

  return DONT_MARK;
}
