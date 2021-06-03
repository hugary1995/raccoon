//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeLowerDimensionalDeformationGradient.h"

#include "libmesh/string_to_enum.h"

registerADMooseObject("raccoonApp", ComputeLowerDimensionalDeformationGradient);

InputParameters
ComputeLowerDimensionalDeformationGradient::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the deformation gradient of a 2D element in the 3D space. Eigen "
      "deformation gradients are removed from the total deformation gradient. The F-bar approach "
      "can optionally be used to correct volumetric locking.");

  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<bool>(
      "volumetric_locking_correction", false, "Flag to correct volumetric locking");
  params.addParam<std::vector<MaterialPropertyName>>(
      "eigen_deformation_gradient_names", "List of eigen deformation gradients to be applied");

  params.addRequiredCoupledVar("out_of_plane_strain",
                               "Nonlinear variable to weakly enforce the plane stress condition");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeLowerDimensionalDeformationGradient::ComputeLowerDimensionalDeformationGradient(
    const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _Q(declareProperty<RankTwoTensor>(
        prependBaseName("lower_dimensional_coordinate_transformation"))),
    _ndisp(coupledComponents("displacements")),
    _disp(adCoupledValues("displacements")),
    _grad_disp(adCoupledGradients("displacements")),
    _volumetric_locking_correction(getParam<bool>("volumetric_locking_correction") &&
                                   !this->isBoundaryMaterial()),
    _out_of_plane_strain(
        isParamValid("out_of_plane_strain") ? &adCoupledValue("out_of_plane_strain") : nullptr),
    _current_elem_volume(_assembly.elemVolume()),
    _F(declareADProperty<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _Fm(declareADProperty<RankTwoTensor>(prependBaseName("mechanical_deformation_gradient"))),
    _Fg_names(prependBaseName(
        getParam<std::vector<MaterialPropertyName>>("eigen_deformation_gradient_names"))),
    _Fgs(_Fg_names.size())
{
  for (unsigned int i = 0; i < _Fgs.size(); ++i)
    _Fgs[i] = &getADMaterialProperty<RankTwoTensor>(_Fg_names[i]);

  if (getParam<bool>("use_displaced_mesh"))
    paramError("use_displaced_mesh", "The strain calculator needs to run on the undisplaced mesh.");
}

void
ComputeLowerDimensionalDeformationGradient::initialSetup()
{
  displacementIntegrityCheck();
}

void
ComputeLowerDimensionalDeformationGradient::displacementIntegrityCheck()
{
  if (_ndisp != 3)
    paramError("displacements",
               name() + " must be used in 3D, and 3 displacement variables must be provided.");
}

void
ComputeLowerDimensionalDeformationGradient::initQpStatefulProperties()
{
  _F[_qp].setToIdentity();
  _Fm[_qp].setToIdentity();
}

ADReal
ComputeLowerDimensionalDeformationGradient::computeQpOutOfPlaneGradDisp()
{
  return std::exp((*_out_of_plane_strain)[_qp]) - 1;
}

RealVectorValue
ComputeLowerDimensionalDeformationGradient::getNormal() const
{
  FEType fe_type(Utility::string_to_enum<Order>("First"),
                 Utility::string_to_enum<FEFamily>("LAGRANGE"));
  auto & fe = _assembly.getFE(fe_type, 2);
  const auto & dxyzdxi = fe->get_fe_map().get_dxyzdxi();
  const auto & dxyzdeta = fe->get_fe_map().get_dxyzdeta();
  RealVectorValue normal = dxyzdxi[_qp].cross(dxyzdeta[_qp]);
  normal /= normal.norm();
  return normal;
}

void
ComputeLowerDimensionalDeformationGradient::computeProperties()
{
  ADRankTwoTensor ave_F;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
    // To apply the out-of-plane strain, we follow these steps:

    // Step 1: Compute the coordinate transformation matrix Q such that the z-axis aligns with
    // element normal. Then apply the transformation to matrix A.
    RealVectorValue n = getNormal();
    Real cost = n(2);
    Real sint = std::sqrt(1 - cost * cost);
    RankTwoTensor nxn, sinn;
    nxn.vectorOuterProduct(n, n);
    sinn.fillFromInputVector(
        {0, -n(2) * sint, n(1) * sint, n(2) * sint, 0, -n(0) * sint, -n(1) * sint, n(0) * sint, 0});
    _Q[_qp].setToIdentity();
    _Q[_qp] *= cost;
    _Q[_qp] += nxn * (1 - cost) + sinn;
    ADRankTwoTensor Ap = _Q[_qp] * A * _Q[_qp].transpose();

    // Step 2: Fill in the out-of-plane strain, and zero out the shear components.
    Ap(2, 2) = computeQpOutOfPlaneGradDisp();
    Ap(2, 0) = Ap(2, 1) = Ap(0, 2) = Ap(1, 2) = 0;

    // Step 3: Transform the coordinate back to the cartesion coordinates.
    A = _Q[_qp].transpose() * Ap * _Q[_qp];
    _F[_qp] = A;
    _F[_qp].addIa(1.0);

    if (_volumetric_locking_correction)
      ave_F += _F[_qp] * _JxW[_qp] * _coord[_qp];
  }

  if (_volumetric_locking_correction)
    ave_F /= _current_elem_volume;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    if (_volumetric_locking_correction)
      _F[_qp] *= std::cbrt(ave_F.det() / _F[_qp].det());

    // Remove the eigen deformation gradient
    ADRankTwoTensor Fg(ADRankTwoTensor::initIdentity);
    for (auto Fgi : _Fgs)
      Fg *= (*Fgi)[_qp];
    _Fm[_qp] = _F[_qp] * Fg.inverse();
  }
}
