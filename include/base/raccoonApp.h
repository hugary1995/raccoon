//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MooseApp.h"

class raccoonApp;

template <>
InputParameters validParams<raccoonApp>();

class raccoonApp : public MooseApp
{
public:
  raccoonApp(InputParameters parameters);
  virtual ~raccoonApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
};

