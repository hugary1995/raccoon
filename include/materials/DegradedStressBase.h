#ifndef DegradedStressBase_H
#define DegradedStressBase_H

#include "ComputeStressBase.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

// Forward Declarations
class DegradedStressBase;

template <>
InputParameters validParams<DegradedStressBase>();

class DegradedStressBase : public ComputeStressBase
{
public:
  DegradedStressBase(const InputParameters & parameters);

protected:
  /// the Macaulay bracket operation
  virtual Real Macaulay(Real);

  /// derivative of the Macaulay bracket operator W.R.T. the operand
  virtual Real dMacaulay(Real);

  /// spectral decomposition
  virtual RankFourTensor spectralDecomposition(const RankTwoTensor, RankTwoTensor &);

  /// number of damage fields
  const unsigned int _num_fields;

  /// name of the material that holds the lumped degradation function
  std::string _g_name;

  /// the lumped degradation function
  const MaterialProperty<Real> & _g;

  /// whether to keep track of the maximum strain energy to enforce irreversibility
  const bool _history;

  /// names of the coupled damage fields
  std::vector<std::string> _var_names;

  /// Derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _dg_dd;

  /// Second derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _d2g_dd2;

  /// Derivatives of the stress W.R.T. each damage field
  std::vector<MaterialProperty<RankTwoTensor> *> _dstress_dd;

  // elasticity tensor C_ijkl
  // We have to get the elasticity tensor in every derived stress calculator due to an annoying
  // change in MOOSE framework
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;
};

#endif // DegradedStressBase_H
