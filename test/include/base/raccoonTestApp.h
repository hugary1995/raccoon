//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef RACCOONTESTAPP_H
#define RACCOONTESTAPP_H

#include "MooseApp.h"

class raccoonTestApp;

template <>
InputParameters validParams<raccoonTestApp>();

class raccoonTestApp : public MooseApp
{
public:
  raccoonTestApp(InputParameters parameters);
  virtual ~raccoonTestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

#endif /* RACCOONTESTAPP_H */
