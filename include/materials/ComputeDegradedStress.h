#ifndef ComputeDegradedStress_H
#define ComputeDegradedStress_H

#include "DecompositionWarehouse.h"

// Forward Declarations
class ComputeDegradedStress;

template <>
InputParameters validParams<ComputeDegradedStress>();

class ComputeDegradedStress : public DecompositionWarehouse
{
public:
  ComputeDegradedStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;

  /// name of the material that holds the positive strain energy
  const MaterialPropertyName _E_el_pos_name;

  /// positive strain energy
  MaterialProperty<Real> & _E_el_pos;

  /// history maximum of positive strain energy upto the previous time step
  const MaterialProperty<Real> & _E_el_pos_maximum_old;

  /// initial strain energy field (if any)
  const VariableValue & _E_el_pos_initial;

  /// derivatives of elastic energy W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _dE_el_dd;

  /// second derivatives of elastic energy W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _d2E_el_dd2;

  /// whether to keep track of the maximum strain energy to enforce irreversibility
  const bool _history;
};

#endif // ComputeDegradedStress_H
