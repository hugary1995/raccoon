//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class SumStress : public ADMaterial
{
public:
  static InputParameters validParams();

  SumStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpProperties() override;

  const std::string _stress_name;

  const std::vector<std::string> & _base_names;

  const std::string _base_name;

  ADMaterialProperty<RankTwoTensor> & _stress;

  unsigned int _N;

  std::vector<const ADMaterialProperty<RankTwoTensor> *> _stresses;
};
