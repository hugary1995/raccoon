//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeEigenstrainFromScalarInitialStress.h"
#include "RankTwoTensor.h"

registerMooseObject("raccoonApp", ComputeEigenstrainFromScalarInitialStress);

InputParameters
ComputeEigenstrainFromScalarInitialStress::validParams()
{
  InputParameters params = ComputeEigenstrainBase::validParams();
  params.addClassDescription("Computes an eigenstrain from an initial stress");
  params.addCoupledVar("initial_stress_xx", 0, "initial stress xx compoent");
  params.addCoupledVar("initial_stress_xy", 0, "initial stress xy compoent");
  params.addCoupledVar("initial_stress_xz", 0, "initial stress xz compoent");
  params.addCoupledVar("initial_stress_yx", 0, "initial stress yx compoent");
  params.addCoupledVar("initial_stress_yy", 0, "initial stress yy compoent");
  params.addCoupledVar("initial_stress_yz", 0, "initial stress yz compoent");
  params.addCoupledVar("initial_stress_zx", 0, "initial stress zx compoent");
  params.addCoupledVar("initial_stress_zy", 0, "initial stress zy compoent");
  params.addCoupledVar("initial_stress_zz", 0, "initial stress zz compoent");
  params.addParam<std::string>("base_name",
                               "The base_name for the elasticity tensor that will be "
                               "used to compute strain from stress.  Do not provide "
                               "any base_name if your elasticity tensor does not use "
                               "one.");
  return params;
}

ComputeEigenstrainFromScalarInitialStress::ComputeEigenstrainFromScalarInitialStress(
    const InputParameters & parameters)
  : ComputeEigenstrainBase(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor")),
    _sigma00(coupledScalarValue("initial_stress_xx")),
    _sigma01(coupledScalarValue("initial_stress_xy")),
    _sigma02(coupledScalarValue("initial_stress_xz")),
    _sigma10(coupledScalarValue("initial_stress_yx")),
    _sigma11(coupledScalarValue("initial_stress_yy")),
    _sigma12(coupledScalarValue("initial_stress_yz")),
    _sigma20(coupledScalarValue("initial_stress_zx")),
    _sigma21(coupledScalarValue("initial_stress_zy")),
    _sigma22(coupledScalarValue("initial_stress_zz"))
{
}

void
ComputeEigenstrainFromScalarInitialStress::computeQpEigenstrain()
{
  RankTwoTensor initial_stress(_sigma00[0],
                               _sigma10[0],
                               _sigma20[0],
                               _sigma01[0],
                               _sigma11[0],
                               _sigma21[0],
                               _sigma02[0],
                               _sigma12[0],
                               _sigma22[0]);

  _eigenstrain[_qp] = -_elasticity_tensor[_qp].invSymm() * initial_stress;
}
