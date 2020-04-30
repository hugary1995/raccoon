//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

/**
 * Self auxiliary value
 */
class FPIMaterialPropertyAux : public AuxKernel
{
public:
  static InputParameters validParams();

  FPIMaterialPropertyAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// material property to project values from
  const ADMaterialProperty<Real> & _v;
};
