//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

/**
 * Self auxiliary value
 */
template <bool is_ad>
class FPIVariableValueAuxTempl : public AuxKernel
{
public:
  static InputParameters validParams();

  FPIVariableValueAuxTempl(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// coupeld variable to read values from
  const GenericVariableValue<is_ad> & _v;
};

typedef FPIVariableValueAuxTempl<false> FPIVariableValueAux;
typedef FPIVariableValueAuxTempl<true> ADFPIVariableValueAux;
