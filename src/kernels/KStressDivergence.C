//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "KStressDivergence.h"
#include "ElasticityTensorTools.h"

registerMooseObject("raccoonApp", KStressDivergence);

template <>
InputParameters
validParams<KStressDivergence>()
{
  InputParameters params = validParams<ALEKernel>();
  params.addClassDescription(
      "Stress divergence kernel based on Kirchhoff stress for large "
      "deformation, NOTE: it reduces to smalld eformation if deformation gradient is not defined.");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<std::string>("base_name", "Material property base name");
  params.addCoupledVar("damage_fields", "use vector coupling for all coupled damage fields");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

KStressDivergence::KStressDivergence(const InputParameters & parameters)
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
KStressDivergence::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
  if (!_large_deformation)
    _console
        << "======================================================================================="
        << "\nDeformation gradient not defined, the KStressDivergence kernel is "
           "\nreduced to small deformation formulation. Use a large deformation strain calculator "
           "\nto calculate the deformation gradient if you want to run a large deformation "
           "problem.\n"
        << "======================================================================================="
        << std::endl;
}

Real
KStressDivergence::computeQpResidual()
{
  Real residual = _grad_test[_i][_qp] * _stress[_qp].row(_component) / (*_F)[_qp].det();

  return residual;
}

Real
KStressDivergence::computeQpJacobian()
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
KStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
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
KStressDivergence::computeQpJacobianFromGeometricNonliearity()
{
  if (!_large_deformation)
    return 0.0;

  RealVectorValue gt = _grad_test[_i][_qp];
  RealVectorValue gp = _grad_phi[_j][_qp];

  return (_stress[_qp] * gt) * gp / (*_F)[_qp].det();
}

Real
KStressDivergence::computeQpJacobianFromMaterialStiffness(unsigned int i, unsigned int k)
{
  // Material stiffness =  W_{i,J}F_{iI}C_{IJKL}F_{kK}Phi_{k,L}
  RealVectorValue gt = _grad_test[_i][_qp];
  RealVectorValue gp = _grad_phi[_j][_qp];

  return ElasticityTensorTools::elasticJacobian(_Jacobian_mult[_qp], i, k, gt, gp) /
         (*_F)[_qp].det();
}

Real
KStressDivergence::computeQpJacobianFromDamage(RankTwoTensor dstress_dd)
{
  if (_large_deformation)
    dstress_dd = dstress_dd * (*_F)[_qp].inverse().transpose();

  // Damage jacobian contribution = W_{i,I} dstress_dd_{i,I} Phi
  return _grad_test[_i][_qp] * dstress_dd.row(_component) * _phi[_j][_qp];
}

RankFourTensor
KStressDivergence::pullBack(const RankFourTensor & c)
{
  RankTwoTensor F = (*_F)[_qp];
  RankTwoTensor Finv = F.inverse();

  RankFourTensor C;
  for (unsigned int I = 0; I < _ndisp; I++)
    for (unsigned int J = 0; J < _ndisp; J++)
      for (unsigned int K = 0; K < _ndisp; K++)
        for (unsigned int L = 0; L < _ndisp; L++)
        {
          Real tmp = 0.0;
          for (unsigned int i = 0; i < _ndisp; i++)
            for (unsigned int j = 0; j < _ndisp; j++)
              for (unsigned int k = 0; k < _ndisp; k++)
                for (unsigned int l = 0; l < _ndisp; l++)
                  tmp += Finv(i, I) * Finv(j, J) * Finv(k, K) * Finv(l, L) * c(i, j, k, l);
          C(I, J, K, L) = tmp;
        }

  return C;
}
