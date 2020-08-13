//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

template <bool is_ad>
class KineticEnergyTempl : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  KineticEnergyTempl(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const GenericMaterialProperty<Real, is_ad> & _rho;
  const unsigned _ndisp;
  std::vector<const VariableValue *> _vel_var;
};

typedef KineticEnergyTempl<false> KineticEnergy;
typedef KineticEnergyTempl<true> ADKineticEnergy;
