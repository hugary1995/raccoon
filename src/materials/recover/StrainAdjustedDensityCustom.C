//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StrainAdjustedDensityCustom.h"

registerMooseObject("SolidMechanicsApp", StrainAdjustedDensityCustom);
registerMooseObject("SolidMechanicsApp", ADStrainAdjustedDensityCustom);

template <bool is_ad>
InputParameters
StrainAdjustedDensityCustomTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();

  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system. If "
      "displacements are not coupled, a different material class such as GenericMaterialProperty "
      "or ParsedMaterial should be used.");

  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple material systems on the same block, "
                               "e.g. for multiple phases");
  params.addRequiredParam<MaterialPropertyName>("strain_free_density",
                                                "Material property for strain-free density");
  params.addClassDescription("Creates density material property");

  return params;
}

template <bool is_ad>
StrainAdjustedDensityCustomTempl<is_ad>::StrainAdjustedDensityCustomTempl(
    const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _strain_free_density(
        this->template getGenericMaterialProperty<Real, is_ad>("strain_free_density")),
    _density(declareGenericProperty<Real, is_ad>(_base_name + "density")),
    _F(getADMaterialProperty<RankTwoTensor>("deformation_gradient_NoFbar"))
{
  if (getParam<bool>("use_displaced_mesh"))
    paramError("use_displaced_mesh",
               "StrainAdjustedDensity needs to act on an undisplaced mesh. Use of a displaced mesh "
               "leads to "
               "incorrect gradient values");
}

template <bool is_ad>
void
StrainAdjustedDensityCustomTempl<is_ad>::initQpStatefulProperties()
{
  computeQpProperties();
}

template <bool is_ad>
void
StrainAdjustedDensityCustomTempl<is_ad>::computeQpProperties()
{
  _density[_qp] = _strain_free_density[_qp] / MetaPhysicL::raw_value(_F[_qp]).det();
}

template class StrainAdjustedDensityCustomTempl<false>;
template class StrainAdjustedDensityCustomTempl<true>;
