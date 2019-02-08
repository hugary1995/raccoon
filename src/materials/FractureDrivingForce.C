#include "FractureDrivingForce.h"

registerMooseObject("raccoonApp", FractureDrivingForce);

template <>
InputParameters
validParams<FractureDrivingForce>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Compute the fracture driving force function and its derivatives for "
                             "all coupled damage fields");
  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_base_name",
      "w",
      "base name of the material that hold the local dissipation energy density");
  params.addParam<MaterialPropertyName>(
      "fracture_driving_force_base_name",
      "D",
      "base name of the material that holds the driving force, a specific "
      "field name will be appended");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the material that holds the elastic energy");
  params.addParam<MaterialPropertyName>(
      "mobility_base_name", "mobility", "base name of the material that holds the mobility");
  return params;
}

FractureDrivingForce::FractureDrivingForce(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters),
    _num_fields(coupledComponents("damage_fields")),
    _w_name(getParam<MaterialPropertyName>("local_dissipation_base_name") + "_"),
    _D_name(getParam<MaterialPropertyName>("fracture_driving_force_base_name") + "_"),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el(getMaterialPropertyByName<Real>(_E_el_name)),
    _mobility_base_name(getParam<MaterialPropertyName>("mobility_base_name") + "_")
{
  // reserve space
  _var_names.resize(_num_fields);
  _mobility.resize(_num_fields);

  _D.resize(_num_fields);
  _dD_dd.resize(_num_fields);
  _d2D_dd2.resize(_num_fields);

  _w.resize(_num_fields);
  _dw_dd.resize(_num_fields);
  _d2w_dd2.resize(_num_fields);

  _dE_el_dd.resize(_num_fields);
  _d2E_el_dd2.resize(_num_fields);

  // get variable names
  for (unsigned int i = 0; i < _num_fields; ++i)
    _var_names[i] = getVar("damage_fields", i)->name();

  // get mobility
  for (unsigned int i = 0; i < _num_fields; ++i)
    _mobility[i] = &getMaterialPropertyByName<Real>(_mobility_base_name + _var_names[i]);

  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    // declare driving force for each field
    _D[i] = &declareProperty<Real>(_D_name + _var_names[i]);
    _dD_dd[i] = &declarePropertyDerivative<Real>(_D_name + _var_names[i], _var_names[i]);
    _d2D_dd2[i] =
        &declarePropertyDerivative<Real>(_D_name + _var_names[i], _var_names[i], _var_names[i]);
    // get local dissipation energy
    _w[i] = &getMaterialPropertyByName<Real>(_w_name + _var_names[i]);
    _dw_dd[i] = &getMaterialPropertyDerivativeByName<Real>(_w_name + _var_names[i], _var_names[i]);
    _d2w_dd2[i] = &getMaterialPropertyDerivativeByName<Real>(
        _w_name + _var_names[i], _var_names[i], _var_names[i]);
    // get elastic energy
    _dE_el_dd[i] = &getMaterialPropertyDerivativeByName<Real>(_E_el_name, _var_names[i]);
    _d2E_el_dd2[i] =
        &getMaterialPropertyDerivativeByName<Real>(_E_el_name, _var_names[i], _var_names[i]);
  }
}

void
FractureDrivingForce::computeQpProperties()
{
  // compute driving force and its derivative
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    (*_D[i])[_qp] = _E_el[_qp] / (*_mobility[i])[_qp] + (*_w[i])[_qp];
    (*_dD_dd[i])[_qp] = (*_dE_el_dd[i])[_qp] / (*_mobility[i])[_qp] + (*_dw_dd[i])[_qp];
    (*_d2D_dd2[i])[_qp] = (*_d2E_el_dd2[i])[_qp] / (*_mobility[i])[_qp] + (*_d2w_dd2[i])[_qp];
  }
}
