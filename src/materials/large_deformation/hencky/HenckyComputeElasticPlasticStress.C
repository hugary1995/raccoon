//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "HenckyComputeElasticPlasticStress.h"
#include "ElasticityTensorTools.h"

registerMooseObject("raccoonApp", HenckyComputeElasticPlasticStress);

InputParameters
HenckyComputeElasticPlasticStress::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("computes elastic and plastic stress for a Hencky model");

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

  params.addRequiredParam<Real>("yield_stress", "The yield stress");
  params.addRequiredParam<Real>("exponent", "The exponent n in the power law hardening");
  params.addRequiredParam<Real>("reference_plastic_strain",
                                "The $\\epsilon_0$ parameter in the power law hardening");
  return params;
}

HenckyComputeElasticPlasticStress::HenckyComputeElasticPlasticStress(
    const InputParameters & parameters)
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
    _Fp(declareADProperty<RankTwoTensor>(_base_name + "plastic_deformation_gradient")),
    _Fp_old(
        getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "plastic_deformation_gradient")),

    _ep(declareADProperty<Real>(_base_name + "effective_plastic_strain")),
    _ep_old(getMaterialPropertyOldByName<Real>(_base_name + "effective_plastic_strain")),

    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _s(declareADProperty<RankTwoTensor>(_base_name + "deviatoric_kirchhoff_stress")),
    _s_norm(declareADProperty<Real>(_base_name + "deviatoric_kirchhoff_stress_norm")),
    _N(declareADProperty<RankTwoTensor>(_base_name + "flow_direction")),

    _yield_stress(getParam<Real>("yield_stress")),
    _n(getParam<Real>("exponent")),
    _ep0(getParam<Real>("reference_plastic_strain"))
{
}

void
HenckyComputeElasticPlasticStress::initQpStatefulProperties()
{
  _Fp[_qp].setToIdentity();
  _ep[_qp] = 0;
}

ADRankTwoTensor
HenckyComputeElasticPlasticStress::log(const ADRankTwoTensor & A) const
{
  std::vector<ADReal> d;
  ADRankTwoTensor V, D;
  A.symmetricEigenvaluesEigenvectors(d, V);
  for (unsigned int i = 0; i < 3; i++)
    d[i] = std::log(d[i]);
  D.fillFromInputVector(d);
  return V * D * V.transpose();
}

ADRankTwoTensor
HenckyComputeElasticPlasticStress::exp(const ADRankTwoTensor & A) const
{
  std::vector<ADReal> d;
  ADRankTwoTensor V, D;
  A.symmetricEigenvaluesEigenvectors(d, V);
  for (unsigned int i = 0; i < 3; i++)
    d[i] = std::exp(d[i]);
  D.fillFromInputVector(d);
  return V * D * V.transpose();
}

void
HenckyComputeElasticPlasticStress::computeQpProperties()
{
  const ADReal G = ElasticityTensorTools::getIsotropicShearModulus(_elasticity_tensor[_qp]);
  const ADReal K = ElasticityTensorTools::getIsotropicBulkModulus(_elasticity_tensor[_qp]);
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);

  // Compute the flow direction
  ADReal delta_ep = 0;
  ADRankTwoTensor Fe = _F[_qp] * _Fp_old[_qp].inverse(); // Could be optimized
  ADRankTwoTensor Ce = Fe.transpose() * Fe;
  ADRankTwoTensor ee = 0.5 * log(Ce);
  _s[_qp] = _g[_qp] * 2. * G * ee.deviatoric();
  _s_norm[_qp] = _s[_qp].doubleContraction(_s[_qp]);
  if (MooseUtils::absoluteFuzzyEqual(_s_norm[_qp], 0))
    _s_norm[_qp].value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  _s_norm[_qp] = std::sqrt(1.5 * _s_norm[_qp]);
  _N[_qp] = 1.5 * _s[_qp] / _s_norm[_qp];

  // Return mapping
  ADReal phi = _s_norm[_qp] - _gp[_qp] * plasticEnergy(_ep_old[_qp], 1);
  if (phi <= 0.0)
    _ep[_qp] = _ep_old[_qp];
  else
  {
    ADReal phi0 = phi;
    ADReal jacob, step;
    int iter = 0;
    while (std::abs(phi) > 1E-08 && std::abs(phi) > 1e-06 * std::abs(phi0))
    {
      jacob = -3. * _g[_qp] * G - _gp[_qp] * plasticEnergy(_ep_old[_qp] + delta_ep, 2);
      step = -phi / jacob;
      delta_ep += step;
      phi = _s_norm[_qp] - 3. * _g[_qp] * G * delta_ep -
            _gp[_qp] * plasticEnergy(_ep_old[_qp] + delta_ep, 1);
      iter++;
      if (iter > 50)
      {
        std::cout << "initial yield function = " << phi0 << std::endl;
        std::cout << "        yield function = " << phi << std::endl;
        mooseDoOnce(mooseWarning("Too many iterations in return mapping."));
        break;
      }
    }

    _s[_qp] -= _g[_qp] * 2. * G * delta_ep * _N[_qp];
    _ep[_qp] = _ep_old[_qp] + delta_ep;
  }

  // Compute Cauchy stress
  ADRankTwoTensor mandel_stress = _s[_qp] + _g[_qp] * K * ee.trace() * I2;
  _cauchy_stress[_qp] = Fe.inverse().transpose() * mandel_stress * Fe.transpose() / _F[_qp].det();

  // Update Fp
  _Fp[_qp] = exp(delta_ep * _N[_qp]) * _Fp_old[_qp];

  // Compute fracture driving energy
  ADReal U = 0.5 * K * ee.trace() * ee.trace();
  ADRankTwoTensor eedev = _s[_qp] / (_g[_qp] * 2. * G);
  ADReal W = G * eedev.doubleContraction(eedev);
  _E_el_active[_qp] = U + W;
  _E_pl[_qp] = plasticEnergy(_ep[_qp]);
}

ADReal
HenckyComputeElasticPlasticStress::plasticEnergy(const ADReal & ep,
                                                 const unsigned int derivative_order) const
{
  ADReal value = 0;

  if (derivative_order == 0)
    value = _n * _yield_stress * _ep0 / (_n + 1) * (std::pow(1 + ep / _ep0, 1 / _n + 1) - 1);

  if (derivative_order == 1)
    value = _yield_stress * std::pow(1 + ep / _ep0, 1 / _n);

  if (derivative_order == 2)
    value = _yield_stress * std::pow(1 + ep / _ep0, 1 / _n - 1) / _n / _ep0;

  return value;
}
