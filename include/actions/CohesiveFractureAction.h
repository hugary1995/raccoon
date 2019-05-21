//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE includes
#include "FractureActionBase.h"

// Forward declaration
class CohesiveFractureAction;

template <>
InputParameters validParams<CohesiveFractureAction>();

class CohesiveFractureAction : public FractureActionBase
{
public:
  CohesiveFractureAction(const InputParameters & params);

  virtual void act() override;

protected:
};

