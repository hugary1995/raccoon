//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureMarkerBase.h"
#include "FEProblem.h"
#include "Assembly.h"

#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<FractureMarkerBase>()
{
  InputParameters params = validParams<Marker>();
  params += validParams<MaterialPropertyInterface>();
  return params;
}

FractureMarkerBase::FractureMarkerBase(const InputParameters & parameters)
  : Marker(parameters),
    MaterialPropertyInterface(this, blockIDs(), Moose::EMPTY_BOUNDARY_IDS),
    _qrule(_assembly.qRule()),
    _q_point(_assembly.qPoints()),
    _qp(0)
{
}

Marker::MarkerValue
FractureMarkerBase::computeElementMarker()
{
  MarkerValue current_mark = DONT_MARK;

  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
  {
    MarkerValue new_mark = computeQpMarker();

    current_mark = std::max(current_mark, new_mark);
  }

  return current_mark;
}
