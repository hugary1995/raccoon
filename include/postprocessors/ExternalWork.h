//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalPostprocessor.h"

class ExternalWork : public NodalPostprocessor
{
public:
  static InputParameters validParams();

  ExternalWork(const InputParameters & parameters);

protected:
  virtual void initialize() override;
  virtual void execute() override;
  virtual Real computeQpValue();
  virtual Real getValue() override;

  void threadJoin(const UserObject & y) override;

  Real _sum;

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
  const VariableValue & _f_0_old;
  const VariableValue & _f_1_old;
  const VariableValue & _f_2_old;

  /// cumulative sum of the post-processor value from the old time step */
  const PostprocessorValue & _sum_old;
};
