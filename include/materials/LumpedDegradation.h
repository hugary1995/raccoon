#ifndef LumpedDegradation_H
#define LumpedDegradation_H

#include "Material.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class LumpedDegradation;

template <>
InputParameters validParams<LumpedDegradation>();

class LumpedDegradation : public DerivativeMaterialInterface<Material>
{
public:
  LumpedDegradation(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// property name of the lumped degradation
  const MaterialPropertyName _g_name;

  /// number of coupled damage fields
  const unsigned int _num_fields;

  /// names of the coupled damage fields
  std::vector<std::string> _var_names;

  /// names of the degradation functions
  std::vector<std::string> _g_names;

  /// values the degradation for each damage field
  std::vector<const MaterialProperty<Real> *> _g;

  /// Derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _dg_dd;

  /// Second derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _d2g_dd2;

  /// value of the lumped degradation
  MaterialProperty<Real> * _product;

  /// Derivatives of the lumped degradation W.R.T. _var_names[i]
  std::vector<MaterialProperty<Real> *> _dproduct_dd;

  /// Second derivatives of the lumped degradation W.R.T. _var_names[i]
  std::vector<MaterialProperty<Real> *> _d2product_dd2;
};

#endif // LumpedDegradation_H
