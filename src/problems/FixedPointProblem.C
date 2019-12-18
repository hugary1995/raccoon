//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

// #include "FixedPointProblem.h"
//
// registerMooseObject("raccoonApp", FixedPointProblem);
//
// InputParameters
// FixedPointProblem::validParams()
// {
//   InputParameters params = FEProblem::validParams();
//   return params;
// }
//
// FixedPointProblem::FixedPointProblem(const InputParameters & params) : FEProblem(params) {}
//
// void
// FixedPointProblem::betweenIterations()
// {
//   if (_material_props.hasStatefulProperties())
//     _material_props.shift(*this);
//
//   if (_bnd_material_props.hasStatefulProperties())
//     _bnd_material_props.shift(*this);
//
//   if (_neighbor_material_props.hasStatefulProperties())
//     _neighbor_material_props.shift(*this);
// }

#include "FixedPointProblem.h"

#include "NonlinearSystemBase.h"

registerMooseObject("raccoonApp", FixedPointProblem);

InputParameters
FixedPointProblem::validParams()
{
  InputParameters params = FEProblem::validParams();
  params.addParam<TagName>("fp_tag_name", "fp_previous", "Tag name for the fixed point iteration");
  return params;
}

FixedPointProblem::FixedPointProblem(const InputParameters & params)
  : FEProblem(params),
    _tag_previous(getParam<TagName>("fp_tag_name")),
    _tag_id(addVectorTag(_tag_previous)),
    _residual_previous(_nl->addVector(_tag_id, false, GHOSTED))
{
}

void
FixedPointProblem::computeResidual(const NumericVector<Number> & soln,
                                   NumericVector<Number> & residual)
{
  // excluding the previous tag evaluation
  _nl->disassociateVectorFromTag(_residual_previous, _tag_id);

  auto & tags = getVectorTags();

  _fe_vector_tags.clear();

  for (auto & tag : tags)
    if (tag.second != _tag_id)
      _fe_vector_tags.insert(tag.second);

  computeResidualInternal(soln, residual, _fe_vector_tags);

  residual += _residual_previous;

  _nl->associateVectorToTag(_residual_previous, _tag_id);
}

void
FixedPointProblem::computeFullResidual(const NumericVector<Number> & soln,
                                       NumericVector<Number> & residual)
{
  FEProblem::computeResidual(soln, residual);
  residual += _residual_previous;
}
