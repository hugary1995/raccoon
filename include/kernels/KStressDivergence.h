//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ALEKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

// Forward Declarations
class KStressDivergence;

template <>
InputParameters validParams<KStressDivergence>();

class KStressDivergence : public ALEKernel
{
public:
  KStressDivergence(const InputParameters & parameters);

protected:
  virtual void initialSetup() override;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  virtual Real computeQpJacobianFromGeometricNonliearity();
  virtual Real computeQpJacobianFromMaterialStiffness(unsigned int i, unsigned int k);
  virtual Real computeQpJacobianFromDamage(RankTwoTensor dstress_dd);
  virtual RankFourTensor pullBack(const RankFourTensor & c);

  /// Material property base name
  std::string _base_name;

  /// Stress calculated by the stress calculator
  const MaterialProperty<RankTwoTensor> & _stress;

  /// Whether this is large deformation problem
  const bool _large_deformation;

  /// Pointer to the deformation gradient
  const MaterialProperty<RankTwoTensor> * _F;

  /// dstress_dstrain
  const MaterialProperty<RankFourTensor> & _Jacobian_mult;

  /// Component of the displacement vector
  const unsigned int _component;

  /// Number of coupled displacement variables
  unsigned int _ndisp;

  /// Coupled displacement variables
  std::vector<unsigned int> _disp_var;

  /// Number of coupled damage variables
  unsigned int _ndamage;

  /// Coupled damage variables
  std::vector<unsigned int> _d_var;

  /// dstress_ddi
  std::vector<const MaterialProperty<RankTwoTensor> *> _dstress_dd;
};

