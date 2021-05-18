//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"

class KineticEnergy : public ElementIntegralPostprocessor, public BaseNameInterface
{
public:
  static InputParameters validParams();

  KineticEnergy(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// The density
  const ADMaterialProperty<Real> & _rho;

  /// The number of displacements
  const unsigned _ndisp;

  /// The vector of velocities
  std::vector<const VariableValue *> _u_dots;
};
