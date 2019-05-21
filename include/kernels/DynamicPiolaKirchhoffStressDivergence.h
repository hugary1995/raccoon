//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PiolaKirchhoffStressDivergence.h"

// Forward Declarations
class DynamicPiolaKirchhoffStressDivergence;

template <>
InputParameters validParams<DynamicPiolaKirchhoffStressDivergence>();

class DynamicPiolaKirchhoffStressDivergence : public PiolaKirchhoffStressDivergence
{
public:
  DynamicPiolaKirchhoffStressDivergence(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<RankTwoTensor> & _stress_old;
  const MaterialProperty<RankTwoTensor> & _stress_older;
  const MaterialProperty<RankTwoTensor> * _F_old;
  const MaterialProperty<RankTwoTensor> * _F_older;

  // Rayleigh damping parameter _zeta and HHT time integration parameter _alpha
  const MaterialProperty<Real> & _zeta;
  const Real _alpha;
};

