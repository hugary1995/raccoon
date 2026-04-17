#include "ADRankTwoTensorForward.h"
#include "ADReal.h"
#include "BaseNameInterface.h"
#include "InputParameters.h"
#include "Material.h"
#include "Moose.h"
#include "MooseTypes.h"
#include "RankTwoTensorForward.h"
#include "SolutionUserObject.h"
#include "SolutionTensor.h"
#include "Qp_Mapping.h"

registerADMooseObject("raccoonApp", SolutionTensor);

InputParameters
SolutionTensor::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("Getting tensor from solution object");
  params.addParam<UserObjectName>("solution", "The SolutionUserObject to extract data from.");
  params.addParam<std::string>("tensor_name", "stress", "Name of tensor");
  params.addRequiredParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  return params;
}

SolutionTensor::SolutionTensor(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _tensor_name(getParam<std::string>("tensor_name")),
    _tensor(declareADProperty<RankTwoTensor>(prependBaseName(_tensor_name + "_sol"))),
    _tensor_old(getMaterialPropertyOld<RankTwoTensor>(prependBaseName(_tensor_name + "_sol"))),
    _solution_object_ptr(NULL),
    _element(getParam<MooseEnum>("element").getEnum<QpMapping::Element>())
{
  _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/true);
}

void
SolutionTensor::initialSetup()
{
  _solution_object_ptr = &getUserObject<SolutionUserObject>("solution");
}

void
SolutionTensor::initStatefulProperties(unsigned int n_points)
{
  for (_qp = 0; _qp < n_points; ++_qp)
  {
    _tensor[_qp].setToIdentity();
  }

  unsigned int qp_max = _qpnum;

  auto formatQP = [qp_max](unsigned int qp)
  {
    if (qp_max < 10)
      return std::to_string(qp); // Single digit
    else
      return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp); // Two digits
  };

  std::vector<std::string> indices = {"x", "y", "z"};

  for (_qp = 0; _qp < n_points; ++_qp)
  {
    unsigned int qp_sel = QpMapping::getQP(_qp + 1, _lookup);

    // Populate tensor from solution object
    for (int i_ind = 0; i_ind < 3; i_ind++)
      for (int j_ind = 0; j_ind < 3; j_ind++)
      {
        _tensor[_qp](i_ind, j_ind) = _solution_object_ptr->pointValue(
            _t,
            _current_elem->true_centroid(),
            _tensor_name + "_" + indices[i_ind] + indices[j_ind] + "_" + formatQP(qp_sel),
            nullptr);
      }
  }
}