//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonUnitApp.h"
#include "Moose.h"

InputParameters
raccoonUnitApp::validParams()
{
  InputParameters params = raccoonApp::validParams();
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
