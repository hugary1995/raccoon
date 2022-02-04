//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE includes
#include "BaseNameInterface.h"
#include "MaterialPropertyInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class DegradationInterface : public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  DegradationInterface(const MooseObject * obj, const VariableName d_name);

private:
  // @{ Required interfaces
  const BaseNameInterface * _bn_interface;
  MaterialPropertyInterface * _mp_interface;
  // @}

protected:
  /// Name of the degradation function
  const MaterialPropertyName _g_name;

  /// The degradation function
  const ADMaterialProperty<Real> & _g;

  /// The derivative of the degradation function w.r.t. the phase field
  const ADMaterialProperty<Real> & _dg_dd;
};

InputParameters
DegradationInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addRequiredParam<MaterialPropertyName>("degradation_function", "The degradation function");
  return params;
}

DegradationInterface::DegradationInterface(const MooseObject * obj, const VariableName d_name)
  : DerivativeMaterialPropertyNameInterface(),
    _bn_interface(dynamic_cast<const BaseNameInterface *>(obj)),
    _mp_interface(const_cast<MaterialPropertyInterface *>(
        dynamic_cast<const MaterialPropertyInterface *>(obj))),
    _g_name(_bn_interface->prependBaseName("degradation_function", true)),
    _g(_mp_interface->getADMaterialProperty<Real>(_g_name)),
    _dg_dd(_mp_interface->getADMaterialProperty<Real>(derivativePropertyName(_g_name, {d_name})))
{
}
