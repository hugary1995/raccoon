#ifndef CohesiveFractureAction_H
#define CohesiveFractureAction_H

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

#endif // NCohesiveFractureAction_H
