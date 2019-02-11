#ifndef BrittleFractureAction_H
#define BrittleFractureAction_H

// MOOSE includes
#include "FractureActionBase.h"

// Forward declaration
class BrittleFractureAction;

template <>
InputParameters validParams<BrittleFractureAction>();

class BrittleFractureAction : public FractureActionBase
{
public:
  BrittleFractureAction(const InputParameters & params);

  virtual void act() override;

protected:
};

#endif // NBrittleFractureAction_H
