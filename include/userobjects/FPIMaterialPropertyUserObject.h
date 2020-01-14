//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MaterialPropertyUserObject.h"

// Forward Declarations
class FPIMaterialPropertyUserObject;

template <>
InputParameters validParams<FPIMaterialPropertyUserObject>();

class FPIMaterialPropertyUserObject : public MaterialPropertyUserObject
{
public:
  static InputParameters validParams();

  FPIMaterialPropertyUserObject(const InputParameters & parameters);
};
