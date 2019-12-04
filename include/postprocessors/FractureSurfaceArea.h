//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "FractureEnergy.h"

class FractureSurfaceArea;

template <>
InputParameters validParams<FractureSurfaceArea>();

class FractureSurfaceArea : public FractureEnergy
{
public:
  FractureSurfaceArea(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// energy release rate
  const MaterialProperty<Real> & _Gc;
};
