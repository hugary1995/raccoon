//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonTestApp.h"
#include "raccoonApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
raccoonTestApp::validParams()
{
  InputParameters params = raccoonApp::validParams();
  return params;
}

raccoonTestApp::raccoonTestApp(InputParameters parameters) : MooseApp(parameters)
{
  raccoonTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

raccoonTestApp::~raccoonTestApp() {}

void
raccoonTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  raccoonApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"raccoonTestApp"});
    Registry::registerActionsTo(af, {"raccoonTestApp"});
  }
}

void
raccoonTestApp::registerApps()
{
  raccoonApp::registerApps();
  registerApp(raccoonTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
raccoonTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  raccoonTestApp::registerAll(f, af, s);
}
extern "C" void
raccoonTestApp__registerApps()
{
  raccoonTestApp::registerApps();
}
