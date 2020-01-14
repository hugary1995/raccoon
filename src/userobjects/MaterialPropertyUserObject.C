//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "MaterialPropertyUserObject.h"
#include "libmesh/quadrature.h"

registerMooseObject("raccoonApp", MaterialPropertyUserObject);

defineLegacyParams(MaterialPropertyUserObject);

InputParameters
MaterialPropertyUserObject::validParams()
{
  InputParameters params = ElementUserObject::validParams();
  params.addRequiredParam<MaterialPropertyName>(
      "mat_prop", "the name of the material property we are going to read from");

  return params;
}

MaterialPropertyUserObject::MaterialPropertyUserObject(const InputParameters & parameters)
  : ElementUserObject(parameters), _qp(0), _from(getMaterialProperty<Real>("mat_prop"))
{
}

void
MaterialPropertyUserObject::initialize()
{
  _to.clear();
  _to.resize(_subproblem.mesh().getMesh().max_elem_id());
}

void
MaterialPropertyUserObject::threadJoin(const UserObject & y)
{
  const MaterialPropertyUserObject & mat_uo = static_cast<const MaterialPropertyUserObject &>(y);
  for (unsigned int i = 0; i < _to.size(); i++)
    for (_qp = 0; _qp < _to[0].size(); _qp++)
      _to[i][_qp] += mat_uo._to[i][_qp];
}

void
MaterialPropertyUserObject::computeProperties()
{
  if (_to[_current_elem->id()].empty())
    _to[_current_elem->id()].resize(_qrule->n_points());

  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    _to[_current_elem->id()][_qp] = _from[_qp];
}
