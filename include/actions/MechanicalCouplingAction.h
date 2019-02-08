#ifndef MechanicalCouplingAction_H
#define MechanicalCouplingAction_H

// MOOSE includes
#include "Action.h"

// Forward declaration
class MechanicalCouplingAction;

template <>
InputParameters validParams<MechanicalCouplingAction>();

class MechanicalCouplingAction : public Action
{
public:
  MechanicalCouplingAction(const InputParameters & params);

  virtual void act();

protected:
  /// list of restricted block
  const std::string _block_name;
  /// strain formulation
  enum class Strain
  {
    Small,
    Finite
  } _strain;
  /// irreversibility formulation
  enum class Irreversibility
  {
    History,
    Active_set,
    Trust_region,
    none
  } _irreversibility;
};

#endif // NMechanicalCouplingAction_H
