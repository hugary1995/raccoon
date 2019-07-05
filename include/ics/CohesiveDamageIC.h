//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#ifndef CohesiveDamageIC_H
#define CohesiveDamageIC_H

#include "DamageICBase.h"

class CohesiveDamageIC;

template <>
InputParameters validParams<CohesiveDamageIC>();

class CohesiveDamageIC : public DamageICBase
{
public:
  CohesiveDamageIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point & p);
};

#endif // CohesiveDamageIC_H
