//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PFFSource.h"

registerMooseObject("raccoonApp", PFFSource);

InputParameters
PFFSource::validParams()
{
  InputParameters params = KernelValue::validParams();
  params.addClassDescription(
      "The source term in the phase-field evolution equation. The weak form "
      "is $(w, \\dfrac{\\partial \\psi}{\\partial d})$. To be used with NEML2");
  params.addParam<MaterialPropertyName>("dpsi_dd",
                                        "dpsi_dd"
                                        "1st derivatrive of free energy w.r.t. phase-field");
  params.addParam<MaterialPropertyName>("d2psi_dd2",
                                        "d2psi_dd2"
                                        "2nd derivatrive of free energy w.r.t. phase-field");
  return params;
}

PFFSource::PFFSource(const InputParameters & parameters)
  : KernelValue(parameters),
    _dpsi_dd(getMaterialProperty<Real>("dpsi_dd")),
    _d2psi_dd2(getMaterialProperty<Real>("d2psi_dd2"))
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
