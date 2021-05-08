//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ThinFilmInterfaceEnergy.h"

registerMooseObject("raccoonApp", ThinFilmInterfaceEnergy);

InputParameters
ThinFilmInterfaceEnergy::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes the interface energy corresponding to a shear-lag model, "
                             "i.e. $\\psi = 0.5 c \\bs{u} \\cdot \\bs{u}$.");
  params.addRequiredParam<Real>(
      "coef", "The coefficient describing the mismatch between the film and the substrate");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<MaterialPropertyName>(
      "interface_energy_name", "E_int", "name of the material that holds the interface energy");

  return params;
}

ThinFilmInterfaceEnergy::ThinFilmInterfaceEnergy(const InputParameters & parameters)
  : ADMaterial(parameters),
    _E_int_name(getParam<MaterialPropertyName>("interface_energy_name")),
    _E_int(declareADProperty<Real>(_E_int_name)),
    _coef(getParam<Real>("coef")),
    _ndisp(coupledComponents("displacements")),
    _disp(3)
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &adCoupledValue("displacements", i);

  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp[i] = &adZeroValue();
}

void
ThinFilmInterfaceEnergy::computeQpProperties()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  _E_int[_qp] = 0.5 * _coef * u * u;
}
