//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "FractureMarkerBase.h"
#include "CoupleableMooseVariableDependencyIntermediateInterface.h"
#include "DerivativeMaterialInterface.h"

class FractureMarker;

template <>
InputParameters validParams<FractureMarker>();

class FractureMarker : public DerivativeMaterialInterface<FractureMarkerBase>,
                       public CoupleableMooseVariableDependencyIntermediateInterface
{
public:
  FractureMarker(const InputParameters & parameters);

protected:
  virtual MarkerValue computeQpMarker();

  /// Number of damage variables
  const unsigned int _ndamage;

  /// derivatives of elastic energy W.R.T. each damage field
  std::vector<const MaterialProperty<Real> *> _dE_el_dd;

  /// derivatives of local dissipation energy W.R.T. each damage field
  std::vector<const MaterialProperty<Real> *> _dw_dd;
};

