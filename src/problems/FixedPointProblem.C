//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FixedPointProblem.h"

registerMooseObject("raccoonApp", FixedPointProblem);

InputParameters
FixedPointProblem::validParams()
{
  InputParameters params = FEProblem::validParams();
  return params;
}

FixedPointProblem::FixedPointProblem(const InputParameters & params) : FEProblem(params) {}

void
FixedPointProblem::betweenIterations()
{
  if (_material_props.hasStatefulProperties())
    _material_props.shift(*this);

  if (_bnd_material_props.hasStatefulProperties())
    _bnd_material_props.shift(*this);

  if (_neighbor_material_props.hasStatefulProperties())
    _neighbor_material_props.shift(*this);
}
