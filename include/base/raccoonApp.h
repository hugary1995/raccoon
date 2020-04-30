//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MooseApp.h"

class raccoonApp : public MooseApp
{
public:
  static InputParameters validParams();

  raccoonApp(InputParameters parameters);

  virtual ~raccoonApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
};
