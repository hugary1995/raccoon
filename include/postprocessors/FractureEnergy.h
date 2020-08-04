//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

template <bool is_ad>
class FractureEnergyTempl : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  FractureEnergyTempl(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const GenericMaterialProperty<Real, is_ad> & _kappa;
  const GenericMaterialProperty<Real, is_ad> & _M;
  const GenericVariableGradient<is_ad> & _grad_d;
  const GenericMaterialProperty<Real, is_ad> & _w;
};

typedef FractureEnergyTempl<false> FractureEnergy;
typedef FractureEnergyTempl<true> ADFractureEnergy;
