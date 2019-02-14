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

  Function & _Gc;
  Function & _L;
  Real _w_norm;
  MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _kappa_old;
  MaterialProperty<Real> & _M;
  const MaterialProperty<Real> & _M_old;
};

#endif // FractureMaterial_H
