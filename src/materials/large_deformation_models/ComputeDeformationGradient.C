//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeDeformationGradient.h"

registerADMooseObject("raccoonApp", ComputeDeformationGradient);

InputParameters
ComputeDeformationGradient::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the deformation gradient. Eigen deformation gradients are removed "
      "from the total deformation gradient. The F-bar approach can optionally be used to correct "
      "volumetric locking.");

  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<bool>(
      "volumetric_locking_correction", false, "Flag to correct volumetric locking");
  params.addParam<std::vector<MaterialPropertyName>>(
      "eigen_deformation_gradient_names", "List of eigen deformation gradients to be applied");

  params.addCoupledVar("out_of_plane_strain",
                       "Nonlinear variable to weakly enforce the plane stress condition");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeDeformationGradient::ComputeDeformationGradient(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _coord_sys(_assembly.coordSystem()),
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
ComputeDeformationGradient::initialSetup()
{
  displacementIntegrityCheck();

  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
  {
    _disp.push_back(&_ad_zero);
    _grad_disp.push_back(&_ad_grad_zero);
  }
}

void
ComputeDeformationGradient::displacementIntegrityCheck()
{
  // Checking for consistency between mesh size and length of the provided displacements vector
  if (_ndisp != _mesh.dimension())
    paramError(
        "displacements",
        "The number of variables supplied in 'displacements' must match the mesh dimension.");

  // Don't use F-bar in 1D
  if (_ndisp == 1 && _volumetric_locking_correction)
    paramError("volumetric_locking_correction", "has to be set to false for 1-D problems.");

  // Check for RZ
  if (getBlockCoordSystem() == Moose::COORD_RZ && _ndisp != 2)
    paramError("displacements",
               "There must be two displacement variables provided, one in r-direction another in "
               "z-direction");

  // If out of plane strain is provided
  if (_out_of_plane_strain)
  {
    if (_ndisp != 2)
      mooseError(
          "Out of plane strain only makes sens in 2D. But you provided ", _ndisp, " displacements");
    if (getBlockCoordSystem() != Moose::COORD_XYZ)
      mooseError("Out of plane strain only supports cartesian coordinate system.");
  }
}

void
ComputeDeformationGradient::initQpStatefulProperties()
{
  _F[_qp].setToIdentity();
  _Fm[_qp].setToIdentity();
}

ADReal
ComputeDeformationGradient::computeQpOutOfPlaneGradDisp()
{
  if (_out_of_plane_strain)
    return std::exp((*_out_of_plane_strain)[_qp]) - 1;
  else if (!MooseUtils::absoluteFuzzyEqual(_q_point[_qp](0), 0.0))
    return (*_disp[0])[_qp] / _q_point[_qp](0);
  else
    return 0.0;
}

void
ComputeDeformationGradient::computeProperties()
{
  ADRankTwoTensor ave_F;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
    if (_coord_sys == Moose::COORD_RZ || _out_of_plane_strain)
      A(2, 2) = computeQpOutOfPlaneGradDisp();
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
