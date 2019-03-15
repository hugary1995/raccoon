#ifndef GreenStrain_H
#define GreenStrain_H

#include "ComputeStrainBase.h"

class GreenStrain;

template <>
InputParameters validParams<GreenStrain>();

class GreenStrain : public ComputeStrainBase
{
public:
  GreenStrain(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  MaterialProperty<RankTwoTensor> & _F;
};

#endif // GreenStrain_H
