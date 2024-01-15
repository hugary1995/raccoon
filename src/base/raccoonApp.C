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

  // Do not use legacy material output option.
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

raccoonApp::raccoonApp(InputParameters parameters) : MooseApp(parameters)
{
  raccoonApp::registerAll(_factory, _action_factory, _syntax);
}

raccoonApp::~raccoonApp() {}

static void
associateSyntaxInner(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
raccoonApp::registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax)
{
  ModulesApp::registerAll(factory, action_factory, syntax);
  Registry::registerObjectsTo(factory, {"raccoonApp"});
  Registry::registerActionsTo(action_factory, {"raccoonApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
raccoonApp::registerApps()
{
  registerApp(raccoonApp);
  ModulesApp::registerApps();
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
