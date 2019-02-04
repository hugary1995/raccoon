#include "Degradation.h"

registerMooseObject("raccoonApp", Degradation);

template <>
InputParameters
validParams<Degradation>()
{
  InputParameters params = validParams<DerivativeParsedMaterial>();
  params.addClassDescription(
      "Parse degradation function and its derivatives for a single damage field");
  params.set<unsigned int>("derivative_order") = 2;
  return params;
}

Degradation::Degradation(const InputParameters & parameters) : DerivativeParsedMaterial(parameters)
{
  if (_nargs != 1)
    mooseError(name(),
               ": Degradation should depend on a single damage field. Use LumpedDegradation to "
               "combine lump multiple degradation functions.");
}
