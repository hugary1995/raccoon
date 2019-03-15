#ifndef PiolaKirchhoffStressDivergence_H
#define PiolaKirchhoffStressDivergence_H

#include "Kernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

// Forward Declarations
class PiolaKirchhoffStressDivergence;

template <>
InputParameters validParams<PiolaKirchhoffStressDivergence>();

class PiolaKirchhoffStressDivergence : public Kernel
{
public:
  PiolaKirchhoffStressDivergence(const InputParameters & parameters);

protected:
  virtual void initialSetup() override;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  std::string _base_name;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _F;
  const MaterialProperty<RankFourTensor> & _Jacobian_mult;

  const unsigned int _component;

  /// Coupled displacement variables
  unsigned int _ndisp;
  std::vector<unsigned int> _disp_var;
};

#endif // PiolaKirchhoffStressDivergence_H
