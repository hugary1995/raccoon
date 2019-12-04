//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureSurfaceArea.h"

registerMooseObject("raccoonApp", FractureSurfaceArea);

template <>
InputParameters
validParams<FractureSurfaceArea>()
{
  InputParameters params = validParams<FractureEnergy>();
  params.addParam<MaterialPropertyName>("energy_release_rate_name",
                                        "energy_release_rate",
                                        "name of the material that holds the energy release rate");
  return params;
}

FractureSurfaceArea::FractureSurfaceArea(const InputParameters & parameters)
  : FractureEnergy(parameters), _Gc(getMaterialProperty<Real>("energy_release_rate_name"))
{
}

Real
FractureSurfaceArea::computeQpIntegral()
{
  return FractureEnergy::computeQpIntegral() / _Gc[_qp];
}
