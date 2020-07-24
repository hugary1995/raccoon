//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "raccoonUnitApp.h"
#include "Moose.h"

template <>
InputParameters
validParams<raccoonUnitApp>()
{
  InputParameters params = validParams<raccoonApp>();
  return params;
}

raccoonUnitApp::raccoonUnitApp(const InputParameters & parameters) : raccoonApp(parameters)
{
  srand(processor_id());
  raccoonUnitApp::registerAll(_factory, _action_factory, _syntax);
}

void
raccoonUnitApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"raccoonUnitApp"});
}

raccoonUnitApp::~raccoonUnitApp() {}
