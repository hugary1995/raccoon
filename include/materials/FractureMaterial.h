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
  virtual void computeQpProperties() override;

  Function & _Gc;
  Function & _L;
  Real _w_norm;
  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _mobility;
};

#endif // FractureMaterial_H
