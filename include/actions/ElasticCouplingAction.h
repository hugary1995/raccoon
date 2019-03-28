#ifndef ElasticCouplingAction_H
#define ElasticCouplingAction_H

// MOOSE includes
#include "Action.h"

// Forward declaration
class ElasticCouplingAction;

template <>
InputParameters validParams<ElasticCouplingAction>();

class ElasticCouplingAction : public Action
{
public:
  ElasticCouplingAction(const InputParameters & params);

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
  /// nonliear displacement variables
  std::vector<VariableName> _displacements;
};

#endif // NElasticCouplingAction_H
