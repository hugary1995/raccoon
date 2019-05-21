//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CohesiveFractureAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"
// LIBMESH includes
#include "libmesh/string_to_enum.h"

registerMooseAction("raccoonApp", CohesiveFractureAction, "add_variable");
registerMooseAction("raccoonApp", CohesiveFractureAction, "add_kernel");
registerMooseAction("raccoonApp", CohesiveFractureAction, "add_material");

template <>
InputParameters
validParams<CohesiveFractureAction>()
{
  InputParameters params = validParams<FractureActionBase>();
  params.addClassDescription(
      "Set up the kernels and materials needed for a cohesive fracture model");

  params.addParam<Real>(
      "local_dissipation_norm", 2.0 / 3.0, "norm of the local dissipation ||w(d)||");
  params.addParam<std::string>("p", "1.0", "shape parameter to control the size of cohesive zone");
  params.addRequiredParam<FunctionName>("fracture_energy_barrier",
                                        "value of the critical fracture energy");

  return params;
}

CohesiveFractureAction::CohesiveFractureAction(const InputParameters & params)
  : FractureActionBase(params)
{
}

void
CohesiveFractureAction::act()
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
    params1.set<std::vector<std::string>>("material_property_names") = {
        getParam<MaterialPropertyName>("mob_name") + "_" + _var_name,
        getParam<MaterialPropertyName>("fracture_energy_barrier_name") + "_" + _var_name};
    params1.set<std::string>("f_name") =
        getParam<MaterialPropertyName>("degradation_name") + "_" + _var_name;
    params1.set<std::vector<std::string>>("constant_names") = {"eta", "p"};
    params1.set<std::vector<std::string>>("constant_expressions") = {
        getParam<std::string>("residual_degradation"), getParam<std::string>("p")};
    // below is the messy version of (1-d)^2/((1-d)^2+M_d/b_d*d*(1+p*d))*(1-eta)+eta
    params1.set<std::string>("function") =
        "(1-" + _var_name + ")^2/((1-" + _var_name + ")^2+" +
        getParam<MaterialPropertyName>("mob_name") + "_" + _var_name + "/" +
        getParam<MaterialPropertyName>("fracture_energy_barrier_name") + "_" + _var_name + "*" +
        _var_name + "*(1+p*" + _var_name + "))*(1-eta)+eta";
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
    params2.set<std::string>("function") = _var_name;
    params2.set<unsigned int>("derivative_order") = 2;
    params2.applyParameters(parameters());
    _problem->addMaterial(type, name, params2);

    // critical fracture energy
    type = "StationaryGenericFunctionMaterial";
    name = "FractureEnergyBarrier_" + _var_name;
    InputParameters params3 = _factory.getValidParams(type);
    params3.set<std::vector<std::string>>("prop_names") = {
        getParam<MaterialPropertyName>("fracture_energy_barrier_name") + "_" + _var_name};
    params3.set<std::vector<FunctionName>>("prop_values") = {
        getParam<FunctionName>("fracture_energy_barrier")};
    params3.applyParameters(parameters());
    _problem->addMaterial(type, name, params3);
  }
}
