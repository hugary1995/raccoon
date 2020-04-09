//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADStressDivergenceTensors.h"

// Forward Declarations
template <ComputeStage>
class AD1DStressDivergenceTensors;

declareADValidParams(AD1DStressDivergenceTensors);

template <ComputeStage compute_stage>
class AD1DStressDivergenceTensors : public ADStressDivergenceTensors<compute_stage>
{
public:
  static InputParameters validParams();

  AD1DStressDivergenceTensors(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialProperty(Real) & _A;

  usingStressDivergenceTensorsMembers;
};
