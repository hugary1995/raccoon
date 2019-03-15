#include "PiolaKirchhoffStressDivergence.h"
#include "ElasticityTensorTools.h"

registerMooseObject("raccoonApp", PiolaKirchhoffStressDivergence);

template <>
InputParameters
validParams<PiolaKirchhoffStressDivergence>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Piola Kirchhoff stress divergence kernel for the Cartesian coordinate system");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<std::string>("base_name", "Material property base name");
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

PiolaKirchhoffStressDivergence::PiolaKirchhoffStressDivergence(const InputParameters & parameters)
  : Kernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _F(getMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _Jacobian_mult(getMaterialPropertyByName<RankFourTensor>(_base_name + "Jacobian_mult")),
    _component(getParam<unsigned int>("component")),
    _ndisp(coupledComponents("displacements")),
    _disp_var(_ndisp)
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp_var[i] = coupled("displacements", i);
}

void
PiolaKirchhoffStressDivergence::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
}

Real
PiolaKirchhoffStressDivergence::computeQpResidual()
{
  RealVectorValue P = _stress[_qp] * _F[_qp].row(_component);

  Real residual = _grad_test[_i][_qp] * P;

  return residual;
}

Real
PiolaKirchhoffStressDivergence::computeQpJacobian()
{
  // Real jacobian = 0.0;
  //
  // // jacobian = W_{i,I}S_{IJ}U_{i,J} + W_{i,J} F_{iI}C_{IJKL}F_{kK} Phi_{k,L}
  // //          = geometric stiffness  + material stiffness
  //
  // // geometric stiffness
  // jacobian += _stress[_qp] * _grad_test[_i][_qp] * _grad_phi[_j][_qp];
  //
  // // material stiffness
  // RankFourTensor FCF = _Jacobian_mult[_qp];
  // for (unsigned int I = 0; I < _ndisp; I++)
  //   for (unsigned int K = 0; K < _ndisp; K++)
  //     for (unsigned int L = 0; L < _ndisp; L++)
  //       for (unsigned int J = 0; J < _ndisp; J++)
  //         FCF(I, J, K, L) *= _F[_qp](_component, I) * _F[_qp](_component, K);
  //
  // jacobian += ElasticityTensorTools::elasticJacobian(
  //     FCF, _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp]);
  //
  // return jacobian;

  Real jacobian = 0.0;
  RealVectorValue gt = _grad_test[_i][_qp];
  RealVectorValue gp = _grad_phi[_j][_qp];
  RankTwoTensor S = _stress[_qp];
  RankTwoTensor F = _F[_qp];

  unsigned int i = _component;
  unsigned int k = _component;

  for (unsigned int I = 0; I < _ndisp; I++)
    for (unsigned int J = 0; J < _ndisp; J++)
      for (unsigned int K = 0; K < _ndisp; K++)
        for (unsigned int L = 0; L < _ndisp; L++)
          jacobian += gt(I) * S(I, J) * gp(J) +
                      gt(J) * F(i, I) * _Jacobian_mult[_qp](I, J, K, L) * F(k, K) * gp(L);

  return jacobian;
}

Real
PiolaKirchhoffStressDivergence::computeQpOffDiagJacobian(unsigned int jvar)
{
  // off-diagonal Jacobian with respect to a coupled displacement component
  for (unsigned int coupled_component = 0; coupled_component < _ndisp; ++coupled_component)
    if (jvar == _disp_var[coupled_component])
    {
      // Real jacobian = 0.0;
      //
      // // jacobian = W_{i,I}S_{IJ}U_{i,J} + W_{i,J}F_{iI}C_{IJKL}F_{kK}Phi_{k,L}
      // //          = geometric stiffness  + material stiffness
      //
      // // geometric stiffness
      // jacobian += _stress[_qp] * _grad_test[_i][_qp] * _grad_phi[_j][_qp];
      //
      // // material stiffness
      // RankFourTensor FCF = _Jacobian_mult[_qp];
      // for (unsigned int I = 0; I < _ndisp; I++)
      //   for (unsigned int K = 0; K < _ndisp; K++)
      //     for (unsigned int L = 0; L < _ndisp; L++)
      //       for (unsigned int J = 0; J < _ndisp; J++)
      //         FCF(I, J, K, L) *= _F[_qp](_component, I) * _F[_qp](coupled_component, K);
      //
      // jacobian += ElasticityTensorTools::elasticJacobian(
      //     FCF, _component, coupled_component, _grad_test[_i][_qp], _grad_phi[_j][_qp]);
      //
      // return jacobian;

      Real jacobian = 0.0;
      RealVectorValue gt = _grad_test[_i][_qp];
      RealVectorValue gp = _grad_phi[_j][_qp];
      RankTwoTensor S = _stress[_qp];
      RankTwoTensor F = _F[_qp];

      unsigned int i = _component;
      unsigned int k = coupled_component;

      for (unsigned int I = 0; I < _ndisp; I++)
        for (unsigned int J = 0; J < _ndisp; J++)
          for (unsigned int K = 0; K < _ndisp; K++)
            for (unsigned int L = 0; L < _ndisp; L++)
              jacobian += gt(I) * S(I, J) * gp(J) +
                          gt(J) * F(i, I) * _Jacobian_mult[_qp](I, J, K, L) * F(k, K) * gp(L);

      return jacobian;
    }

  return 0.0;
}
