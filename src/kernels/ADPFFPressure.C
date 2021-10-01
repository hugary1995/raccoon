//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFPressure.h"

registerMooseObject("raccoonApp", ADPFFPressure);

InputParameters
ADPFFPressure::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the pressure term in the phase-field evolution equation for pressurized "
      "crack. The weak form is $\\grad w, p \\bfu$.");

  params.addRequiredParam<MaterialPropertyName>("pressure", "Material property name for pressure");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<MaterialPropertyName>("indicator_function",
                                        "I"
                                        "The indicator function");
  return params;
}

ADPFFPressure::ADPFFPressure(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _ndisp(coupledComponents("displacements")),
    _disp(3),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("indicator_function"), _var.name())))
{
  // fetch coupled variables and gradients (as stateful properties if necessary)
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &adCoupledValue("displacements", i);
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp[i] = &_ad_zero;
}

ADRealVectorValue
ADPFFPressure::precomputeQpResidual()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  return _p[_qp] * u * _dI_dd[_qp];
}
