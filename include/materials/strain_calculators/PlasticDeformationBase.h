//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"
#include "BaseNameInterface.h"
#include "PlasticEnergyDensityBase.h"
#include "SDElasticEnergyDensityBase.h"
#include "LDElasticEnergyDensityBase.h"

template <class T>
class PlasticDeformationBase : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  PlasticDeformationBase(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual void initQpStatefulProperties() override;

  /// The elasticity constitutive model
  T * _elasticity_model;

  /// The plastic hardening model
  PlasticEnergyDensityBase * _hardening_model;

  /// The (scalar) effective plastic strain
  ADMaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;

  /// The plastic flow direction
  ADMaterialProperty<RankTwoTensor> & _Np;
};

typedef PlasticDeformationBase<SDElasticEnergyDensityBase> SDPlasticDeformationBase;
typedef PlasticDeformationBase<LDElasticEnergyDensityBase> LDPlasticDeformationBase;
