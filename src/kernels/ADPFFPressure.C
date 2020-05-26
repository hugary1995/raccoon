//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFPressure.h"

registerADMooseObject("raccoonApp", ADPFFPressure);

InputParameters
ADPFFPressure::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.addClassDescription("computes the pressure term in phase-field evolution equation");
  params.addRequiredParam<UserObjectName>("pressure_uo",
                                          "userobject that has pressure values at qps");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  return params;
}

ADPFFPressure::ADPFFPressure(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    _p_uo(getUserObject<ADMaterialPropertyUserObject>("pressure_uo")),
    _ndisp(coupledComponents("displacements")),
    _disp(3)
{
  // fetch coupled variables and gradients (as stateful properties if necessary)
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &adCoupledValue("displacements", i);
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp[i] = &adZeroValue();
}

ADRealVectorValue
ADPFFPressure::precomputeQpResidual()
{
  ADReal p = _p_uo.getData(_current_elem, _qp);
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  return p * u;
}
