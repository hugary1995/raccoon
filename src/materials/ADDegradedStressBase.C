//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADDegradedStressBase.h"

defineADValidParams(
    ADDegradedStressBase,
    ADComputeStressBase,
    params.addClassDescription("Base class for computing damage degraded stress");
    params.addRequiredCoupledVar("d", "damage variable");
    params.addParam<Real>("d_crit",
                          2.0,
                          "enforce the traction free boundary condition when d > d_crit");
    params.addParam<MaterialPropertyName>("elastic_energy_name",
                                          "E_el",
                                          "name of the material that holds the elastic energy");
    params.addParam<MaterialPropertyName>(
        "fracture_energy_barrier_name",
        "b",
        "name of the material that holds the fracture energy barrier");
    params.addParam<MaterialPropertyName>("degradation_name",
                                          "g",
                                          "name of the material that holds the degradation");
    params.addParam<bool>("history",
                          true,
                          "whether irreversibility is enforced using a history approach"););

template <ComputeStage compute_stage>
ADDegradedStressBase<compute_stage>::ADDegradedStressBase(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(adGetMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _d(adCoupledValue("d")),
    _grad_d(adCoupledGradient("d")),
    _d_crit(adGetParam<Real>("d_crit")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _g(adGetADMaterialProperty<Real>(_g_name)),
    _history(adGetParam<bool>("history")),
    _E_el_name(adGetParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_pos(adDeclareADProperty<Real>(_E_el_name)),
    _E_el_pos_old(_history ? &adGetMaterialPropertyOld<Real>(_E_el_name) : NULL),
    _b_name(adGetParam<MaterialPropertyName>("fracture_energy_barrier_name")),
    _b(adGetADMaterialProperty<Real>(_b_name))
{
}

template <ComputeStage compute_stage>
void
ADDegradedStressBase<compute_stage>::computeQpProperties()
{
  if (_d[_qp] < _d_crit)
    computeQpStress();
  else
    computeQpTractionFreeStress();
}

template <ComputeStage compute_stage>
void
ADDegradedStressBase<compute_stage>::initQpStatefulProperties()
{
  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    _E_el_pos[_qp] = _b[_qp];
}

template <ComputeStage compute_stage>
ADReal
ADDegradedStressBase<compute_stage>::computeQpHistory(const ADReal & E_el_pos)
{
  ADReal history_maximum = std::max(E_el_pos, (*_E_el_pos_old)[_qp]);
  history_maximum = std::max(history_maximum, _b[_qp]);
  return history_maximum;
}

template <ComputeStage compute_stage>
void
ADDegradedStressBase<compute_stage>::computeQpTractionFreeStress()
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
  ADReal E_el_pos = 0.5 * stress_active.doubleContraction(strain);

  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    E_el_pos = computeQpHistory(E_el_pos);

  // store the positive elastic energy
  // note that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;
}

template <ComputeStage compute_stage>
ADReal
ADDegradedStressBase<compute_stage>::Macaulay(ADReal x)
{
  return 0.5 * (x + std::abs(x));
}

template <ComputeStage compute_stage>
ADReal
ADDegradedStressBase<compute_stage>::dMacaulay(ADReal x)
{
  ADReal val = x > 0.0 ? 1.0 : 0.0;
  return val;
}

adBaseClass(ADDegradedStressBase);
