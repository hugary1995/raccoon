//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticDeformationBase.h"

template <class T>
InputParameters
PlasticDeformationBase<T>::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addRequiredParam<MaterialName>("elasticity_model", "Name of the elasticity model");
  params.addRequiredParam<MaterialName>("hardening_model", "Name of the plastic hardening model");
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

template <class T>
PlasticDeformationBase<T>::PlasticDeformationBase(const InputParameters & params)
  : Material(params),
    BaseNameInterface(params),
    _elasticity_model(nullptr),
    _hardening_model(nullptr),
    _ep(this->template declareADProperty<Real>(prependBaseName("effective_plastic_strain"))),
    _ep_old(this->template getMaterialPropertyOldByName<Real>(
        prependBaseName("effective_plastic_strain"))),
    _Np(this->template declareADProperty<RankTwoTensor>(prependBaseName("plastic_flow_direction")))
{
}

template <class T>
void
PlasticDeformationBase<T>::initialSetup()
{
  _elasticity_model =
      dynamic_cast<T *>(&getMaterialByName(getParam<MaterialName>("elasticity_model"), true));
  if (!_elasticity_model)
    mooseError("Elasticity model ",
               _elasticity_model->name(),
               " of type ",
               _elasticity_model->type(),
               " is not compatible with ",
               type());

  _hardening_model = dynamic_cast<PlasticEnergyDensityBase *>(
      &getMaterialByName(getParam<MaterialName>("hardening_model"), true));
  if (!_hardening_model)
    mooseError("Plastic hardening model ",
               _hardening_model->name(),
               " of type ",
               _hardening_model->type(),
               " is not compatible with ",
               type());
}

template <class T>
void
PlasticDeformationBase<T>::initQpStatefulProperties()
{
  _ep[_qp] = 0;
}

template class PlasticDeformationBase<SDElasticEnergyDensityBase>;
template class PlasticDeformationBase<LDElasticEnergyDensityBase>;
