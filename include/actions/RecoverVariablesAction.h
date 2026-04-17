#include "Action.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include "MooseEnum.h"
#include <unordered_map>
#include "Qp_Mapping.h"

class RecoverVariablesAction : public Action

{
public:
  static InputParameters validParams();

  RecoverVariablesAction(const InputParameters & params);

  virtual void act() override;

protected:
  std::vector<MaterialName> _tensor_materials;

  std::vector<MaterialName> _materials;

  std::string _output_name;

  unsigned int _qpnum;

  QpMapping::Element _element;

private:
  const std::unordered_map<int, int> * _lookup;
};
