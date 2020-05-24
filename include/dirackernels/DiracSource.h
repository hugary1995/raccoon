//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// Moose Includes
#include "DiracKernel.h"

// Forward Declarations
class DiracSource;

template <>
InputParameters validParams<DiracSource>();

class DiracSource : public DiracKernel
{
public:
  DiracSource(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;

protected:
  Point _point;
  int _dim;
  Real _fL;
  Real _t1;
  Real _tRT;
  Real _tL;
  Real _tP;
  Real _p0;
  Real _d1;
  Real _upcoeff;
  Real _downcoeff;
  Real _rho;
};
