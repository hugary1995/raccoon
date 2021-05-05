//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

/**
 * This class computes the stress triaxiliaty
 */
class StressTriaxiality : public Material
{
public:
  static InputParameters validParams();

  StressTriaxiality(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// Base name of the material system
  const std::string _base_name;

  /// Traxiality
  ADMaterialProperty<Real> & _T;

  /// Stress
  const ADMaterialProperty<RankTwoTensor> & _stress;
};
