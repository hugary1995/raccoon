//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFSource.h"

registerMooseObject("raccoonApp", ADPFFSource);

InputParameters
ADPFFSource::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("The source term in the phase-field evolution equation. The weak form "
                             "is $(w, \\dfrac{\\partial \\psi}{\\partial d})$.");
  params.addParam<MaterialPropertyName>("free_energy",
                                        "psi"
                                        "The Helmholtz free energy");
  return params;
}

ADPFFSource::ADPFFSource(const InputParameters & parameters)
  : ADKernelValue(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _psi_name(getParam<MaterialPropertyName>("free_energy")),
    _dpsi_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(_psi_name, _var.name())))
{
}

ADReal
ADPFFSource::precomputeQpResidual()
{
  return _dpsi_dd[_qp];
}
