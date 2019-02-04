#include "LumpedDegradation.h"

registerMooseObject("raccoonApp", LumpedDegradation);

template <>
InputParameters
validParams<LumpedDegradation>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Lump all degradations associated with each coupled damage field, and "
                             "automatically computes their derivatives");
  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<std::string>(
      "lumped_degradation_name", "g", "name of the material that holds the lumped degradation");
  return params;
}

LumpedDegradation::LumpedDegradation(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters),
    _prop_name(getParam<std::string>("lumped_degradation_name")),
    _num_fields(coupledComponents("damage_fields"))
{
  // reserve space
  _var_names.resize(_num_fields);
  _g.resize(_num_fields);
  _dg_dd.resize(_num_fields);
  _d2g_dd2.resize(_num_fields);
  _dprop_dd.resize(_num_fields);
  _d2prop_dd2.resize(_num_fields);

  // get variable names
  for (unsigned int i = 0; i < _num_fields; ++i)
    _var_names[i] = getVar("damage_fields", i)->name();

  // get all degradations and their derivatives
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    _g[i] = &getMaterialProperty<Real>("g_" + _var_names[i]);
    _dg_dd[i] = &getMaterialPropertyDerivative<Real>("g_" + _var_names[i], _var_names[i]);
    _d2g_dd2[i] =
        &getMaterialPropertyDerivative<Real>("g_" + _var_names[i], _var_names[i], _var_names[i]);
  }

  // declare lumped degradation
  _prop = &declareProperty<Real>(_prop_name);

  // declare all derivatives of the lumped degradation
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    _dprop_dd[i] = &declarePropertyDerivative<Real>(_prop_name, _var_names[i]);
    _d2prop_dd2[i] = &declarePropertyDerivative<Real>(_prop_name, _var_names[i], _var_names[i]);
  }
}

void
LumpedDegradation::computeQpProperties()
{
  // compute the lumped degradation
  // g = g(d1)g(d2)...g(dn)
  (*_prop)[_qp] = 1.;
  for (unsigned int i = 0; i < _num_fields; ++i)
    (*_prop)[_qp] *= (*_g[i])[_qp];

  // compute derivatives
  // dprop_dd(d1) = dg_dd(d1)g(d2)...g(dn)
  // d2prop_dd2(d1) = d2g_dd2(d1)g(d2)...g(dn)
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    (*_dprop_dd[i])[_qp] = (*_prop)[_qp] / (*_g[i])[_qp] * (*_dg_dd[i])[_qp];
    (*_d2prop_dd2[i])[_qp] = (*_prop)[_qp] / (*_g[i])[_qp] * (*_d2g_dd2[i])[_qp];
  }
}
