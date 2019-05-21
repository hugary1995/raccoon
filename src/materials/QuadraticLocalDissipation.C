//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticLocalDissipation.h"

registerADMooseObject("raccoonApp", QuadraticLocalDissipation);

defineADValidParams(QuadraticLocalDissipation,
                    ADMaterial,
                    params.addRequiredCoupledVar("d", "phase-field damage variable");
                    params.addParam<MaterialPropertyName>(
                        "local_dissipation_name",
                        "w",
                        "name of the material that holds the local dissipation"););

template <ComputeStage compute_stage>
QuadraticLocalDissipation<compute_stage>::QuadraticLocalDissipation(
    const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _d(adCoupledValue("d")),
    _w_name(adGetParam<MaterialPropertyName>("local_dissipation_name")),
    _w(adDeclareADProperty<Real>(_w_name)),
    _dw_dd(adDeclareADProperty<Real>(
        derivativePropertyNameFirst(_w_name, this->getVar("d", 0)->name())))
{
}

template <ComputeStage compute_stage>
void
QuadraticLocalDissipation<compute_stage>::computeQpProperties()
{
  ADReal d = _d[_qp];

  _w[_qp] = d * d;
  _dw_dd[_qp] = d;
}
