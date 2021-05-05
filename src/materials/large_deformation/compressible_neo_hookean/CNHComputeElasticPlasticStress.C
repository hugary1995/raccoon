//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHComputeElasticPlasticStress.h"
#include "ElasticityTensorTools.h"

registerMooseObject("raccoonApp", CNHComputeElasticPlasticStress);

InputParameters
CNHComputeElasticPlasticStress::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "computes elastic and plastic stress for a compressible Neo-Hookean material");

  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.suppressParameter<bool>("use_displaced_mesh");

  params.addParam<MaterialPropertyName>(
      "degradation_name", "g", "name of the degradation material. Use degradation_mat instead.");
  params.addParam<MaterialPropertyName>(
      "plastic_degradation_name",
      "gp",
      "name of the plastic degradation material. Use plastic_degradation_mat instead.");

  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the material that holds the elastic energy");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_name", "E_pl", "name of the material for plastic work");

  params.addParam<bool>(
      "enforce_isochoricity", true, "whether to enforce the isochoricity constraint");

  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("hardening_modulus", "slope of the hardening curve");
  return params;
}

CNHComputeElasticPlasticStress::CNHComputeElasticPlasticStress(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),

    _g(getADMaterialProperty<Real>("degradation_name")),
    _gp(getADMaterialProperty<Real>("plastic_degradation_name")),

    _E_el_name(_base_name + getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_active(declareADProperty<Real>(_E_el_name + "_active")),
    _E_pl_name(_base_name + getParam<MaterialPropertyName>("plastic_energy_name")),
    _E_pl(declareADProperty<Real>(_E_pl_name)),

    _F(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),

    _be_bar(declareADProperty<RankTwoTensor>(_base_name +
                                             "volume_perserving_left_cauchy_green_strain")),
    _be_bar_old(getMaterialPropertyOldByName<RankTwoTensor>(
        _base_name + "volume_perserving_left_cauchy_green_strain")),

    _ep(declareADProperty<Real>(_base_name + "effective_plastic_strain")),
    _ep_old(getMaterialPropertyOldByName<Real>(_base_name + "effective_plastic_strain")),

    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _s(declareADProperty<RankTwoTensor>(_base_name + "deviatoric_kirchhoff_stress")),
    _s_norm(declareADProperty<Real>(_base_name + "deviatoric_kirchhoff_stress_norm")),
    _N(declareADProperty<RankTwoTensor>(_base_name + "flow_direction")),

    _isochoricity(getParam<bool>("enforce_isochoricity")),

    _yield_stress(getParam<Real>("yield_stress")),
    _hardening_modulus(getParam<Real>("hardening_modulus"))
{
}

void
CNHComputeElasticPlasticStress::initQpStatefulProperties()
{
  _be_bar[_qp].setToIdentity();
  _ep[_qp] = 0;
}

void
CNHComputeElasticPlasticStress::computeQpProperties()
{
  const ADReal G = ElasticityTensorTools::getIsotropicShearModulus(_elasticity_tensor[_qp]);
  const ADReal K = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp]);
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);

  ADRankTwoTensor f = _F[_qp] * _F_old[_qp].inverse();
  ADRankTwoTensor f_bar = f / std::cbrt(f.det());

  // Compute the flow direction
  ADReal delta_ep = 0;
  _be_bar[_qp] = f_bar * _be_bar_old[_qp] * (f_bar.transpose());
  _s[_qp] = _g[_qp] * G * _be_bar[_qp].deviatoric();
  _s_norm[_qp] = std::sqrt(_s[_qp].doubleContraction(_s[_qp]));
  _N[_qp] = std::sqrt(1.5) * _s[_qp] / _s_norm[_qp];

  // Return mapping
  ADReal phi = _s_norm[_qp] - std::sqrt(2.0 / 3.0) * _gp[_qp] * plasticEnergy(_ep_old[_qp], 1);

  if (phi <= 0.0)
    _ep[_qp] = _ep_old[_qp];
  else
  {
    ADReal phi0 = phi;
    ADReal jacob, step;
    int iter = 0;
    while (std::abs(phi) > 1E-08 && std::abs(phi) > 1e-06 * std::abs(phi0))
    {
      jacob = -std::sqrt(3.0 / 2.0) * _g[_qp] * G * _be_bar[_qp].trace() -
              std::sqrt(2.0 / 3.0) * _gp[_qp] * plasticEnergy(_ep_old[_qp] + delta_ep, 2);
      step = -phi / jacob;
      delta_ep += step;
      phi = _s_norm[_qp] - std::sqrt(3.0 / 2.0) * _g[_qp] * G * delta_ep * _be_bar[_qp].trace() -
            std::sqrt(2.0 / 3.0) * _gp[_qp] * plasticEnergy(_ep_old[_qp] + delta_ep, 1);
      iter++;
      if (iter > 50)
      {
        std::cout << "initial yield function = " << phi0 << std::endl;
        std::cout << "        yield function = " << phi << std::endl;
        mooseDoOnce(mooseWarning("Too many iterations in return mapping."));
        break;
      }
    }

    _s[_qp] = _s[_qp] - _g[_qp] * G * delta_ep * _be_bar[_qp].trace() * _N[_qp];
    _ep[_qp] = _ep_old[_qp] + delta_ep;
  }

  // Compute Cauchy stress
  ADReal J = _F[_qp].det();
  ADReal p = 0.5 * K * (J * J - 1);
  ADRankTwoTensor tau = (J >= 1.0 ? _g[_qp] : 1) * p * I2 + _s[_qp];
  _cauchy_stress[_qp] = tau / J;

  // Update intermediate configuration
  if (_isochoricity)
  {
    ADRankTwoTensor be_bar_dev = _s[_qp] / _g[_qp] / G;
    Real a = raw_value(be_bar_dev(0, 0));
    Real b = raw_value(be_bar_dev(1, 1));
    Real c = raw_value(be_bar_dev(2, 2));
    Real d = raw_value(be_bar_dev(1, 2));
    Real e = raw_value(be_bar_dev(0, 2));
    Real h = raw_value(be_bar_dev(0, 1));

    Real A = a + b + c;
    Real B = a * b + a * c + b * c - d * d - e * e - h * h;
    Real C = a * b * c + 2.0 * d * e * h - a * d * d - b * e * e - c * h * h - 1.0;

    Real D = std::cbrt(-2 * A * A * A +
                       3 * std::sqrt(3) *
                           std::sqrt(4 * A * A * A * C - A * A * B * B - 18 * A * B * C +
                                     4 * B * B * B + 27 * C * C) +
                       9 * A * B - 27 * C);

    Real Ie_bar = D / 3 / std::cbrt(2) - std::cbrt(2) * (3 * B - A * A) / 3 / D - A / 3;

    _be_bar[_qp] = be_bar_dev + Ie_bar * I2;
  }

  // Compute fracture driving energy
  ADReal U = 0.5 * K * (0.5 * (J * J - 1) - std::log(J));
  ADReal W = 0.5 * G * (_be_bar[_qp].trace() - 3.0);
  _E_el_active[_qp] = J >= 1.0 ? U + W : W;
  _E_pl[_qp] = plasticEnergy(_ep[_qp]);
}

ADReal
CNHComputeElasticPlasticStress::plasticEnergy(const ADReal & ep,
                                              const unsigned int derivative_order) const
{
  ADReal value = 0;

  if (derivative_order == 0)
    value = _yield_stress * ep + 0.5 * _hardening_modulus * ep * ep;

  if (derivative_order == 1)
    value = _yield_stress + _hardening_modulus * ep;

  if (derivative_order == 2)
    value = _hardening_modulus;

  return value;
}
