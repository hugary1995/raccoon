#include "ElasticCouplingAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"

registerMooseAction("raccoonApp", ElasticCouplingAction, "add_material");
registerMooseAction("raccoonApp", ElasticCouplingAction, "add_kernel");
registerMooseAction("raccoonApp", ElasticCouplingAction, "add_variable");

template <>
InputParameters
validParams<ElasticCouplingAction>()
{
  InputParameters params = validParams<Action>();
  params.addClassDescription(
      "Set up the kernels and materials needed for a phase field for fracture problem");

  params.addParam<std::vector<SubdomainName>>("block",
                                              "The list of ids of the blocks (subdomain) "
                                              "that the phase field kernels will be "
                                              "applied to");
  params.addParam<bool>(
      "use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParamNamesToGroup("use_displaced_mesh", "Advanced");

  params.addRequiredCoupledVar("damage_fields", "use vector coupling for all damage fields");
  params.addParam<MaterialPropertyName>("degradation_name", "g", "name of lumped degradation");
  params.addParam<MaterialPropertyName>(
      "fracture_driving_force_name", "D", "base name of fracture driving force");

  MooseEnum strainType("SMALL FINITE", "SMALL");
  params.addParam<MooseEnum>("strain", strainType, "Strain formulation");
  MooseEnum irreversibilityType("HISTORY ACTIVE_SET TRUST_REGION NONE", "HISTORY");
  params.addParam<MooseEnum>(
      "irreversibility", irreversibilityType, "How to enforece irreversibility?");
  MooseEnum decompositionType("STRAIN_SPECTRAL NO_DECOMP", "NO_DECOMP");
  params.addParam<MooseEnum>("decomposition", decompositionType, "which decomposition to use");

  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the elastic energy");
  params.addCoupledVar(
      "initial_positive_elastic_energy", 0, "variable that holds the initial strain energy field");
  params.addParam<MaterialPropertyName>(
      "mob_name", "M", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "fracture_energy_barrier_name",
      "b",
      "name of the material that holds the critical fracture energy");

  // displacement variables
  params.addParam<std::vector<VariableName>>(
      "displacements", "The nonlinear displacement variables for the problem");

  return params;
}

ElasticCouplingAction::ElasticCouplingAction(const InputParameters & params)
  : Action(params),
    _block_name(name()),
    _strain(getParam<MooseEnum>("strain").getEnum<Strain>()),
    _irreversibility(getParam<MooseEnum>("irreversibility").getEnum<Irreversibility>()),
    _displacements(getParam<std::vector<VariableName>>("displacements"))
{
}

void
ElasticCouplingAction::act()
{
  std::string type = "";
  std::string name = "";

  //
  // Add kernels
  //
  if (_current_task == "add_kernel")
  {
    if (_strain == Strain::Small)
      type = "StressDivergenceTensors";
    if (_strain == Strain::Finite)
      type = "PiolaKirchhoffStressDivergence";
    InputParameters kernel_params = _factory.getValidParams(type);

    for (unsigned int i = 0; i < _displacements.size(); ++i)
    {
      name = "TM_block_" + _block_name + "_" + Moose::stringify(i);
      kernel_params.set<unsigned int>("component") = i;
      kernel_params.set<NonlinearVariableName>("variable") = _displacements[i];
      kernel_params.applyParameters(parameters());

      _problem->addKernel(type, name, kernel_params);
    }
  }

  //
  // Add Materials
  //
  else if (_current_task == "add_material")
  {
    // degradation
    type = "LumpedDegradation";
    name = type + "_block_" + _block_name;
    InputParameters params1 = _factory.getValidParams(type);
    params1.applyParameters(parameters());
    _problem->addMaterial(type, name, params1);

    // degraded stress
    type = "SmallStrainElasticDegradedStress";
    name = type + "_block_" + _block_name;
    InputParameters params2 = _factory.getValidParams(type);
    if (_irreversibility != Irreversibility::History)
      params2.set<bool>("history") = false;
    params2.applyParameters(parameters());
    _problem->addMaterial(type, name, params2);

    // strain
    if (_strain == Strain::Small)
    {
      type = "ComputeSmallStrain";
      InputParameters params3 = _factory.getValidParams(type);
      name = type + "_block_" + _block_name;
      params3.applyParameters(parameters());
      _problem->addMaterial(type, name, params3);
    }
    if (_strain == Strain::Finite)
    {
      // we use automatic differention for deformation gradient
      type = "GreenStrain";
      InputParameters params3_residual = _factory.getValidParams(type + "<RESIDUAL>");
      InputParameters params3_jacobian = _factory.getValidParams(type + "<JACOBIAN>");
      name = type + "_block_" + _block_name;
      params3_residual.applyParameters(parameters());
      params3_jacobian.applyParameters(parameters());
      _problem->addADResidualMaterial(type + "<RESIDUAL>", name + "_residual", params3_residual);
      _problem->addADJacobianMaterial(type + "<JACOBIAN>", name + "_jacobian", params3_jacobian);
      _problem->haveADObjects(true);
    }
  }

  //
  // Add variables
  //
  else if (_current_task == "add_variable")
  {
    // determine necessary order
    const bool second = _problem->mesh().hasSecondOrderElements();
    // Loop through the displacement variables
    for (const auto & disp : _displacements)
      _problem->addVariable(disp,
                            FEType(Utility::string_to_enum<Order>(second ? "SECOND" : "FIRST"),
                                   Utility::string_to_enum<FEFamily>("LAGRANGE")),
                            1.0);
  }
}
