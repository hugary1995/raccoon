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
  virtual void finalize() override;
  virtual void threadJoin(const UserObject & y) override;

  /// Cumulative sum of the post-processor value
  Real _sum;

  /// The number of displacements
  const unsigned int _ndisp;

  /// Vector of velocities
  std::vector<const VariableValue *> _u_dots;

  /// The number of forces
  const unsigned int _nforce;

  /// Vector of forces
  std::vector<const VariableValue *> _forces;

  /// Cumulative sum of the post-processor value from the old time step */
  const PostprocessorValue & _sum_old;
};
