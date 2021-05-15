//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADNewtonianViscosity.h"

registerMooseObject("raccoonApp", ADNewtonianViscosity);

InputParameters
ADNewtonianViscosity::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.addClassDescription("Newtonian viscosity for large deformation.");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  params.addParam<MaterialPropertyName>("viscosity", "viscosity", "Name of the viscosity material");
  params.addParam<std::string>("base_name", "Material property base name");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

ADNewtonianViscosity::ADNewtonianViscosity(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _F(getADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _component(getParam<unsigned int>("component")),
    _eta(getADMaterialProperty<Real>("viscosity"))
{
}

void
ADNewtonianViscosity::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_XYZ)
    mooseError(
        "The coordinate system in the Problem block must be set to XYZ for cartesian geometries.");
}

ADRealVectorValue
ADNewtonianViscosity::precomputeQpResidual()
{
  ADRankTwoTensor F_dot = (_F[_qp] - _F_old[_qp]) / _dt;
  ADRankTwoTensor F_inv = _F[_qp].inverse();
  ADRankTwoTensor D = (F_dot * F_inv + F_inv.transpose() * F_dot.transpose()) / 2;

  return _eta[_qp] * D.row(_component);
}
