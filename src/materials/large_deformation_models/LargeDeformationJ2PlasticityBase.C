//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationJ2PlasticityBase.h"

InputParameters
LargeDeformationJ2PlasticityBase::validParams()
{
  InputParameters params = LargeDeformationPlasticityModel::validParams();
  params.addParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  return params;
}

LargeDeformationJ2PlasticityBase::LargeDeformationJ2PlasticityBase(
    const InputParameters & parameters)
  : LargeDeformationPlasticityModel(parameters),
    _element(QpMapping::Element::HEX8_3rd),
    _qpnum(0),
    _lookup(nullptr)
{
  if (_recover)
  {
    if (!isParamSetByUser("element"))
      mooseError("'element' must be specified when recover = true");
    _element = getParam<MooseEnum>("element").getEnum<QpMapping::Element>();
    _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/true);
  }
}

std::string
LargeDeformationJ2PlasticityBase::formatQP(unsigned int qp) const
{
  if (_qpnum < 10)
    return std::to_string(qp);
  return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp);
}

void
LargeDeformationJ2PlasticityBase::initQpStatefulProperties()
{
  _Fp[_qp].setToIdentity();
  _ep[_qp] = 0;

  if (!_recover)
    return;

  const std::vector<std::string> indices = {"x", "y", "z"};
  unsigned int qp_sel = QpMapping::getQP(_qp + 1, _lookup);

  _ep[_qp] = _solution_object_ptr->pointValue(
      _t, _current_elem->true_centroid(), "effective_plastic_strain_" + formatQP(qp_sel), nullptr);
  if (_ep[_qp] < 0)
    _ep[_qp] = 0;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      _Fp[_qp](i, j) = _solution_object_ptr->pointValue(
          _t,
          _current_elem->true_centroid(),
          "plastic_deformation_gradie_" + indices[i] + indices[j] + "_" + formatQP(qp_sel),
          nullptr);
}
