#ifndef BrittleFractureMaterial_H
#define BrittleFractureMaterial_H

#include "Material.h"
#include "Function.h"

class BrittleFractureMaterial;

template <>
InputParameters validParams<BrittleFractureMaterial>();

class BrittleFractureMaterial : public Material
{
public:
  BrittleFractureMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  Function & _Gc;
  Function & _L;
  Real _w_norm;
  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _mobility;
};

#endif // BrittleFractureMaterial_H
