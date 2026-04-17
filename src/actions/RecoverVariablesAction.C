#include "Factory.h"
#include "FEProblem.h"
#include "AddVariableAction.h"
#include "InputParameters.h"
#include "NonlinearSystemBase.h"
#include "Parser.h"
#include "Qp_Mapping.h"
#include "RecoverVariablesAction.h"

registerMooseAction("raccoonApp", RecoverVariablesAction, "add_aux_variable");
registerMooseAction("raccoonApp", RecoverVariablesAction, "add_aux_kernel");

InputParameters
RecoverVariablesAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addParam<std::vector<MaterialName>>("tensor_materials", "materials to output qps on");
  params.addParam<std::vector<MaterialName>>("materials", "materials to output qps on");
  params.addParam<std::string>("output_name", "exodusqp", "name of output for variables");
  // params.addParam<Real>("num_qps", 8, "Number of QPs");
  params.addRequiredParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  return params;
}

RecoverVariablesAction::RecoverVariablesAction(const InputParameters & params)
  : Action(params),
    _tensor_materials(getParam<std::vector<MaterialName>>("tensor_materials")),
    _materials(getParam<std::vector<MaterialName>>("materials")),
    _output_name(getParam<std::string>("output_name")),
    _element(getParam<MooseEnum>("element").getEnum<QpMapping::Element>())
{
  _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/false);
}

void
RecoverVariablesAction::act()
{
  auto dim = _mesh->dimension();
  unsigned int qp_max = _qpnum;

  std::vector<std::string> conv = {"x", "y", "z"};

  // Formatting qp number in the case where num_qps > 10
  auto formatQP = [qp_max](unsigned int qp)
  {
    if (qp_max < 10)
      return std::to_string(qp); // Single digit
    else
      return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp); // Two digits
  };

  if (_current_task == "add_aux_variable")
  {
    auto var_params = _factory.getValidParams("MooseVariable");
    var_params.set<MooseEnum>("order") = "CONSTANT";
    var_params.set<MooseEnum>("family") = "MONOMIAL";
    var_params.set<std::vector<OutputName>>("outputs") = {_output_name};
    // Non tensor mats
    for (unsigned int i = 0; i < _materials.size(); i++)
    {
      // Starting at 1 because qps start at one in Sierra
      for (unsigned int qp = 1; qp <= qp_max; qp++)
        _problem->addAuxVariable("MooseVariable", _materials[i] + "_" + formatQP(qp), var_params);
    }

    // Tensor mats
    for (unsigned int i = 0; i < _tensor_materials.size(); i++)
    {
      // Assuming 8 QPs
      // Starting at 1 because qps start at one in Sierra
      for (unsigned int qp = 1; qp <= qp_max; qp++)
        for (unsigned int j = 0; j < dim; j++)
          for (unsigned int k = 0; k < dim; k++)
          {
            std::string matname = _tensor_materials[i];
            if (_tensor_materials[i].size() > 26)
              matname.erase(26, _tensor_materials[i].size() - 1);
            _problem->addAuxVariable("MooseVariable",
                                     matname + "_" + conv[j] + conv[k] + "_" + formatQP(qp),
                                     var_params);
          }
    }
  }

  if (_current_task == "add_aux_kernel")
  {
    // Non tensor
    for (unsigned int i = 0; i < _materials.size(); i++)
      // assuming 8 QPs
      for (unsigned int qp = 1; qp <= qp_max; qp++)
      {
        unsigned int qp_sel = QpMapping::getQP(qp, _lookup);
        InputParameters params = _factory.getValidParams("ADMaterialRealAux");
        params.set<AuxVariableName>("variable") = _materials[i] + "_" + formatQP(qp);
        params.set<MaterialPropertyName>("property") = _materials[i];
        params.set<unsigned int>("selected_qp") = qp_sel - 1;
        _problem->addAuxKernel("ADMaterialRealAux", _materials[i] + "_" + formatQP(qp), params);
      }

    // tensor
    for (unsigned int i = 0; i < _tensor_materials.size(); i++)
    {
      std::string matname = _tensor_materials[i];
      if (_tensor_materials[i].size() > 26)
        matname.erase(26, _tensor_materials[i].size() - 1);
      // assuming 8 QPs
      for (unsigned int qp = 1; qp <= qp_max; qp++)
      {
        unsigned int qp_sel = QpMapping::getQP(qp, _lookup);
        for (unsigned int j = 0; j < dim; j++)
          for (unsigned int k = 0; k < dim; k++)
          {
            InputParameters params = _factory.getValidParams("ADRankTwoAux");
            params.set<AuxVariableName>("variable") =
                matname + "_" + conv[j] + conv[k] + "_" + formatQP(qp);
            params.set<MaterialPropertyName>("rank_two_tensor") = _tensor_materials[i];
            params.set<unsigned int>("selected_qp") = qp_sel - 1;
            params.set<unsigned int>("index_i") = j;
            params.set<unsigned int>("index_j") = k;
            _problem->addAuxKernel("ADRankTwoAux",
                                   _tensor_materials[i] + "_" + conv[j] + conv[k] + "_" +
                                       formatQP(qp),
                                   params);
          }
      }
    }
  }
}
