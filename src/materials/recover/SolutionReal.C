#include "BaseNameInterface.h"
#include "InputParameters.h"
#include "Material.h"
#include "MooseTypes.h"
#include "RankTwoTensorForward.h"
#include "SolutionUserObject.h"
#include "SolutionReal.h"
#include "Qp_Mapping.h"

registerADMooseObject("raccoonApp", SolutionReal);

InputParameters
SolutionReal::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("Getting tensor from solution object");
  params.addParam<UserObjectName>("solution", "The SolutionUserObject to extract data from.");
  params.addParam<Real>("num_qps", 8, "Number of QPs");
  params.addParam<std::string>("mat_name", "stress", "Name of tensor");
  params.addRequiredParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  return params;
}

SolutionReal::SolutionReal(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _mat_name(getParam<std::string>("mat_name")),
    _mat(declareADProperty<Real>(prependBaseName(_mat_name + "_sol"))),
    _mat_old(getMaterialPropertyOld<Real>(prependBaseName(_mat_name + "_sol"))),
    _solution_object_ptr(NULL),
    _element(getParam<MooseEnum>("element").getEnum<QpMapping::Element>())
{
  _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/true);
}

void
SolutionReal::initialSetup()
{
  _solution_object_ptr = &getUserObject<SolutionUserObject>("solution");
}

void
SolutionReal::initStatefulProperties(unsigned int n_points)
{
  unsigned int qp_max = _qpnum;

  auto formatQP = [qp_max](unsigned int qp)
  {
    if (qp_max < 10)
      return std::to_string(qp); // Single digit
    else
      return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp); // Two digits
  };

  for (_qp = 0; _qp < n_points; ++_qp)
  {
    unsigned int qp_sel = QpMapping::getQP(_qp + 1, _lookup);
    // Populate from solution user object
    _mat[_qp] = _solution_object_ptr->pointValue(
        _t, _current_elem->true_centroid(), _mat_name + "_" + formatQP(qp_sel), nullptr);
  }
}