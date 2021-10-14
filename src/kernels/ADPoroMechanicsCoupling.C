//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPoroMechanicsCoupling.h"

registerMooseObject("raccoonApp", ADPoroMechanicsCoupling);

InputParameters
ADPoroMechanicsCoupling::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "Kernel handling coupling of porepressure to porous media through the Biot coefficient");
  params.addRequiredCoupledVar("porepressure", "The pore pressure");
  params.addRequiredParam<unsigned int>("component",
                                        "The gradient component (0 for x, 1 for y and 2 for z)");
  params.addParam<MaterialPropertyName>(
      "biot_coefficient", "biot_coefficient", "The Biot coefficient.");
  return params;
}

ADPoroMechanicsCoupling::ADPoroMechanicsCoupling(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _coefficient(getADMaterialProperty<Real>(prependBaseName("biot_coefficient", true))),
    _porepressure(adCoupledValue("porepressure")),
    _component(getParam<unsigned int>("component"))
{
  if (_component >= _mesh.dimension())
    mooseError(
        name(), ": component is ", _component, ", but mesh dimension is ", _mesh.dimension());
}

ADReal
ADPoroMechanicsCoupling::computeQpResidual()
{
  return -_coefficient[_qp] * _porepressure[_qp] * _grad_test[_i][_qp](_component);
}
