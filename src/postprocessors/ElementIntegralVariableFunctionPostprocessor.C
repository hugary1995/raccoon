// //* This file is part of the RACCOON application
// //* being developed at Dolbow lab at Duke University
// //* http://dolbow.pratt.duke.edu
//
// #include "ElementIntegralVariableFunctionPostprocessor.h"
//
// registerMooseObject("raccoonApp", ElementIntegralVariableFunctionPostprocessor);
//
// template <>
// InputParameters
// validParams<ElementIntegralVariableFunctionPostprocessor>()
// {
//   InputParameters params = validParams<ElementIntegralVariablePostprocessor>();
//   params.addRequiredParam<FunctionName>(
//       "function", "1", "the function to be multiplied with the variable");
//   return params;
// }
//
// ElementIntegralVariableFunctionPostprocessor::ElementIntegralVariableFunctionPostprocessor(
//     const InputParameters & parameters)
//   : ElementIntegralVariablePostprocessor(parameters), _function(getFunction("function"))
// {
// }
//
// Real
// ElementIntegralVariableFunctionPostprocessor::computeQpIntegral()
// {
//   return _u[_qp] * _function.value(_t, _q_point[_qp]);
// }
