//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "raccoonApp.h"

class raccoonUnitApp;

template <>
InputParameters validParams<raccoonUnitApp>();

class raccoonUnitApp : public raccoonApp
{
public:
  raccoonUnitApp(const InputParameters & parameters);
  virtual ~raccoonUnitApp();

  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
};
