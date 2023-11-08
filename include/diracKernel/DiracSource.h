//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// Moose Includes
#include "DiracKernel.h"

class DiracSource : public DiracKernel
{
public:
  static InputParameters validParams();
  DiracSource(const InputParameters & parameters);

  virtual void addPoints() override;
  virtual Real computeQpResidual() override;

protected:
  /// position of the dirac source
  Point _point;

  /// numbe of dimensions
  int _dim;
  Real _fL;

  /// list of parameters for the source equation
  Real _t1;
  Real _tRT;
  Real _tL;
  Real _tP;
  Real _p0;
  Real _d1;
  Real _upcoeff;
  Real _downcoeff;

  /// fluid density
  Real _rho;

  /// number of acoustic shots
  Real _num_shots;
};
