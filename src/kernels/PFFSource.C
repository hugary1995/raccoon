//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PFFSource.h"

registerMooseObject("raccoonApp", PFFSource);

InputParameters
PFFSource::validParams()
{
  InputParameters params = DerivativeMaterialInterface<KernelValue>::validParams();
  params.addClassDescription("The source term in the phase-field evolution equation. The weak form "
                             "is $(w, \\dfrac{\\partial \\psi}{\\partial d})$.");
  params.addParam<MaterialPropertyName>("free_energy",
                                        "psi"
                                        "The Helmholtz free energy");
  return params;
}

PFFSource::PFFSource(const InputParameters & parameters)
  : DerivativeMaterialInterface<KernelValue>(parameters),
    _psi_name(getParam<MaterialPropertyName>("free_energy")),
    _dpsi_dd(getMaterialPropertyDerivative<Real>(_psi_name, _var.name())),
    _d2psi_dd2(getMaterialPropertyDerivative<Real>(_psi_name, _var.name(), _var.name()))
{
}

Real
PFFSource::precomputeQpResidual()
{
  return _dpsi_dd[_qp];
}

Real
PFFSource::precomputeQpJacobian()
{
  return _d2psi_dd2[_qp] * _phi[_j][_qp];
}
