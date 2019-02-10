#include "DegradedStressBase.h"

template <>
InputParameters
validParams<DegradedStressBase>()
{
  InputParameters params = validParams<ComputeStressBase>();
  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<MaterialPropertyName>(
      "degradation_name", "g", "name of the material that holds the lumped degradation");
  params.addParam<bool>(
      "history",
      true,
      "whether to use keep track of the maximum elastic energy to enforce irreversibility");
  return params;
}

DegradedStressBase::DegradedStressBase(const InputParameters & parameters)
  : ComputeStressBase(parameters),
    _num_fields(coupledComponents("damage_fields")),
    _g_name(getParam<MaterialPropertyName>("degradation_name")),
    _g(getMaterialPropertyByName<Real>(_g_name)),
    _history(getParam<bool>("history"))
{
  // reserve space for damage field names
  _var_names.resize(_num_fields);
  // reserve space for degradation derivatives
  _dg_dd.resize(_num_fields);
  _d2g_dd2.resize(_num_fields);
  // reserve space for derivative of stress W.R.T. each damage field
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
DegradedStressBase::Macaulay(Real x)
{
  Real val = x > 0.0 ? x : 0.0;
  return val;
}

Real
DegradedStressBase::dMacaulay(Real x)
{
  Real val = x > 0.0 ? 1.0 : 0.0;
  return val;
}

RankFourTensor
DegradedStressBase::spectralDecomposition(const RankTwoTensor A, RankTwoTensor & A_pos)
{
  std::vector<Real> eigvals(LIBMESH_DIM);
  std::vector<Real> eigvals_pos(LIBMESH_DIM);
  std::vector<Real> deivals_pos_deigvals(LIBMESH_DIM);
  RankTwoTensor eigvecs;

  // Compute eigenvectors and eigenvalues of this tensor
  A.symmetricEigenvaluesEigenvectors(eigvals, eigvecs);

  // Separate out positive and negative eigen values
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    eigvals_pos[i] = Macaulay(eigvals[i]);
    deivals_pos_deigvals[i] = dMacaulay(eigvals[i]);
  }

  // components of projection tensor
  RankFourTensor Gab, Gba;
  RankTwoTensor Ma, Mb;
  RankFourTensor proj_pos;

  for (unsigned int a = 0; a < LIBMESH_DIM; ++a)
  {
    Ma.vectorOuterProduct(eigvecs.column(a), eigvecs.column(a));
    proj_pos += deivals_pos_deigvals[a] * Ma.outerProduct(Ma);
  }

  for (unsigned int a = 0; a < LIBMESH_DIM; ++a)
    for (unsigned int b = 0; b < a; ++b)
    {
      Ma.vectorOuterProduct(eigvecs.column(a), eigvecs.column(a));
      Mb.vectorOuterProduct(eigvecs.column(b), eigvecs.column(b));

      Gab = Ma.mixedProductIkJl(Mb) + Ma.mixedProductIlJk(Mb);
      Gba = Mb.mixedProductIkJl(Ma) + Mb.mixedProductIlJk(Ma);

      Real theta_ab;
      if (!MooseUtils::absoluteFuzzyEqual(eigvals[a], eigvals[b]))
        theta_ab = 0.5 * (eigvals_pos[a] - eigvals_pos[b]) / (eigvals[a] - eigvals[b]);
      else
        theta_ab = 0.25 * (deivals_pos_deigvals[a] + deivals_pos_deigvals[b]);

      proj_pos += theta_ab * (Gab + Gba);
    }

  // Diagonal tensor of eigenvalues
  RankTwoTensor eigval(eigvals);
  RankTwoTensor eigval_pos(eigvals_pos);

  // calculate positive spectral part of A
  A_pos = eigvecs * eigval_pos * eigvecs.transpose();

  return proj_pos;
}
