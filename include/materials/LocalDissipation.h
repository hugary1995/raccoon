#ifndef LocalDissipation_H
#define LocalDissipation_H

#include "DerivativeParsedMaterial.h"

// Forward Declarations
class LocalDissipation;

template <>
InputParameters validParams<LocalDissipation>();

/**
 * computes local dissipation associated a single damage field, and
 * automatically computes its derivative
 */
class LocalDissipation : public DerivativeParsedMaterial
{
public:
  LocalDissipation(const InputParameters & parameters);
};

#endif // LocalDissipation_H
