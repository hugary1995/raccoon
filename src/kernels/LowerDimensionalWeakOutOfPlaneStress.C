//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LowerDimensionalWeakOutOfPlaneStress.h"

registerMooseObject("raccoonApp", LowerDimensionalWeakOutOfPlaneStress);

InputParameters
LowerDimensionalWeakOutOfPlaneStress::validParams()
{
  InputParameters params = WeakPlaneStress::validParams();

  params.addClassDescription("Out-of-plane stress for embedded lower-dimensional elements.");
  params.addParam<FunctionName>("out_of_plane_stress", "0", "The out of plane stress");

  return params;
}

LowerDimensionalWeakOutOfPlaneStress::LowerDimensionalWeakOutOfPlaneStress(
    const InputParameters & parameters)
  : WeakPlaneStress(parameters),
    _out_of_plane_stress(getFunction("out_of_plane_stress")),
    _Q(getMaterialProperty<RankTwoTensor>(_base_name +
                                          "lower_dimensional_coordinate_transformation"))
{
}

Real
LowerDimensionalWeakOutOfPlaneStress::computeQpResidual()
{
  RealVectorValue n(-_q_point[_qp](0), -_q_point[_qp](1), 0);
  n /= n.norm();
  return (_stress[_qp] * n) * n + _out_of_plane_stress.value(_t, _q_point[_qp]);
  // RankTwoTensor stress = _Q[_qp] * _stress[_qp] * _Q[_qp].transpose();
  // return stress(2, 2) + _out_of_plane_stress.value(_t, _q_point[_qp]);
}

Real
LowerDimensionalWeakOutOfPlaneStress::computeQpJacobian()
{
  RealVectorValue n(-_q_point[_qp](0), -_q_point[_qp](1), 0);
  n /= n.norm();
  RankTwoTensor nn;
  nn.vectorOuterProduct(n, n);
  RankTwoTensor Jp = nn.initialContraction(_Jacobian_mult[_qp]);
  return Jp(_direction, _direction) * _test[_i][_qp] * _phi[_j][_qp];
}

Real
LowerDimensionalWeakOutOfPlaneStress::computeQpOffDiagJacobian(unsigned int jvar)
{
  RealVectorValue n(-_q_point[_qp](0), -_q_point[_qp](1), 0);
  n /= n.norm();
  RankTwoTensor nn;
  nn.vectorOuterProduct(n, n);
  RankTwoTensor Jp = nn.initialContraction(_Jacobian_mult[_qp]);

  Real val = 0.0;

  // off-diagonal Jacobian with respect to a coupled displacement component
  if (_disp_coupled)
  {
    for (unsigned int coupled_direction = 0; coupled_direction < _ndisp; ++coupled_direction)
    {
      if (jvar == _disp_var[coupled_direction])
      {
        unsigned int coupled_direction_index = 0;
        switch (_direction)
        {
          case 0: // x
          {
            if (coupled_direction == 0)
              coupled_direction_index = 1;
            else
              coupled_direction_index = 2;
            break;
          }
          case 1: // y
          {
            if (coupled_direction == 0)
              coupled_direction_index = 0;
            else
              coupled_direction_index = 2;
            break;
          }
          default: // z
          {
            coupled_direction_index = coupled_direction;
            break;
          }
        }

        val = Jp(coupled_direction_index, coupled_direction_index) * _test[_i][_qp] *
              _grad_phi[_j][_qp](coupled_direction_index);
      }
    }
  }

  // off-diagonal Jacobian with respect to a coupled temperature variable
  if (_temp_coupled && jvar == _temp_var)
  {
    RankTwoTensor total_deigenstrain_dT;
    for (const auto deigenstrain_dT : _deigenstrain_dT)
      total_deigenstrain_dT += (*deigenstrain_dT)[_qp];

    Real sum = 0.0;
    for (unsigned int i = 0; i < 3; ++i)
      sum += Jp(i, i) * total_deigenstrain_dT(i, i);

    val = -sum * _test[_i][_qp] * _phi[_j][_qp];
  }

  return val;
}
