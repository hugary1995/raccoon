//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class StrainEnergy : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  StrainEnergy(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const std::string _base_name;
  const ADMaterialProperty<RankTwoTensor> & _stress;
  const ADMaterialProperty<RankTwoTensor> & _strain;
};
