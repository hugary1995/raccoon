//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ComputeMultipleInelasticStress.h"

class PFFComputeMultipleInelasticStress : public ComputeMultipleInelasticStress
{
public:
  static InputParameters validParams();

  PFFComputeMultipleInelasticStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;

  // @{ Strain energy density and its derivative w/r/t damage
  const MaterialPropertyName _psie_name;
  MaterialProperty<Real> & _psie_active;
  // @}

  // @{ Interface energy density and its derivative w/r/t damage
  const MaterialPropertyName _psii_name;
  MaterialProperty<Real> & _psii_active;
  const MaterialProperty<Real> & _psii_active_old;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _gip_name;
  const MaterialProperty<Real> & _gip;
  const MaterialPropertyName _gop_name;
  const MaterialProperty<Real> & _gop;
  // @}

private:
  RankTwoTensor I2;
  RankFourTensor I4;
  RankFourTensor I4_ip;
  RankFourTensor I4_op;
  RankFourTensor I4_ip_vol;
  RankFourTensor I4_op_vol;
};
