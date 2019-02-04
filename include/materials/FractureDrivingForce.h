#ifndef FractureDrivingForce_H
#define FractureDrivingForce_H

#include "Material.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class FractureDrivingForce;

template <>
InputParameters validParams<FractureDrivingForce>();

class FractureDrivingForce : public DerivativeMaterialInterface<Material>
{
public:
  FractureDrivingForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// number of coupled damage fields
  const unsigned int _num_fields;

  /// names of the local dissipation energy
  std::vector<std::string> _w_names;

  /// base name of the driving force
  std::string _D_name;

  /// elastic energy name
  MaterialPropertyName _E_el_name;

  /// elastic energy
  const MaterialProperty<Real> & _E_el;

  /// base name of mobility
  MaterialPropertyName _mobility_base_name;

  /// names of the coupled damage fields
  std::vector<std::string> _var_names;

  /// mobility values for each damage field
  std::vector<const MaterialProperty<Real> *> _mobility;

  /// values of the driving force for each damage field
  std::vector<MaterialProperty<Real> *> _D;

  /// Derivative of the driving force W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _dD_dd;

  /// Second derivative of the driving force W.R.T. each damage field
  std::vector<MaterialProperty<Real> *> _d2D_dd2;

  /// values of the local dissipation
  std::vector<const MaterialProperty<Real> *> _w;

  /// Derivatives of the local dissipation
  std::vector<const MaterialProperty<Real> *> _dw_dd;

  /// Second derivatives of the local dissipation
  std::vector<const MaterialProperty<Real> *> _d2w_dd2;

  /// Derivatives of the elastic energy
  std::vector<const MaterialProperty<Real> *> _dE_el_dd;

  /// Second derivatives of the elastic energy
  std::vector<const MaterialProperty<Real> *> _d2E_el_dd2;
};

#endif // FractureDrivingForce_H
