//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalPostprocessor.h"

class ArcLengthStep : public NodalPostprocessor
{
public:
  static InputParameters validParams();

  ArcLengthStep(const InputParameters & parameters);

protected:
  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;

  void threadJoin(const UserObject & y) override;

  const unsigned int _ndisp;
  const VariableValue & _disp_0;
  const VariableValue & _disp_1;
  const VariableValue & _disp_2;
  const VariableValue & _disp_0_old;
  const VariableValue & _disp_1_old;
  const VariableValue & _disp_2_old;

  const unsigned int _nforce;
  const VariableValue & _f_0;
  const VariableValue & _f_1;
  const VariableValue & _f_2;

  const Real _delta_D;
  const Real _delta_U;
  Real _sum1;
  Real _sum2;
  const PostprocessorValue & _lambda_old;
};
