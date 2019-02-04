#ifndef PFFRACBULKMATERIAL_H
#define PFFRACBULKMATERIAL_H

#include "Material.h"
#include "Function.h"

class PFFracBulkMaterial;

template <>
InputParameters validParams<PFFracBulkMaterial>();

class PFFracBulkMaterial : public Material
{
public:
  PFFracBulkMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  Function & _Gc;
  Function & _L;
  Real _w_norm;
  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _mobility;
};

#endif // PFFracBulkMaterial_H
