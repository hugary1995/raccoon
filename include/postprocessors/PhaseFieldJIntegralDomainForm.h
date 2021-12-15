//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PhaseFieldJIntegralDomainForm : public ElementIntegralPostprocessor,
                                      public BaseNameInterface,
                                      public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldJIntegralDomainForm(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;

  /// The strain energy density
  const ADMaterialProperty<Real> & _psie;

  /// The number of displacements
  const unsigned int _ndisp;

  /// Vector of displacement gradients
  std::vector<const VariableGradient *> _grad_u;

  const RealVectorValue _t;

  const VariableGradient & _grad_q;
};
