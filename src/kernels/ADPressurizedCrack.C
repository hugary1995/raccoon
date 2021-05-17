//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPressurizedCrack.h"

registerMooseObject("raccoonApp", ADPressurizedCrack);

InputParameters
ADPressurizedCrack::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("This class computes the body force term from pressurized phase-field "
                             "fracture. The weak form is $(w, p \\grad d)$.");
  params.addParam<MaterialPropertyName>("pressure_mat", "pressure inside the crack");
  params.addCoupledVar("pressure_var", "pressure inside the crack");
  params.addRequiredCoupledVar("d", "coupled damage variable");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  return params;
}

ADPressurizedCrack::ADPressurizedCrack(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _comp(getParam<unsigned int>("component")),
    _p_mat(isParamValid("pressure_mat") ? &getADMaterialProperty<Real>("pressure_mat") : nullptr),
    _p_var(isParamValid("pressure_var") ? &adCoupledValue("pressure_var") : nullptr),
    _grad_d(adCoupledGradient("d"))
{
  if (!_p_mat && !_p_var)
    mooseError(name() +
               ": pressure should be provided by pressure_mat or pressure_var, none provided.");
  if (_p_mat && _p_var)
    mooseError(name() +
               ": pressure should be provided by either pressure_mat or pressure_var, not both.");
}

ADReal
ADPressurizedCrack::precomputeQpResidual()
{
  ADReal p = _p_mat ? (*_p_mat)[_qp] : (*_p_var)[_qp];
  return p * _grad_d[_qp](_comp);
}
