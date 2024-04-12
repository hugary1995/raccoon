//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonApp.h"
#include "raccoonAppTypes.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
raccoonApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("automatic_automatic_scaling") = false;
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_bahavior") = false;
  return params;
}

raccoonApp::raccoonApp(InputParameters parameters) : MooseApp(parameters)
{
  raccoonApp::registerAll(_factory, _action_factory, _syntax);
}

raccoonApp::~raccoonApp() {}

void
raccoonApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<raccoonApp>(f, af, s);
  Registry::registerObjectsTo(f, {"raccoonApp"});
  Registry::registerActionsTo(af, {"raccoonApp"});
}

void
raccoonApp::registerApps()
{
  registerApp(raccoonApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
raccoonApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  raccoonApp::registerAll(f, af, s);
}
extern "C" void
raccoonApp__registerApps()
{
  raccoonApp::registerApps();
}
