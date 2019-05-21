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

  /// slope of the local dissipation function when d = 1
  Function & _dw_dd;

  /// slope of the degradation function when d = 1
  Function & _dg_dd;

  /// Mobility
  const MaterialProperty<Real> & _M;

  /// computed fracture energy barrier
  MaterialProperty<Real> & _b;
};

#endif // FractureEnergyBarrier_H
