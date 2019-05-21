#ifndef FractureMaterial_H
#define FractureMaterial_H

#include "Material.h"
#include "Function.h"

class FractureMaterial;

template <>
InputParameters validParams<FractureMaterial>();

class FractureMaterial : public Material
{
public:
  FractureMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// energy release rate
  Function & _Gc;

  /// length scale in the fracture surface energy density
  Function & _L;

  /// norm of the local dissipation function
  Function & _w_norm;

  /// interface coefficient in Allen-Cahn equation
  MaterialProperty<Real> & _kappa;

  /// interface coefficient at the previous time step
  const MaterialProperty<Real> & _kappa_old;

  /// Mobility in Allen-Cahn equation
  MaterialProperty<Real> & _M;

  /// Mobility at the previous time step
  const MaterialProperty<Real> & _M_old;
};

#endif // FractureMaterial_H
