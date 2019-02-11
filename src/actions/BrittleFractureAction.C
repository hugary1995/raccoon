#include "BrittleFractureAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"
// LIBMESH includes
#include "libmesh/string_to_enum.h"

registerMooseAction("raccoonApp", BrittleFractureAction, "add_variable");
registerMooseAction("raccoonApp", BrittleFractureAction, "add_kernel");
registerMooseAction("raccoonApp", BrittleFractureAction, "add_material");

template <>
InputParameters
validParams<BrittleFractureAction>()
{
  InputParameters params = validParams<FractureActionBase>();
  params.addClassDescription(
      "Set up the kernels and materials needed for a phase field for fracture problem");

  params.addParam<Real>("local_dissipation_norm", 0.5, "norm of the local dissipation ||w(d)||");

  return params;
}

BrittleFractureAction::BrittleFractureAction(const InputParameters & params)
  : FractureActionBase(params)
{
}

void
BrittleFractureAction::act()
{
  FractureActionBase::act();

  std::string type = "";
  std::string name = "";

  //
  // Add Materials
  //
  if (_current_task == "add_material")
  {
    // degradation
    type = "DerivativeParsedMaterial";
    name = "Degradation_" + _var_name;
    InputParameters params1 = _factory.getValidParams(type);
    params1.set<std::vector<VariableName>>("args") = {_var_name};
    params1.set<std::string>("f_name") =
        getParam<MaterialPropertyName>("degradation_name") + "_" + _var_name;
    params1.set<std::vector<std::string>>("constant_names") = {"eta"};
    params1.set<std::vector<std::string>>("constant_expressions") = {
        getParam<std::string>("residual_degradation")};
    params1.set<std::string>("function") = "(1-" + _var_name + ")^2*(1-eta)+eta";
    params1.set<unsigned int>("derivative_order") = 2;
    params1.applyParameters(parameters());
    _problem->addMaterial(type, name, params1);
    // local free energy
    type = "DerivativeParsedMaterial";
    name = "LocalDissipationEnergy_" + _var_name;
    InputParameters params2 = _factory.getValidParams(type);
    params2.set<std::vector<VariableName>>("args") = {_var_name};
    params2.set<std::string>("f_name") =
        getParam<MaterialPropertyName>("local_dissipation_name") + "_" + _var_name;
    params2.set<std::string>("function") = _var_name + "^2";
    params2.set<unsigned int>("derivative_order") = 2;
    params2.applyParameters(parameters());
    _problem->addMaterial(type, name, params2);
    // critical fracture energy
    type = "FractureEnergyBarrier";
    name = type + "_" + _var_name;
    InputParameters params3 = _factory.getValidParams(type);
    params3.set<std::vector<VariableName>>("d") = {_var_name};
    params3.set<Real>("initial_degradation_slope") = -2.0;
    params3.set<Real>("initial_local_dissipation_slope") = 0.0;
    params3.applyParameters(parameters());
    _problem->addMaterial(type, name, params3);
  }
}
