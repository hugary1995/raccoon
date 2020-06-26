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
      "degradation_name", "g", "name of the degradation material. Use degradation_mat instead.");
  params.addParam<MaterialPropertyName>("degradation_mat",
                                        "name of the material that holds the degradation");
  params.addParam<UserObjectName>("degradation_uo",
                                  "name of the userobject that holds the degradation");
  return params;
}

ADDegradedElasticStressBase::ADDegradedElasticStressBase(const InputParameters & parameters)
  : ADComputeStressBase(parameters),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _d(adCoupledValue("d")),
    _grad_d(adCoupledGradient("d")),
    _d_crit(getParam<Real>("d_crit")),
    _g_mat(isParamValid("degradation_mat") ? &getADMaterialProperty<Real>("degradation_mat")
                                           : &getADMaterialProperty<Real>("degradation_name")),
    _g_uo(isParamValid("degradation_uo")
              ? &getUserObject<ADMaterialPropertyUserObject>("degradation_uo")
              : nullptr),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_active(declareADProperty<Real>(_E_el_name + "_active"))
{
  if (parameters.isParamSetByUser("degradation_name"))
    mooseDeprecated("degradation_name is deprecated in favor of degradation_mat.");
  if (!_g_uo && parameters.isParamSetByAddParam("degradation_name"))
    mooseDeprecated("degradation_name is deprecated in favor of degradation_mat.");

  bool provided_by_mat = _g_mat;
  bool provided_by_uo = _g_uo;

  /// degradation should be provided
  if (!provided_by_mat && !provided_by_uo)
    mooseError("no degradation provided.");

  /// degradation should not be multiply defined
  if ((provided_by_mat ? 1 : 0) + (provided_by_uo ? 1 : 0) > 1)
    mooseError("degradation multiply defined.");
}

ADReal
ADDegradedElasticStressBase::g()
{
  if (_g_mat)
    return (*_g_mat)[_qp];
  else if (_g_uo)
    return _g_uo->getData(_current_elem, _qp);
  else
    mooseError("Internal Error");

  return 0;
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
  _stress[_qp] = g() * stress_active + stress_inactive;

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
