#include "CNHDegradedPK1Stress.h"

registerADMooseObject("raccoonApp", CNHDegradedPK1Stress);

defineADValidParams(
    CNHDegradedPK1Stress,
    ADComputeStressBase,
    params.addClassDescription("Compute PK1 stress from HLCG strain");
    params.addParam<MaterialPropertyName>(
        "elastic_energy_name",
        "E_el",
        "name of the material that holds the elastic energy normalized by mobility");
    params.addParam<MaterialPropertyName>("degradation_name",
                                          "g",
                                          "name of the material that holds the degradation");
    params.addParam<bool>(
        "history",
        true,
        "whether to use keep track of the maximum elastic energy to enforce irreversibility"););

template <ComputeStage compute_stage>
CNHDegradedPK1Stress<compute_stage>::CNHDegradedPK1Stress(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(adGetADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _F(adGetADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _g(adGetADMaterialProperty<Real>(_g_name)),
    _history(adGetParam<bool>("history")),
    _E_el_name(adGetParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_pos(adDeclareADProperty<Real>(_E_el_name)),
    _E_el_pos_old(adGetMaterialPropertyOld<Real>(_E_el_name))
{
}

template <ComputeStage compute_stage>
void
CNHDegradedPK1Stress<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  ADReal mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  ADReal K = lambda + 2.0 * mu / 3.0;

  ADRankTwoTensor C = _mechanical_strain[_qp];
  ADRankTwoTensor F = _F[_qp];
  ADReal J = F.det();
  ADReal lnJ = std::log(J);

  // Identity tensor
  ADRankTwoTensor I2;
  I2.addIa(1.0);

  // positive and negative PK2 stress
  ADRankTwoTensor S, S_pos, S_neg;

  // volumetric and deviatoric part of the PK2 stress
  ADReal dU_dJ = 0.5 * K * (J - 1.0 / J);
  ADRankTwoTensor S_vol = J * dU_dJ * C.inverse();
  ADRankTwoTensor S_dev = mu * std::pow(J, -2.0 / 3.0) * (I2 - C.trace() * C.inverse() / 3);

  // group volumetric and deviatoric parts into positive and negative classifications
  // the positive part is degraded and the negative part is not
  S_pos = S_dev;
  if (J >= 1.0)
    S_pos += S_vol;
  else
    S_neg += S_vol;
  S = _g[_qp] * S_pos + S_neg;

  // PK1 stress
  _stress[_qp] = F * S;

  // elastic driving energy
  ADReal E_el_vol = 0.5 * K * (0.5 * (J * J - 1.0) - lnJ);
  ADReal E_el_dev = 0.5 * mu * std::pow(J, -2.0 / 3.0) * (C.trace() - 3);
  ADReal E_el_pos = J >= 1.0 ? E_el_vol + E_el_dev : E_el_dev;

  // store the positive elastic energy so that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;

  // enforce irreversibility if history is set to true
  if (_history && E_el_pos < _E_el_pos_old[_qp])
    _E_el_pos[_qp] = _E_el_pos_old[_qp];
}
