#include "SmallStrainElasticDegradedStress.h"

registerMooseObject("raccoonApp", SmallStrainElasticDegradedStress);

template <>
InputParameters
validParams<SmallStrainElasticDegradedStress>()
{
  InputParameters params = validParams<DegradedStressBase>();
  params.addClassDescription("Compute stress and degrade it based on the lumped degradation "
                             "function. Also compute elastic energy and its derivatives .");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name",
      "E_el",
      "name of the material that holds the elastic energy normalized by mobility");
  params.addCoupledVar(
      "initial_positive_elastic_energy", 0, "variable that holds the initial strain energy field");
  params.addParam<MaterialPropertyName>(
      "mob_name", "M", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "fracture_energy_barrier_name", "b", "name of the fracture energy barrier");
  return params;
}

SmallStrainElasticDegradedStress::SmallStrainElasticDegradedStress(
    const InputParameters & parameters)
  : DegradedStressBase(parameters),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_pos_initial(coupledValue("initial_positive_elastic_energy")),
    _E_el_pos(declareProperty<Real>(_E_el_name + "_pos")),
    _E_el_pos_old(getMaterialPropertyOldByName<Real>(_E_el_name + "_pos"))
{
  // reserve space for normalized elastic energy and their derivatives
  _E_el.resize(_num_fields);
  _dE_el_dd.resize(_num_fields);
  _d2E_el_dd2.resize(_num_fields);
  // reserve space for mobility
  _M.resize(_num_fields);
  // reserve space for fracture energy barrier
  _E_crit.resize(_num_fields);

  // declare normalized elastic energy and their derivatives
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    _E_el[i] = &declareProperty<Real>(_E_el_name + "_" + _var_names[i]);
    _dE_el_dd[i] =
        &declarePropertyDerivative<Real>(_E_el_name + "_" + _var_names[i], _var_names[i]);
    _d2E_el_dd2[i] = &declarePropertyDerivative<Real>(
        _E_el_name + "_" + _var_names[i], _var_names[i], _var_names[i]);
  }

  // get mobility values for each damage field
  for (unsigned int i = 0; i < _num_fields; ++i)
    _M[i] = &getMaterialPropertyByName<Real>(getParam<MaterialPropertyName>("mob_name") + "_" +
                                             _var_names[i]);

  // get critical fracture energy
  for (unsigned int i = 0; i < _num_fields; ++i)
    _E_crit[i] = &getMaterialPropertyByName<Real>(
        getParam<MaterialPropertyName>("fracture_energy_barrier_name") + "_" + _var_names[i]);
}

void
SmallStrainElasticDegradedStress::initQpStatefulProperties()
{
  computeAdditionalFreeEnergy(_E_el_pos_initial[_qp]);
}

void
SmallStrainElasticDegradedStress::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);

  // uncracked strain, this is what we base decomposition upon
  const RankTwoTensor strain0 = _mechanical_strain[_qp];

  // spectral decomposition on strain
  // strain0 is passed in and it returns the positive spectral part of strain0 and its corresponding
  // projection tensor for jacobian correction
  RankTwoTensor strain_pos;
  RankFourTensor proj_pos = spectralDecomposition(strain0, strain_pos);

  // prepare some useful quantities
  const RankFourTensor I4Symmetric(RankFourTensor::initIdentitySymmetricFour);
  const RankTwoTensor I2(RankTwoTensor::initIdentity);
  const RankFourTensor I2I2 = I2.outerProduct(I2);
  Real strain0_tr = strain0.trace();

  // positive stress
  RankTwoTensor stress_pos = lambda * Macaulay(strain0_tr) * I2 + 2.0 * mu * strain_pos;

  // degraded stress
  _stress[_qp] = _elasticity_tensor[_qp] * strain0 - (1.0 - _g[_qp]) * stress_pos;
  for (unsigned int i = 0; i < _num_fields; ++i)
    (*_dstress_dd[i])[_qp] = (*_dg_dd[i])[_qp] * stress_pos;

  // correct jacobian
  RankFourTensor Jacobian_pos = lambda * dMacaulay(strain0_tr) * I2I2 + 2.0 * mu * proj_pos;
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp] - (1.0 - _g[_qp]) * Jacobian_pos;

  // positive elastic energy
  Real E_el_pos = 0.5 * lambda * Macaulay(strain0_tr) * Macaulay(strain0_tr) +
                  mu * strain_pos.doubleContraction(strain_pos);

  // store the positive elastic energy so that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;

  // enforce irreversibility if history is set to true
  if (_history && E_el_pos < _E_el_pos_old[_qp])
    _E_el_pos[_qp] = _E_el_pos_old[_qp];

  // NOTE: we always lag the elastic energy by one step to avoid convergence issues. The drawback is
  // that we need to use very small time steps to ensure consistency
  // compute normalized positive elastic energy as additional energy to feed into the Allen-Cahn
  // equation
  computeAdditionalFreeEnergy(_E_el_pos_old[_qp]);
}

void
SmallStrainElasticDegradedStress::computeAdditionalFreeEnergy(Real E_el_pos)
{
  // note that we normalize the additional free energy by mobility so that it is conformal with the
  // Allen-Cahn equation. See website theory for detailed derivation.
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    Real E = (*_E_crit[i])[_qp] + Macaulay(E_el_pos - (*_E_crit[i])[_qp]);
    (*_E_el[i])[_qp] = _g[_qp] * E / (*_M[i])[_qp];
    (*_dE_el_dd[i])[_qp] = (*_dg_dd[i])[_qp] * E / (*_M[i])[_qp];
    (*_d2E_el_dd2[i])[_qp] = (*_d2g_dd2[i])[_qp] * E / (*_M[i])[_qp];
  }
}
