#ifndef SideIntegralPositiveMaxPrincipalStressPostprocessor_H
#define SideIntegralPositiveMaxPrincipalStressPostprocessor_H

#include "SideIntegralPostprocessor.h"
#include "RankTwoTensor.h"

// Forward Declarations
class SideIntegralPositiveMaxPrincipalStressPostprocessor;

template <>
InputParameters validParams<SideIntegralPositiveMaxPrincipalStressPostprocessor>();

class SideIntegralPositiveMaxPrincipalStressPostprocessor : public SideIntegralPostprocessor
{
public:
  SideIntegralPositiveMaxPrincipalStressPostprocessor(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// base name of stress
  const std::string _base_name;
  /// stress tensor
  const MaterialProperty<RankTwoTensor> & _stress;
};

#endif
