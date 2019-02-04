#ifndef DecompositionWarehouse_H
#define DecompositionWarehouse_H

#include "ComputeStressBase.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

enum Decomposition
{
  INVALID,
  NO_DECOMP,
  STRAIN_SPECTRAL
};

// Forward Declarations
class DecompositionWarehouse;

template <>
InputParameters validParams<DecompositionWarehouse>();

class DecompositionWarehouse : public ComputeStressBase
{
public:
  DecompositionWarehouse(const InputParameters & parameters);
  static Decomposition decomposition(std::string name);

protected:
  virtual Real Macaulay(Real);
  virtual Real getQpPoissonsRatio();
  virtual Real getQpYoungsModulus();
  virtual Real noDecomp();
  virtual Real spectralStrainDecomp();
  virtual Real decomposeStressAndComputePositiveStrainEnergy();

  /// whether a Multiapp-based staggered solution scheme is used
  const bool _staggered;

  /// type of decomposition
  const Decomposition _decomposition;

  /// name of the material that holds the lumped degradation function
  std::string _g_name;

  /// the lumped degradation function
  const MaterialProperty<Real> & _g;

  /// number of damage fields
  const unsigned int _num_fields;

  /// name of the material holding elastic energy
  MaterialPropertyName _E_el_name;

  /// elastic energy
  MaterialProperty<Real> & _E_el;

  /// names of the coupled damage fields
  std::vector<std::string> _var_names;

  /// Derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _dg_dd;

  /// Second derivatives of the degradations of each damage field
  std::vector<const MaterialProperty<Real> *> _d2g_dd2;

  /// Derivatives of the stress W.R.T. each damage field
  std::vector<MaterialProperty<RankTwoTensor> *> _dstress_dd;
};

#endif // DecompositionWarehouse_H
