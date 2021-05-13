//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticHardeningModel.h"

class PowerLawHardening : public PlasticHardeningModel
{
public:
  static InputParameters validParams();

  PowerLawHardening(const InputParameters & parameters);

  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) override;

protected:
  // @{ The plastic energy parameters
  const ADMaterialProperty<Real> & _sigma_y;
  const ADMaterialProperty<Real> & _n;
  const ADMaterialProperty<Real> & _ep0;
  // @}
};
