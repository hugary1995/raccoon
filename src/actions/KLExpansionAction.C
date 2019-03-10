#include "KLExpansionAction.h"

// MOOSE includes
#include "Factory.h"
#include "FEProblem.h"
#include "MooseObjectAction.h"
// LIBMESH includes
#include "libmesh/string_to_enum.h"
// cpp includes
#include <fstream>
#include <string>

registerMooseAction("raccoonApp", KLExpansionAction, "add_function");
registerMooseAction("raccoonApp", KLExpansionAction, "add_aux_variable");
registerMooseAction("raccoonApp", KLExpansionAction, "add_aux_kernel");

template <>
InputParameters
validParams<KLExpansionAction>()
{
  MooseEnum families(AddAuxVariableAction::getAuxVariableFamilies());
  MooseEnum orders(AddAuxVariableAction::getAuxVariableOrders());
  InputParameters params = validParams<Action>();
  params.addClassDescription("Automatically generate data files for KL basis functions and add "
                             "functions to interpolate the random field");
  params.addParam<MooseEnum>(
      "family", families, "Specifies the family of FE shape functions to use for this variable");
  params.addParam<MooseEnum>("order",
                             orders,
                             "Specifies the order of the FE shape function to use "
                             "for this variable (additional orders not listed are "
                             "allowed)");
  params.addRequiredParam<FileName>("file_name",
                                    "name of the file containing information for the KL expansion "
                                    "(run writeKL.m to generate KL info)");
  MooseEnum perturbationType("RANDOM CUSTOM", "RANDOM");
  params.addParam<MooseEnum>("perturbation", perturbationType, "type of perturbation");
  params.addParam<std::vector<Real>>("custom_Gaussian_weights", "custom Gaussian weights");
  params.addParam<bool>(
      "monitor_bases", false, "add aux variables and aux kernels to visualize bases");
  params.addParam<bool>(
      "monitor_field",
      true,
      "add aux variables and aux kernels to visualize the generated random field");
  params.addRequiredParam<std::string>("field_name", "name of the generated random field");
  MooseEnum distributionType("NORMAL GAMMA");
  params.addRequiredParam<MooseEnum>(
      "field_distribution", distributionType, "type of distribution");
  params.addRequiredParam<Real>("mean", "average value of the random field");
  params.addRequiredParam<Real>("CV", "coefficient of variation of the random field");
  return params;
}

KLExpansionAction::KLExpansionAction(const InputParameters & params)
  : Action(params),
    _fe_type(Utility::string_to_enum<Order>(getParam<MooseEnum>("order")),
             Utility::string_to_enum<FEFamily>(getParam<MooseEnum>("family"))),
    _file_name(getParam<FileName>("file_name")),
    _Gaussian_perturbation(getParam<MooseEnum>("perturbation").getEnum<GaussianPerturbation>()),
    _field_distribution(getParam<MooseEnum>("field_distribution").getEnum<FieldDistribution>()),
    _monitor_bases(getParam<bool>("monitor_bases")),
    _monitor_field(getParam<bool>("monitor_field")),
    _Gaussian_gen(ENGINE, GAUSSIAN_DIST)
{
  // seed random engine
  ENGINE.seed(std::time(NULL));

  generateDataFile();

  _functions.resize(_num_bases);
  _Gaussian_weights.resize(_num_bases);

  if (_Gaussian_perturbation == GaussianPerturbation::Random)
    for (unsigned int i = 0; i < _num_bases; i++)
      _Gaussian_weights[i] = _Gaussian_gen();
  else if (_Gaussian_perturbation == GaussianPerturbation::Custom)
    _Gaussian_weights = getParam<std::vector<Real>>("custom_Gaussian_weights");
}

void
KLExpansionAction::act()
{
  if (_current_task == "add_aux_variable")
  {
    if (_monitor_bases)
      for (unsigned int i = 0; i < _num_bases; i++)
        _problem->addAuxVariable("basis_" + std::to_string(i), _fe_type);
    if (_monitor_field)
      _problem->addAuxVariable(getParam<std::string>("field_name"), _fe_type);
  }
  else if (_current_task == "add_aux_kernel")
  {
    if (_monitor_bases)
      for (unsigned int i = 0; i < _num_bases; i++)
      {
        std::string type = "FunctionAux";
        std::string name = "basis_" + std::to_string(i);
        InputParameters aux_params = _factory.getValidParams(type);
        aux_params.set<AuxVariableName>("variable") = name;
        aux_params.set<FunctionName>("function") = name;
        // Set the execute options
        ExecFlagEnum execute_options = MooseUtils::getDefaultExecFlagEnum();
        execute_options = {EXEC_INITIAL};
        aux_params.set<ExecFlagEnum>("execute_on") = execute_options;
        aux_params.applyParameters(parameters());
        _problem->addAuxKernel(type, name, aux_params);
      }
    if (_monitor_field)
    {
      std::string type = "FunctionAux";
      std::string name = getParam<std::string>("field_name");
      InputParameters aux_params = _factory.getValidParams(type);
      aux_params.set<AuxVariableName>("variable") = name;
      aux_params.set<FunctionName>("function") = name;
      // Set the execute options
      ExecFlagEnum execute_options = MooseUtils::getDefaultExecFlagEnum();
      execute_options = {EXEC_INITIAL};
      aux_params.set<ExecFlagEnum>("execute_on") = execute_options;
      aux_params.applyParameters(parameters());
      _problem->addAuxKernel(type, name, aux_params);
    }
  }
  else if (_current_task == "add_function")
  {
    addFunctionInterpolator();
    addFieldGenerator();
  }
}

void
KLExpansionAction::generateDataFile()
{
  // preamble to store axis information
  std::string preamble = "";
  // data for each generated data file
  std::vector<std::string> eigvecs;

  // Let's read!
  std::ifstream file(_file_name);
  std::string line;
  while (std::getline(file, line))
  {
    // if line starts with AXIS, add it and its following line of data to the preamble
    if (line.find("AXIS") == 0)
    {
      preamble += line + "\n";
      std::getline(file, line);
      preamble += line + "\n";
    }
    // if line starts with DATA, add it and its following line of data to the eigvecs
    else if (line.find("DATA") == 0)
    {
      std::string eigvec = "";
      eigvec += line + "\n";
      std::getline(file, line);
      eigvec += line;
      eigvecs.push_back(eigvec);
    }
  }

  _num_bases = eigvecs.size();

  // Let's write!
  for (unsigned int i = 0; i < _num_bases; i++)
  {
    std::ofstream file(_file_name + std::to_string(i));
    // write preamble
    file << preamble;
    // write eigvec
    file << eigvecs[i];
  }
}

void
KLExpansionAction::addFunctionInterpolator()
{
  for (unsigned int i = 0; i < _num_bases; i++)
  {
    std::string type = "PiecewiseMultilinear";
    std::string name = "basis_" + std::to_string(i);
    _functions[i] = name;
    InputParameters interpolator_params = _factory.getValidParams(type);
    interpolator_params.set<FileName>("data_file") = _file_name + std::to_string(i);
    interpolator_params.applyParameters(parameters());
    _problem->addFunction(type, name, interpolator_params);
  }
}

void
KLExpansionAction::addFieldGenerator()
{
  std::string type = "";
  if (_field_distribution == FieldDistribution::Normal)
    type = "NormalRandomFieldFromBasisFunctions";
  else if (_field_distribution == FieldDistribution::Gamma)
    type = "GammaRandomFieldFromBasisFunctions";
  else
    mooseError("unknown filed distribution type");

  std::string name = getParam<std::string>("field_name");
  InputParameters field_params = _factory.getValidParams(type);
  field_params.set<std::vector<FunctionName>>("functions") = _functions;
  field_params.set<std::vector<Real>>("w") = _Gaussian_weights;
  field_params.applyParameters(parameters());
  _problem->addFunction(type, name, field_params);
}
