#include "raccoonApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<raccoonApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

raccoonApp::raccoonApp(InputParameters parameters) : MooseApp(parameters)
{
  raccoonApp::registerAll(_factory, _action_factory, _syntax);
}

raccoonApp::~raccoonApp() {}

static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerSyntax("BrittleFractureAction", "Modules/PhaseFieldFracture/BrittleFracture/*");
  registerSyntax("ElasticCouplingAction", "Modules/PhaseFieldFracture/ElasticCoupling/*");
}

void
raccoonApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"raccoonApp"});
  Registry::registerActionsTo(af, {"raccoonApp"});
  associateSyntaxInner(s, af);

  /* register custom execute flags, action syntax, etc. here */
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
