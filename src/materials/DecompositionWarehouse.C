#include "DecompositionWarehouse.h"
#include "libmesh/utility.h"
#include "libmesh/string_to_enum.h"

template <>
InputParameters
validParams<DecompositionWarehouse>()
{
  InputParameters params = validParams<ComputeStressBase>();
  params.addParam<bool>("staggered", false, "whether this is used in a staggered scheme");
  params.addRequiredParam<std::string>("decomposition", "which decomposition to use");
  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<std::string>(
      "lumped_degradation_name", "g", "name of the material that holds the lumped degradation");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the material that holds the elastic energy");
  return params;
}

DecompositionWarehouse::DecompositionWarehouse(const InputParameters & parameters)
  : ComputeStressBase(parameters),
    _staggered(getParam<bool>("staggered")),
    _decomposition(decomposition(getParam<std::string>("decomposition"))),
    _g_name(getParam<std::string>("lumped_degradation_name")),
    _g(getMaterialPropertyByName<Real>(_g_name)),
    _num_fields(coupledComponents("damage_fields")),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el(declareProperty<Real>(_E_el_name))
{
  // reserve space
  _var_names.resize(_num_fields);
  _dg_dd.resize(_num_fields);
  _d2g_dd2.resize(_num_fields);
  _dstress_dd.resize(_num_fields);

  // get variable names
  for (unsigned int i = 0; i < _num_fields; ++i)
    _var_names[i] = getVar("damage_fields", i)->name();

  // get all degradations's derivatives
  for (unsigned int i = 0; i < _num_fields; ++i)
  {
    _dg_dd[i] = &getMaterialPropertyDerivative<Real>(_g_name, _var_names[i]);
    _d2g_dd2[i] = &getMaterialPropertyDerivative<Real>(_g_name, _var_names[i], _var_names[i]);
  }

  // declare dstress_ddi
  for (unsigned int i = 0; i < _num_fields; ++i)
    _dstress_dd[i] =
        &declarePropertyDerivative<RankTwoTensor>(_base_name + "stress", _var_names[i]);
}

Real
DecompositionWarehouse::Macaulay(Real x)
{
  Real val = x > 0.0 ? x : 0.0;
  return val;
}

Real
DecompositionWarehouse::getQpPoissonsRatio()
{
  // Isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  return 0.5 * lambda / (lambda + mu);
}

Real
DecompositionWarehouse::getQpYoungsModulus()
{
  // Isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  return mu * (3.0 * lambda + 2.0 * mu) / (lambda + mu);
}

Real
DecompositionWarehouse::noDecomp()
{
  // uncracked strain and stress
  RankTwoTensor strain0 = _mechanical_strain[_qp];
  RankTwoTensor stress0 = _elasticity_tensor[_qp] * strain0;

  // correct Jacobian
  _Jacobian_mult[_qp] = _g[_qp] * _elasticity_tensor[_qp];

  // stress, off diagonal terms
  _stress[_qp] = _g[_qp] * stress0;
  for (unsigned int i = 0; i < _num_fields; ++i)
    (*_dstress_dd[i])[_qp] = (*_dg_dd[i])[_qp] * stress0;

  // driving Energy
  _E_el[_qp] = 0.5 * stress0.doubleContraction(strain0);
  return _E_el[_qp];
}

Real
DecompositionWarehouse::spectralStrainDecomp()
{
  // uncracked strain
  RankTwoTensor strain0 = _mechanical_strain[_qp];

  // Isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);

  // spectral decomposition and projection tensors
  std::vector<Real> eigvals, eigvals_pos(LIBMESH_DIM);
  RankTwoTensor eigvec;
  Real strain0tr, strain0tr_pos;
  RankTwoTensor strain0pos, stress0pos;
  RankFourTensor I4Symmetric(RankFourTensor::initIdentitySymmetricFour);
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  RankFourTensor I2I2 = I2.outerProduct(I2);
  RankFourTensor proj_pos;
  RankFourTensor Jacobian_pos;

  proj_pos = strain0.positiveProjectionEigenDecomposition(eigvals, eigvec);

  // Diagonal tensor of eigenvalues
  for (unsigned i = 0; i < LIBMESH_DIM; i++)
    eigvals_pos[i] = eigvals[i] >= 0 ? eigvals[i] : 0.0;
  RankTwoTensor eigval(eigvals);
  RankTwoTensor eigval_pos(eigvals_pos);

  strain0tr = strain0.trace();
  strain0tr_pos = Macaulay(strain0tr);

  // calculate the tensile (postive) and compressive (negative) parts of stress
  strain0pos = eigvec * eigval_pos * eigvec.transpose();
  stress0pos = lambda * strain0tr_pos * I2 + 2.0 * mu * strain0pos;

  // degraded stress
  _stress[_qp] = _elasticity_tensor[_qp] * strain0 - (1. - _g[_qp]) * stress0pos;
  for (unsigned int i = 0; i < _num_fields; ++i)
    (*_dstress_dd[i])[_qp] = (*_dg_dd[i])[_qp] * stress0pos;

  // correct jacobian
  Jacobian_pos = lambda * (strain0tr >= 0) * I2I2 + 2.0 * mu * proj_pos;
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp] - (1. - _g[_qp]) * Jacobian_pos;

  // Energy with positive principal strains
  _E_el[_qp] = 0.5 * lambda * strain0tr * strain0tr + mu * strain0.doubleContraction(strain0);
  Real E_el_pos =
      0.5 * lambda * strain0tr_pos * strain0tr_pos + mu * strain0pos.doubleContraction(strain0pos);
  return E_el_pos;
}

Real
DecompositionWarehouse::decomposeStressAndComputePositiveStrainEnergy()
{
  Real E_el_pos;
  if (_decomposition == NO_DECOMP)
    E_el_pos = noDecomp();
  else if (_decomposition == STRAIN_SPECTRAL)
    E_el_pos = spectralStrainDecomp();
  else
    mooseError("Undefined decomposition method.");

  return E_el_pos;
}

Decomposition
DecompositionWarehouse::decomposition(std::string name)
{
  Decomposition decomposition(INVALID);
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  if ("no_decomp" == name)
    decomposition = NO_DECOMP;
  else if ("strain_spectral" == name)
    decomposition = STRAIN_SPECTRAL;
  else
    ::mooseError("Invalid decomposition method found: ", name);

  return decomposition;
}
