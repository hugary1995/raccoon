//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "InputParameters.h"
#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"
#include "MaterialProperty.h"
#include "MooseArray.h"
#include "SolutionUserObject.h"
#include "Qp_Mapping.h"

class SolutionTensor : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  SolutionTensor(const InputParameters & parameters);

  void initStatefulProperties(unsigned int n_points) override;

  void initialSetup() override;

protected:
  const std::string _tensor_name;

  ADMaterialProperty<RankTwoTensor> & _tensor;
  const MaterialProperty<RankTwoTensor> & _tensor_old;
  const SolutionUserObject * _solution_object_ptr;
  unsigned int _qpnum;
  QpMapping::Element _element;

private:
  const std::unordered_map<int, int> * _lookup;
};