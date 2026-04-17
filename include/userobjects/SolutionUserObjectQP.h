#pragma once

#include "SolutionUserObject.h"
#include "Qp_Mapping.h"

class SolutionUserObjectQP : public SolutionUserObject
{
public:
  static InputParameters validParams();
  SolutionUserObjectQP(const InputParameters & parameters);

protected:
  std::vector<MaterialName> _tensor_materials;
  std::vector<MaterialName> _materials;

  QpMapping::Element _element;

  unsigned int _qpnum;

private:
  const std::unordered_map<int, int> * _lookup;
};
