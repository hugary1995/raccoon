//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SolutionChangeNorm.h"
#include "MathUtils.h"

registerMooseObject("raccoonApp", SolutionChangeNorm);

InputParameters
SolutionChangeNorm::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription(
      "This class computes the solution change (L2) norm of selected variables.");
  params.addRequiredCoupledVar("variable",
                               "The names of the variables that this object operates on");
  return params;
}

SolutionChangeNorm::SolutionChangeNorm(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _us(coupledValues("variable")),
    _us_old(coupledValuesOld("variable"))
{
}

Real
SolutionChangeNorm::getValue() const
{
  return std::sqrt(ElementIntegralPostprocessor::getValue());
}

Real
SolutionChangeNorm::computeQpIntegral()
{
  Real value = 0;
  for (unsigned int i = 0; i < _us.size(); i++)
    value += MathUtils::pow((*_us[i])[_qp] - (*_us_old[i])[_qp], 2);
  return value;
}
