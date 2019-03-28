#ifndef PressureBC_H
#define PressureBC_H

#include "ADIntegratedBC.h"

template <ComputeStage>
class PressureBC;

declareADValidParams(PressureBC);

template <ComputeStage compute_stage>
class PressureBC : public ADIntegratedBC<compute_stage>
{
public:
  PressureBC(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// The function describing the pressure magnitude
  Function & _function;

  const unsigned int _component;

  usingIntegratedBCMembers;
};

#endif /* PressureBC_H */
