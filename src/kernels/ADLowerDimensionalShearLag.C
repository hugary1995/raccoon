//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADLowerDimensionalShearLag.h"

registerMooseObject("raccoonApp", ADLowerDimensionalShearLag);

InputParameters
ADLowerDimensionalShearLag::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("The in-plane shear lag term");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<MaterialPropertyName>("shear_lag_coefficient", "s", "The shear lag coefficient");
  params.addRequiredRangeCheckedParam<unsigned int>("component",
                                                    "component < 3",
                                                    "An integer corresponding to the direction "
                                                    "the variable this kernel acts in. (0 for x, "
                                                    "1 for y, 2 for z)");

  return params;
}

ADLowerDimensionalShearLag::ADLowerDimensionalShearLag(const InputParameters & parameters)
  : ADKernelValue(parameters),
    BaseNameInterface(parameters),
    _disps(adCoupledValues("displacements")),
    _s(getMaterialProperty<Real>(prependBaseName("shear_lag_coefficient", true))),
    _component(getParam<unsigned int>("component"))
{
}

ADReal
ADLowerDimensionalShearLag::precomputeQpResidual()
{
  // Compute coordinate transformation
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  RealVectorValue nt(_q_point[_qp](0), _q_point[_qp](1), 0);
  nt /= nt.norm();
  RealVectorValue nz(0, 0, 1);
  RealVectorValue n = nz.cross(nt);
  Real cost = nt * nz;
  Real sint = std::sqrt(1 - cost * cost);
  RankTwoTensor nxn, offn;
  nxn.vectorOuterProduct(n, n);
  offn.fillFromInputVector({0, -n(2), n(1), n(2), 0, -n(0), -n(1), n(0), 0});
  RankTwoTensor Q = I2 * cost;
  Q += nxn * (1 - cost) + offn * sint;

  ADRealVectorValue u((*_disps[0])[_qp], (*_disps[1])[_qp], (*_disps[2])[_qp]);
  ADRealVectorValue up = Q * u;
  up(2) = 0;
  u = Q.transpose() * up;
  return _s[_qp] * u(_component);
}
