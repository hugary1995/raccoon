//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MaterialPropertyUserObject.h"

template <bool is_ad>
class FPIMaterialPropertyUserObjectTempl : public MaterialPropertyUserObjectTempl<is_ad>
{
public:
  static InputParameters validParams();

  FPIMaterialPropertyUserObjectTempl(const InputParameters & parameters);
};

typedef FPIMaterialPropertyUserObjectTempl<false> FPIMaterialPropertyUserObject;
typedef FPIMaterialPropertyUserObjectTempl<true> ADFPIMaterialPropertyUserObject;
