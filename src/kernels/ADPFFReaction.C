//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFReaction.h"

registerADMooseObject("raccoonApp", ADPFFReaction);

InputParameters
ADPFFReaction::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("computes the reaction term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>("degradation_name", "g", "name of degradation");
  params.addParam<MaterialPropertyName>("driving_energy_mat",
                                        "material property name of the driving energy");
  params.addCoupledVar("driving_energy_var", "auxiliary variable that holds the driving energy");
  params.addParam<UserObjectName>("driving_energy_uo",
                                  "userobject that has driving energy values at qps");
  params.addParam<bool>(
      "lag", false, "whether we should use last step's driving energy to improve convergence");
  return params;
}

ADPFFReaction::ADPFFReaction(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("degradation_name"), _var.name()))),
    _lag(getParam<bool>("lag")),
    _D_mat(isParamValid("driving_energy_mat") && !_lag
               ? &getADMaterialProperty<Real>("driving_energy_mat")
               : nullptr),
    _D_mat_old(isParamValid("driving_energy_mat") && _lag
                   ? &getMaterialPropertyOld<Real>("driving_energy_mat")
                   : nullptr),
    _D_var(isParamValid("driving_energy_var") && !_lag ? &adCoupledValue("driving_energy_var")
                                                       : nullptr),
    _D_var_old(isParamValid("driving_energy_var") && _lag ? &coupledValueOld("driving_energy_var")
                                                          : nullptr),
    _D_uo(isParamValid("driving_energy_uo")
              ? &getUserObject<ADMaterialPropertyUserObject>("driving_energy_uo")
              : nullptr)
{
  bool provided_by_mat = _D_mat || _D_mat_old;
  bool provided_by_var = _D_var || _D_var_old;
  bool provided_by_uo = _D_uo;

  /// driving energy should be provided
  if (!provided_by_mat && !provided_by_var && !provided_by_uo)
    mooseError("no driving energy provided.");

  /// driving energy should not be multiply defined
  if ((provided_by_mat ? 1 : 0) + (provided_by_var ? 1 : 0) + (provided_by_uo ? 1 : 0) > 1)
    mooseError("driving energy multiply defined.");
}

ADReal
ADPFFReaction::precomputeQpResidual()
{
  ADReal D;
  if (_D_var)
    D = (*_D_var)[_qp];
  else if (_D_mat)
    D = (*_D_mat)[_qp];
  else if (_D_mat_old)
    D = (*_D_mat_old)[_qp];
  else if (_D_uo)
    D = _D_uo->getData(_current_elem, _qp);
  else
    mooseError("Internal Error");

  // reaction like driving force
  return _dg_dd[_qp] * D;
}
