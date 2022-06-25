#include "Advection.h"

registerMooseObject("raccoonApp", Advection);

InputParameters
Advection::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("potential", "potential of the velocity field");
  return params;
}

Advection::Advection(const InputParameters & parameters)
  : ADKernel(parameters), _v(adCoupledGradient("potential"))
{
}

ADReal
Advection::computeQpResidual()
{
  return _test[_i][_qp] * _v[_qp] * _grad_u[_qp];
}
