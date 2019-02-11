#ifndef FractureEnergyBarrier_H
#define FractureEnergyBarrier_H

#include "Material.h"

class FractureEnergyBarrier;

template <>
InputParameters validParams<FractureEnergyBarrier>();

class FractureEnergyBarrier : public Material
{
public:
  FractureEnergyBarrier(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const std::string _var_name;
  const Real _dw_dd;
  const Real _dg_dd;
  const MaterialProperty<Real> & _M;
  MaterialProperty<Real> & _E_crit;
};

#endif // FractureEnergyBarrier_H
