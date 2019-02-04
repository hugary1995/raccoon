#ifndef Degradation_H
#define Degradation_H

#include "DerivativeParsedMaterial.h"

// Forward Declarations
class Degradation;

template <>
InputParameters validParams<Degradation>();

/**
 * computes degradation associated a single damage field, and
 * automatically computes all its derivatives
 */
class Degradation : public DerivativeParsedMaterial
{
public:
  Degradation(const InputParameters & parameters);
};

#endif // Degradation_H
