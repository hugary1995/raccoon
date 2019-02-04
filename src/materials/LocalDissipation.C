#include "LocalDissipation.h"

registerMooseObject("raccoonApp", LocalDissipation);

template <>
InputParameters
validParams<LocalDissipation>()
{
  InputParameters params = validParams<DerivativeParsedMaterial>();
  params.addClassDescription(
      "Parse LocalDissipation function and its derivatives for a single damage field");
  params.set<unsigned int>("derivative_order") = 2;
  return params;
}

LocalDissipation::LocalDissipation(const InputParameters & parameters)
  : DerivativeParsedMaterial(parameters)
{
  if (_nargs != 1)
    mooseError(name(), ": LocalDissipation should depend on a single damage field.");
}
