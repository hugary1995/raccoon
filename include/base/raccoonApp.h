//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef RACCOONAPP_H
#define RACCOONAPP_H

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

#endif /* RACCOONAPP_H */
