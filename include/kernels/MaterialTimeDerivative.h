#ifndef MaterialTimeDerivative_H
#define MaterialTimeDerivative_H

#include "TimeDerivative.h"

// Forward Declaration
class MaterialTimeDerivative;

template <>
InputParameters validParams<MaterialTimeDerivative>();

class MaterialTimeDerivative : public TimeDerivative
{
public:
  MaterialTimeDerivative(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const MaterialProperty<Real> & _prop;
};

#endif // MaterialTimeDerivative_H
