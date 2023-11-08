//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "raccoonApp.h"

class raccoonUnitApp : public raccoonApp
{
public:
  static InputParameters validParams();
  raccoonUnitApp(const InputParameters & parameters);
  virtual ~raccoonUnitApp();

  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
};
