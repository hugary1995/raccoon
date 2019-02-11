#include "ElasticCouplingAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"

registerMooseAction("raccoonApp", ElasticCouplingAction, "add_material");

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

  return params;
}

ElasticCouplingAction::ElasticCouplingAction(const InputParameters & params)
  : Action(params),
    _block_name(name()),
    _strain(getParam<MooseEnum>("strain").getEnum<Strain>()),
    _irreversibility(getParam<MooseEnum>("irreversibility").getEnum<Irreversibility>())
{
}

void
ElasticCouplingAction::act()
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
    // degraded stress
    if (_strain == Strain::Small)
    {
      type = "SmallStrainElasticDegradedStress";
      name = type + "_block_" + _block_name;
      InputParameters params2 = _factory.getValidParams(type);
      if (_irreversibility != Irreversibility::History)
        params2.set<bool>("history") = false;
      params2.applyParameters(parameters());
      _problem->addMaterial(type, name, params2);
    }
  }
}
