#ifndef StationaryGenericFunctionMaterial_H
#define StationaryGenericFunctionMaterial_H

#include "Material.h"

// Forward Declarations
class StationaryGenericFunctionMaterial;

template <>
InputParameters validParams<StationaryGenericFunctionMaterial>();

class StationaryGenericFunctionMaterial : public Material
{
public:
  StationaryGenericFunctionMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  std::vector<std::string> _prop_names;
  std::vector<FunctionName> _prop_values;

  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _properties;
  std::vector<const MaterialProperty<Real> *> _properties_old;
  std::vector<Function *> _functions;
};

#endif // StationaryGenericFunctionMaterial_H
