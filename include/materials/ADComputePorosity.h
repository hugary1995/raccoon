//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class ADComputePorosity : public Material
{
public:
  static InputParameters validParams();

  ADComputePorosity(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  const ADVariableValue & _porepressure;
  const ADMaterialProperty<RankTwoTensor> & _strain;

  ADMaterialProperty<Real> & _porosity;
  const MaterialProperty<Real> & _porosity_old;
  const Real _initial_porosity;

  const ADMaterialProperty<Real> & _alpha;
  const ADMaterialProperty<Real> & _M;
};
