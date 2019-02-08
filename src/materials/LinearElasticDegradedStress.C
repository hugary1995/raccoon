#include "LinearElasticDegradedStress.h"

registerMooseObject("raccoonApp", LinearElasticDegradedStress);

template <>
InputParameters
validParams<LinearElasticDegradedStress>()
{
  InputParameters params = validParams<DecompositionWarehouse>();
  params.addClassDescription(
      "Compute degraded stress based on either stress or strain decomposition "
      "of user choice, and compute elastic energy and its derivatives with "
      "respect to each coupled damage field");
  params.addCoupledVar(
      "initial_positive_elastic_energy", 0, "variable that holds the initial strain energy field");
  params.addParam<bool>(
      "history",
      true,
      "whether to use keep track of the maximum strain energy to enforce irreversibility");
  return params;
}

LinearElasticDegradedStress::LinearElasticDegradedStress(const InputParameters & parameters)
  : DecompositionWarehouse(parameters),
    _E_el_pos_name(_E_el_name + "_pos"),
    _E_el_pos(declareProperty<Real>(_E_el_pos_name)),
    _E_el_pos_maximum_old(getMaterialPropertyOldByName<Real>(_E_el_pos_name)),
    _E_el_pos_initial(coupledValue("initial_positive_elastic_energy")),
    _history(getParam<bool>("history"))

{
  // reserve space
  _dE_el_dd.resize(_num_fields);
  _d2E_el_dd2.resize(_num_fields);

  // declare dE_el_ddi
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    _dE_el_dd[i] = &declarePropertyDerivative<Real>(_E_el_name, _var_names[i]);
    _d2E_el_dd2[i] = &declarePropertyDerivative<Real>(_E_el_name, _var_names[i], _var_names[i]);
  }
}

void
LinearElasticDegradedStress::initQpStatefulProperties()
{
  decomposeStressAndComputePositiveStrainEnergy();
  _E_el_pos[_qp] = _E_el_pos_initial[_qp];
}

void
LinearElasticDegradedStress::computeQpStress()
{
  // decompose the stress based on either stress or strain decomposition
  // also set the elastic energy
  Real E_el_pos = decomposeStressAndComputePositiveStrainEnergy();

  // update the history maximum
  _E_el_pos[_qp] = E_el_pos;
  if (_history && E_el_pos < _E_el_pos_maximum_old[_qp])
    _E_el_pos[_qp] = _E_el_pos_maximum_old[_qp];

  // now set dE_el_ddi and d2E_el_ddi2
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    (*_dE_el_dd[i])[_qp] = (*_dg_dd[i])[_qp] * _E_el_pos_maximum_old[_qp];
    (*_d2E_el_dd2[i])[_qp] = (*_d2g_dd2[i])[_qp] * _E_el_pos_maximum_old[_qp];
  }
}
