//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPressurizedCrack.h"

registerMooseObject("raccoonApp", ADPressurizedCrack);

InputParameters
ADPressurizedCrack::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the body force term from pressurized phase-field "
                             "fracture. The weak form is $(w, p \\grad d)$.");

  params.addParam<MaterialPropertyName>("pressure", "pressure inside the crack");
  params.addRequiredCoupledVar("phase_field", "coupled damage variable");
  params.addRequiredParam<MaterialPropertyName>("indicator_function",
                                                "I"
                                                "The pressure indicator function");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  return params;
}

ADPressurizedCrack::ADPressurizedCrack(const InputParameters & parameters)
  : ADKernelValue(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _comp(getParam<unsigned int>("component")),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _grad_d(adCoupledGradient("phase_field")),
    _d_name(getVar("phase_field", 0)->name()),
    _I_name(prependBaseName("indicator_function", true)),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(_I_name, _d_name)))
{
}

ADReal
ADPressurizedCrack::precomputeQpResidual()
{
  return _p[_qp] * _grad_d[_qp](_comp) * _dI_dd[_qp];
}
