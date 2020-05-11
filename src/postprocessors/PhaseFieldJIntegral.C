//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldJIntegral.h"

registerMooseObject("raccoonApp", PhaseFieldJIntegral);

InputParameters
PhaseFieldJIntegral::validParams()
{
  InputParameters params = SideIntegralPostprocessor::validParams();
  params.addClassDescription("Compute the J integral for a phase-field model of fracture");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addRequiredParam<MaterialPropertyName>("elastic_energy_name",
                                                "name of the elastic energy");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  return params;
}

PhaseFieldJIntegral::PhaseFieldJIntegral(const InputParameters & parameters)
  : SideIntegralPostprocessor(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "pk1_stress")),
    _E_elastic(getADMaterialProperty<Real>("elastic_energy_name")),
    _ndisp(coupledComponents("displacements")),
    _grad_disp_0(coupledGradient("displacements", 0)),
    _grad_disp_1(_ndisp >= 2 ? coupledGradient("displacements", 1) : _grad_zero),
    _grad_disp_2(_ndisp >= 3 ? coupledGradient("displacements", 2) : _grad_zero),
    _t(getParam<RealVectorValue>("J_direction"))
{
}

Real
PhaseFieldJIntegral::computeQpIntegral()
{
  RankTwoTensor H(_grad_disp_0[_qp], _grad_disp_1[_qp], _grad_disp_2[_qp]);
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  ADRankTwoTensor Sigma = _E_elastic[_qp] * I2 - H.transpose() * _stress[_qp];
  RealVectorValue n = _normals[_qp];
  ADReal value = _t * Sigma * n;
  return value.value();
}
