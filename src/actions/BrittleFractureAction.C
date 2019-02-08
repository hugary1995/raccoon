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
  InputParameters params = validParams<Action>();
  params.addClassDescription(
      "Set up the kernels and materials needed for a phase field for fracture problem");

  // general
  // Get MooseEnums for the possible order/family options for this variable
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());
  params.addParam<MooseEnum>("family",
                             families,
                             "Specifies the family of FE "
                             "shape functions to use for this variable");
  params.addParam<MooseEnum>("order",
                             orders,
                             "Specifies the order of the FE "
                             "shape function to use for this variable");
  params.addParam<std::vector<SubdomainName>>("block",
                                              "The list of ids of the blocks (subdomain) "
                                              "that the phase field kernels will be "
                                              "applied to");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addParam<bool>("implicit", true, "Whether kernels are implicit or not");
  params.addParam<bool>(
      "use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParamNamesToGroup("scaling implicit use_displaced_mesh", "Advanced");

  // viscosity
  params.addParam<Real>("viscosity", 0.0, "viscosity of crack growth");

  // for material
  params.addRequiredParam<FunctionName>("Gc", "fracture energy release rate");
  params.addRequiredParam<FunctionName>("L", "characteristic crack length scale");
  params.addParam<Real>("local_dissipation_norm", 0.5, "norm of the local dissipation ||w(d)||");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_base_name", "w", "base name of the local dissipation density");
  params.addParam<MaterialPropertyName>(
      "kappa_base_name", "kappa", "base name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_base_name", "mobility", "base name of the material that holds the mobility");

  // for degradation
  params.addParam<MaterialPropertyName>("degradation_base_name", "g", "base name of degradation");
  params.addParam<Real>("residual_degradation",
                        1e-6,
                        "a small number to prevent singularity when degradation becomes zero");

  // other base names
  params.addParam<MaterialPropertyName>(
      "fracture_driving_force_base_name", "D", "base name of fracture driving force");

  // displacement variables
  params.addParam<std::vector<VariableName>>(
      "displacements", "The nonlinear displacement variables for the problem");

  return params;
}

BrittleFractureAction::BrittleFractureAction(const InputParameters & params)
  : Action(params),
    _var_name(name()),
    _fe_type(Utility::string_to_enum<Order>(getParam<MooseEnum>("order")),
             Utility::string_to_enum<FEFamily>(getParam<MooseEnum>("family"))),
    _displacements(getParam<std::vector<VariableName>>("displacements"))
{
}

void
BrittleFractureAction::act()
{
  std::string type = "";
  std::string name = "";

  //
  // Add variable
  //
  if (_current_task == "add_variable")
    _problem->addVariable(_var_name, _fe_type, getParam<Real>("scaling"));
  //
  // Add kernels
  //
  if (_current_task == "add_kernel")
  {
    // Add time derivative kernel
    Real viscosity = getParam<Real>("viscosity");
    if (viscosity > 0.0)
    {
      type = "CoefTimeDerivative";
      name = type + "_" + _var_name;
      InputParameters params1 = _factory.getValidParams(type);
      params1.set<NonlinearVariableName>("variable") = _var_name;
      params1.set<Real>("Coefficient") = getParam<Real>("viscosity");
      params1.applyParameters(parameters());
      _problem->addKernel(type, name, params1);
    }
    // Add AllenCahn kernel
    type = "AllenCahn";
    name = type + "_" + _var_name;
    InputParameters params2 = _factory.getValidParams(type);
    params2.set<NonlinearVariableName>("variable") = _var_name;
    params2.set<MaterialPropertyName>("mob_name") =
        getParam<MaterialPropertyName>("mobility_base_name") + "_" + _var_name;
    params2.set<MaterialPropertyName>("f_name") =
        getParam<MaterialPropertyName>("fracture_driving_force_base_name") + "_" + _var_name;
    params2.applyParameters(parameters());
    _problem->addKernel(type, name, params2);
    // Add ACInterface kernel
    type = "ACInterface";
    name = type + "_" + _var_name;
    InputParameters params3 = _factory.getValidParams(type);
    params3.set<NonlinearVariableName>("variable") = _var_name;
    params3.set<MaterialPropertyName>("mob_name") =
        getParam<MaterialPropertyName>("mobility_base_name") + "_" + _var_name;
    params3.set<MaterialPropertyName>("kappa_name") =
        getParam<MaterialPropertyName>("kappa_base_name") + "_" + _var_name;
    params3.applyParameters(parameters());
    _problem->addKernel(type, name, params3);
    // add off diagonal kernels for dR_disp/dd
    type = "PhaseFieldFractureMechanicsOffDiag";
    name = type + "_" + _var_name;
    InputParameters params4 = _factory.getValidParams(type);
    for (unsigned int i = 0; i < _displacements.size(); ++i)
    {
      params4.set<unsigned int>("component") = i;
      params4.set<NonlinearVariableName>("variable") = _displacements[i];
      params4.set<std::vector<VariableName>>("c") = {_var_name};
      params4.applyParameters(parameters());
      _problem->addKernel(type, name + "_" + Moose::stringify(i), params4);
    }
  }
  //
  // Add Materials
  //
  else if (_current_task == "add_material")
  {
    // Bulk
    type = "BrittleFractureMaterial";
    name = type + "_" + _var_name;
    InputParameters params1 = _factory.getValidParams(type);
    params1.set<std::vector<VariableName>>("d") = {_var_name};
    params1.applyParameters(parameters());
    _problem->addMaterial(type, name, params1);
    // degradation
    type = "DerivativeParsedMaterial";
    name = "Degradation_" + _var_name;
    InputParameters params2 = _factory.getValidParams(type);
    params2.set<std::vector<VariableName>>("args") = {_var_name};
    params2.set<std::string>("f_name") =
        getParam<MaterialPropertyName>("degradation_base_name") + "_" + _var_name;
    params2.set<std::vector<std::string>>("tol_names") = {"eta"};
    params2.set<std::vector<Real>>("tol_values") = {getParam<Real>("residual_degradation")};
    params2.set<std::string>("function") = "(1-" + _var_name + ")^2";
    params2.set<unsigned int>("derivative_order") = 2;
    params2.applyParameters(parameters());
    _problem->addMaterial(type, name, params2);
    // local free energy
    type = "DerivativeParsedMaterial";
    name = "LocalDissipationEnergy_" + _var_name;
    InputParameters params3 = _factory.getValidParams(type);
    params3.set<std::vector<VariableName>>("args") = {_var_name};
    params3.set<std::string>("f_name") =
        getParam<MaterialPropertyName>("local_dissipation_base_name") + "_" + _var_name;
    params3.set<std::string>("function") = _var_name + "^2";
    params3.set<unsigned int>("derivative_order") = 2;
    params3.applyParameters(parameters());
    _problem->addMaterial(type, name, params3);
  }
}
