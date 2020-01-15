//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADCompute2DSmallStrain.h"

template <ComputeStage>
class ADComputePlaneSmallStrain;

declareADValidParams(ADComputePlaneSmallStrain);

/**
 * ADComputePlaneSmallStrain defines small strains under plane stress assumption
 */
template <ComputeStage compute_stage>
class ADComputePlaneSmallStrain : public ADCompute2DSmallStrain<compute_stage>
{
public:
  static InputParameters validParams();

  ADComputePlaneSmallStrain(const InputParameters & parameters);

protected:
  virtual ADReal computeOutOfPlaneStrain() override;

  /// coupled out of plane strain
  const ADVariableValue & _out_of_plane_strain;

  usingCompute2DSmallStrainMembers;
};
