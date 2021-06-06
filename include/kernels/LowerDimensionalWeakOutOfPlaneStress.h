//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "WeakPlaneStress.h"
#include "Function.h"

class LowerDimensionalWeakOutOfPlaneStress : public WeakPlaneStress
{
public:
  static InputParameters validParams();

  LowerDimensionalWeakOutOfPlaneStress(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const Function & _out_of_plane_stress;
  const MaterialProperty<RankTwoTensor> & _Q;
};
