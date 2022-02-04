//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SDPlasticStrainBase.h"
#include "ADSingleVariableReturnMappingSolution.h"

class SDJ2PlasticStrain : public SDPlasticStrainBase, public ADSingleVariableReturnMappingSolution
{
public:
  static InputParameters validParams();

  SDJ2PlasticStrain(const InputParameters & params);

protected:
  virtual void computeQpProperties() override;

  virtual ADReal computeResidual(const ADReal & effective_trial_stress,
                                 const ADReal & delta_ep) override;

  virtual ADReal computeDerivative(const ADReal & effective_trial_stress,
                                   const ADReal & delta_ep) override;

  virtual Real computeReferenceResidual(const ADReal & effective_trial_stress,
                                        const ADReal & delta_ep) override;
};
