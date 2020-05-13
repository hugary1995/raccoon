//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADDegradedElasticStressBase.h"

InputParameters
ADDegradedElasticStressBase::validParams()
{
  InputParameters params = ADComputeStressBase::validParams();
  params.addClassDescription("Base class for computing damage degraded stress");
  params.addRequiredCoupledVar("d", "damage variable");
  params.addParam<Real>(
      "d_crit", 2.0, "enforce the traction free boundary condition when d > d_crit");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the material that holds the elastic energy");
  params.addParam<MaterialPropertyName>(
      "degradation_name", "g", "name of the material that holds the degradation");
  return params;
}

ADDegradedElasticStressBase::ADDegradedElasticStressBase(const InputParameters & parameters)
  : ADComputeStressBase(parameters),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _d(adCoupledValue("d")),
    _grad_d(adCoupledGradient("d")),
    _d_crit(getParam<Real>("d_crit")),
    _g_name(getParam<MaterialPropertyName>("degradation_name")),
    _g(getADMaterialProperty<Real>(_g_name)),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_active(declareADProperty<Real>(_E_el_name + "_active"))
{
}

void
ADDegradedElasticStressBase::computeQpProperties()
{
  if (_d[_qp] < _d_crit)
    computeQpStress();
  else
    computeQpTractionFreeStress();
}

void
ADDegradedElasticStressBase::computeQpTractionFreeStress()
{
  const Real eps = libMesh::TOLERANCE;

  ADRankTwoTensor strain = _mechanical_strain[_qp];
  ADRankTwoTensor stress = _elasticity_tensor[_qp] * strain;

  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
    n = _grad_d[_qp] / _grad_d[_qp].norm();
  ADReal s = (stress * n) * n;
  ADReal s_pos = Macaulay(s);
  ADReal s_neg = s - s_pos;

  ADRankTwoTensor stress_normal_pos, stress_normal_neg;
  stress_normal_pos.vectorOuterProduct(n, n);
  stress_normal_pos *= s_pos;
  stress_normal_pos.vectorOuterProduct(n, n);
  stress_normal_pos *= s_neg;

  ADRankTwoTensor stress_tangential = stress - stress_normal_pos - stress_normal_neg;
  ADRankTwoTensor stress_active = stress_normal_pos + stress_tangential;
  ADRankTwoTensor stress_inactive = stress_normal_neg;

  // degraded stress
  _stress[_qp] = _g[_qp] * stress_active + stress_inactive;

  // elastic driving energy
  _E_el_active[_qp] = 0.5 * stress_active.doubleContraction(strain);
}

ADReal
ADDegradedElasticStressBase::Macaulay(ADReal x)
{
  return 0.5 * (x + std::abs(x));
}

std::vector<ADReal>
ADDegradedElasticStressBase::Macaulay(std::vector<ADReal> v)
{
  std::vector<ADReal> m(v.size());
  for (unsigned int i = 0; i < v.size(); i++)
    m[i] = Macaulay(v[i]);
  return m;
}
