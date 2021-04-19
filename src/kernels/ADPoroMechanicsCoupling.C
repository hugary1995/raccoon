//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPoroMechanicsCoupling.h"

registerMooseObject("raccoonApp", ADPoroMechanicsCoupling);

InputParameters
ADPoroMechanicsCoupling::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Kernel handling coupling of porepressure to porous media through biot coefficient");
  params.addRequiredCoupledVar("porepressure", "Pore pressure");
  params.addRequiredParam<unsigned int>("component",
                                        "The gradient direction (0 for x, 1 for y and 2 for z)");
  return params;
}

ADPoroMechanicsCoupling::ADPoroMechanicsCoupling(const InputParameters & parameters)
  : ADKernel(parameters),
    _coefficient(getADMaterialProperty<Real>("biot_coefficient")),
    _porepressure(adCoupledValue("porepressure")),
    _component(getParam<unsigned int>("component"))
{
  if (_component >= _mesh.dimension())
    mooseError("PoroMechanicsCoupling: component should not be greater than the mesh dimension\n");
}

ADReal
ADPoroMechanicsCoupling::computeQpResidual()
{
  return -_coefficient[_qp] * _porepressure[_qp] * _grad_test[_i][_qp](_component);
}
