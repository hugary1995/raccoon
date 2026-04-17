#include "SolutionUserObjectQP.h"
// MOOSE includes
#include "ConsoleUtils.h"
#include "MooseError.h"
#include "MooseMesh.h"
#include "MooseUtils.h"
#include "MooseVariableFE.h"
#include "RotationMatrix.h"
#include "Function.h"

// libMesh includes
#include "libmesh/equation_systems.h"
#include "libmesh/mesh_function.h"
#include "libmesh/numeric_vector.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/transient_system.h"
#include "libmesh/parallel_mesh.h"
#include "libmesh/serial_mesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/exodusII_io_helper.h"
#include "libmesh/enum_xdr_mode.h"
#include "Qp_Mapping.h"
registerMooseObject("raccoonApp", SolutionUserObjectQP);

InputParameters
SolutionUserObjectQP::validParams()
{
  InputParameters params = SolutionUserObject::validParams();
  params.addParam<std::vector<MaterialName>>("tensor_materials", "materials to output qps on");
  params.addParam<std::vector<MaterialName>>("materials", "materials to output qps on");
  params.addRequiredParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  return params;
}

SolutionUserObjectQP::SolutionUserObjectQP(const InputParameters & parameters)
  : SolutionUserObject(parameters),
    _tensor_materials(getParam<std::vector<MaterialName>>("tensor_materials")),
    _materials(getParam<std::vector<MaterialName>>("materials")),
    _element(getParam<MooseEnum>("element").getEnum<QpMapping::Element>())
{
  _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/true);

  unsigned int qp_max = _qpnum;
  int dim = 3;

  auto formatQP = [qp_max](unsigned int qp)
  {
    if (qp_max < 10)
      return std::to_string(qp); // Single digit
    else
      return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp); // Two digits
  };

  if (!_tensor_materials.empty())
  {
    std::vector<std::string> conv = {"x", "y", "z"};
    for (unsigned int i = 0; i < _tensor_materials.size(); i++)
    {
      std::string matname = _tensor_materials[i];
      if (_tensor_materials[i].size() > 26)
        matname.erase(26, _tensor_materials[i].size() - 1);
      for (unsigned int qp = 1; qp <= qp_max; qp++)
      {
        unsigned int qp_sel = QpMapping::getQP(qp, _lookup);
        for (int j = 0; j < dim; j++)
          for (int k = 0; k < dim; k++)
          {
            _system_variables.push_back(matname + "_" + conv[j] + conv[k] + "_" + formatQP(qp_sel));
          }
      }
    }
  }
  if (!_materials.empty())
  {
    for (unsigned int i = 0; i < _materials.size(); i++)
    {
      // Assuming 8 QPs
      // Starting at 1 because qps start at one in Sierra
      for (unsigned int qp = 1; qp <= qp_max; qp++)
      {
        unsigned int qp_sel = QpMapping::getQP(qp, _lookup);
        _system_variables.push_back(_materials[i] + "_" + formatQP(qp_sel));
      }
    }
  }
}
