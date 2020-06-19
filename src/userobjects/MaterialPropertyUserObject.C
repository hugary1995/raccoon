//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "MaterialPropertyUserObject.h"
#include "libmesh/quadrature.h"

#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", MaterialPropertyUserObject);
registerMooseObject("raccoonApp", ADMaterialPropertyUserObject);

template <bool is_ad>
InputParameters
MaterialPropertyUserObjectTempl<is_ad>::validParams()
{
  InputParameters params = ElementUserObject::validParams();
  params.addClassDescription("store a material property");
  params.addRequiredParam<MaterialPropertyName>(
      "mat_prop", "the name of the material property we are going to read from");
  return params;
}

template <bool is_ad>
MaterialPropertyUserObjectTempl<is_ad>::MaterialPropertyUserObjectTempl(
    const InputParameters & parameters)
  : ElementUserObject(parameters),
    _qp(0),
    _from(getGenericMaterialProperty<Real, is_ad>("mat_prop"))
{
}

template <bool is_ad>
void
MaterialPropertyUserObjectTempl<is_ad>::initialSetup()
{
  meshChanged();
}

template <bool is_ad>
void
MaterialPropertyUserObjectTempl<is_ad>::meshChanged()
{
  _to.resize(_subproblem.mesh().getMesh().max_elem_id());
  for (unsigned int i = 0; i < _to.size(); i++)
    _to[i].resize(_fe_problem.getMaxQps());
}

template <bool is_ad>
void
MaterialPropertyUserObjectTempl<is_ad>::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    _to[_current_elem->id()][_qp] = _from[_qp];
}

template class MaterialPropertyUserObjectTempl<false>;
template class MaterialPropertyUserObjectTempl<true>;
