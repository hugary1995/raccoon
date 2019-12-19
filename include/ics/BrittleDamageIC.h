//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#ifndef BrittleDamageIC_H
#define BrittleDamageIC_H

#include "DamageICBase.h"

class BrittleDamageIC;

template <>
InputParameters validParams<BrittleDamageIC>();

class BrittleDamageIC : public DamageICBase
{
public:
  BrittleDamageIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point & p);
};

#endif // BrittleDamageIC_H
