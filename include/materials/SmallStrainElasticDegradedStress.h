#ifndef SmallStrainElasticDegradedStress_H
#define SmallStrainElasticDegradedStress_H

#include "DegradedStressBase.h"

// Forward Declarations
class SmallStrainElasticDegradedStress;

template <>
InputParameters validParams<SmallStrainElasticDegradedStress>();

class SmallStrainElasticDegradedStress : public DegradedStressBase
{
public:
  SmallStrainElasticDegradedStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;
  virtual void computeAdditionalFreeEnergy(Real, Real);

  /// name of the material holding elastic energy
  MaterialPropertyName _E_el_name;

  /// initial strain energy field (if any)
  const VariableValue & _E_el_pos_initial;

  /// positive strain energy
  MaterialProperty<Real> & _E_el_pos;

  /// history maximum of positive strain energy upto the previous time step
  const MaterialProperty<Real> & _E_el_pos_old;

  /// mobility name
  const MaterialPropertyName _mob_name;

  /// elastic energy names
  std::vector<std::string> _E_el_names;

  /// normalized elastic energy for each damage field
  std::vector<MaterialProperty<Real> *> _E_el;

  /// derivatives of elastic energy W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _dE_el_dd;

  /// second derivatives of elastic energy W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _d2E_el_dd2;

  /// mobility values for each damage field
  std::vector<const MaterialProperty<Real> *> _M;
};

#endif // SmallStrainElasticDegradedStress_H
