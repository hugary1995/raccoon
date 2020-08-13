//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

/**
 * Self auxiliary value
 */
template <bool is_ad>
class FPIMaterialPropertyAuxTempl : public AuxKernel
{
public:
  static InputParameters validParams();

  FPIMaterialPropertyAuxTempl(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// material property to project values from
  const GenericMaterialProperty<Real, is_ad> & _v;
};

typedef FPIMaterialPropertyAuxTempl<false> FPIMaterialPropertyAux;
typedef FPIMaterialPropertyAuxTempl<true> ADFPIMaterialPropertyAux;
