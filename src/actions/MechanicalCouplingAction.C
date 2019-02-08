#include "MechanicalCouplingAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"

registerMooseAction("raccoonApp", MechanicalCouplingAction, "add_material");

template <>
InputParameters
validParams<MechanicalCouplingAction>()
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
  params.addParam<MaterialPropertyName>("degradation_base_name", "g", "name of lumped degradation");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_base_name", "w", "base name of local dissipation energy density");
  params.addParam<MaterialPropertyName>(
      "fracture_driving_force_base_name", "D", "base name of fracture driving force");

  MooseEnum strainType("SMALL FINITE", "SMALL");
  params.addParam<MooseEnum>("strain", strainType, "Strain formulation");
  MooseEnum irreversibilityType("HISTORY ACTIVE_SET TRUST_REGION NONE", "HISTORY");
  params.addParam<MooseEnum>(
      "irreversibility", irreversibilityType, "How to enforece irreversibility?");
  MooseEnum decompositionType("STRAIN_SPECTRAL NO_DECOMP", "NO_DECOMP");
  params.addParam<MooseEnum>("decomposition", decompositionType, "which decomposition to use");

  return params;
}

MechanicalCouplingAction::MechanicalCouplingAction(const InputParameters & params)
  : Action(params),
    _block_name(name()),
    _strain(getParam<MooseEnum>("strain").getEnum<Strain>()),
    _irreversibility(getParam<MooseEnum>("irreversibility").getEnum<Irreversibility>())
{
}

void
MechanicalCouplingAction::act()
{
  std::string type = "";
  std::string name = "";

  //
  // Add Materials
  //
  if (_current_task == "add_material")
  {
    // degradation
    type = "LumpedDegradation";
    name = type + "_block_" + _block_name;
    InputParameters params1 = _factory.getValidParams(type);
    params1.applyParameters(parameters());
    _problem->addMaterial(type, name, params1);
    // degradation
    type = "FractureDrivingForce";
    name = type + "_block_" + _block_name;
    InputParameters params2 = _factory.getValidParams(type);
    params2.applyParameters(parameters());
    _problem->addMaterial(type, name, params2);
    // degraded stress
    if (_strain == Strain::Small)
    {
      type = "LinearElasticDegradedStress";
      name = type + "_block_" + _block_name;
      InputParameters params3 = _factory.getValidParams(type);
      if (_irreversibility != Irreversibility::History)
        params3.set<bool>("history") = false;
      params3.applyParameters(parameters());
      _problem->addMaterial(type, name, params3);
    }
  }
}
