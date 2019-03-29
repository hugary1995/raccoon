#include "PiolaKirchhoffStressDivergence.h"
#include "ElasticityTensorTools.h"

registerMooseObject("raccoonApp", PiolaKirchhoffStressDivergence);

template <>
InputParameters
validParams<PiolaKirchhoffStressDivergence>()
{
  InputParameters params = validParams<ALEKernel>();
  params.addClassDescription(
      "Piola Kirchhoff stress divergence kernel for large deformation, NOTE: it reduces to small "
      "deformation if deformation gradient is not defined.");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<std::string>("base_name", "Material property base name");
  params.addCoupledVar("damage_fields", "use vector coupling for all coupled damage fields");
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

PiolaKirchhoffStressDivergence::PiolaKirchhoffStressDivergence(const InputParameters & parameters)
  : ALEKernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _large_deformation(
        hasMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _F(_large_deformation
           ? &getMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")
           : NULL),
    _Jacobian_mult(getMaterialPropertyByName<RankFourTensor>(_base_name + "Jacobian_mult")),
    _component(getParam<unsigned int>("component")),
    _ndisp(coupledComponents("displacements")),
    _disp_var(_ndisp),
    _ndamage(coupledComponents("damage_fields")),
    _d_var(_ndamage),
    _dstress_dd(_ndamage)
{
  // Coupled displacements
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var[i] = coupled("displacements", i);

  // Coupled damage fields
  for (unsigned int i = 0; i < _ndamage; ++i)
    _d_var[i] = coupled("damage_fields", i);

  // Get dstress_ddi
  for (unsigned int i = 0; i < _ndamage; ++i)
    _dstress_dd[i] = &getMaterialPropertyDerivative<RankTwoTensor>(
        _base_name + "stress", getVar("damage_fields", i)->name());
}

void
PiolaKirchhoffStressDivergence::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
  if (!_large_deformation)
    _console
        << "======================================================================================="
        << "\nDeformation gradient not defined, the PiolaKirchhoffStressDivergence kernel is "
           "\nreduced to small deformation formulation. Use a large deformation strain calculator "
           "\nto calculate the deformation gradient if you want to run a large deformation "
           "problem.\n"
        << "======================================================================================="
        << std::endl;
}

Real
PiolaKirchhoffStressDivergence::computeQpResidual()
{
  RealVectorValue stress =
      _large_deformation ? _stress[_qp] * (*_F)[_qp].row(_component) : _stress[_qp].row(_component);

  Real residual = _grad_test_undisplaced[_i][_qp] * stress;

  return residual;
}

Real
PiolaKirchhoffStressDivergence::computeQpJacobian()
{
  // jacobian = geometric stiffness  + material stiffness
  Real jacobian = 0.0;

  // Geometric stiffness
  jacobian += computeQpJacobianFromGeometricNonliearity();

  // Material stiffness
  jacobian += computeQpJacobianFromMaterialStiffness(_component, _component);

  return jacobian;
}

Real
PiolaKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  // off-diagonal Jacobian with respect to a coupled displacement component
  for (unsigned int coupled_component = 0; coupled_component < _ndisp; ++coupled_component)
    if (jvar == _disp_var[coupled_component])
      return computeQpJacobianFromMaterialStiffness(_component, coupled_component);

  // off-diagonal Jacobian with respect to a coupled damge field
  for (unsigned int i = 0; i < _ndamage; i++)
    if (jvar == _d_var[i])
      return computeQpJacobianFromDamage((*_dstress_dd[i])[_qp]);

  return 0.0;
}

Real
PiolaKirchhoffStressDivergence::computeQpJacobianFromGeometricNonliearity()
{
  if (!_large_deformation)
    return 0.0;

  // Geometric stiffness = W_{i,I}S_{IJ}U_{i,J}
  RealVectorValue gt = _grad_test_undisplaced[_i][_qp];
  RealVectorValue gp = _grad_phi_undisplaced[_j][_qp];
  RankTwoTensor S = _stress[_qp];

  return (S * gt) * gp;
}

Real
PiolaKirchhoffStressDivergence::computeQpJacobianFromMaterialStiffness(unsigned int i,
                                                                       unsigned int k)
{
  // Material stiffness =  W_{i,J}F_{iI}C_{IJKL}F_{kK}Phi_{k,L}
  RealVectorValue gt = _grad_test_undisplaced[_i][_qp];
  RealVectorValue gp = _grad_phi_undisplaced[_j][_qp];

  if (!_large_deformation)
    return ElasticityTensorTools::elasticJacobian(_Jacobian_mult[_qp], i, k, gt, gp);

  // material stiffness
  Real jacobian = 0.0;
  RankTwoTensor F = (*_F)[_qp];
  for (unsigned int I = 0; I < _ndisp; I++)
    for (unsigned int J = 0; J < _ndisp; J++)
      for (unsigned int K = 0; K < _ndisp; K++)
        for (unsigned int L = 0; L < _ndisp; L++)
          jacobian += gt(J) * F(i, I) * _Jacobian_mult[_qp](I, J, K, L) * F(k, K) * gp(L);

  return jacobian;
}

Real
PiolaKirchhoffStressDivergence::computeQpJacobianFromDamage(RankTwoTensor dstress_dd)
{
  if (_large_deformation)
    dstress_dd = (*_F)[_qp] * dstress_dd;

  // Damage jacobian contribution = W_{i,I} dstress_dd_{i,I} Phi
  return _grad_test[_i][_qp] * dstress_dd.row(_component) * _phi[_j][_qp];
}
