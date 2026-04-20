//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"
#include "MaterialProperty.h"
#include "MooseArray.h"
#include "SolutionUserObject.h"
#include "Qp_Mapping.h"
/**
 * This class computes the deformation gradient
 */
class ComputeDeformationGradient : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeDeformationGradient(const InputParameters & parameters);

  void initialSetup() override;

  void computeProperties() override;

  // void computeQpProperties() override;

  void initStatefulProperties(unsigned int n_points) override;

protected:
  // virtual void initQpStatefulProperties() override;

  virtual void displacementIntegrityCheck();

  virtual ADReal computeQpOutOfPlaneGradDisp();

  /// The coordinate system
  const Moose::CoordinateSystemType & _coord_sys;

  /// Coupled displacement variables
  const unsigned int _ndisp;

  /// Displacement variables
  std::vector<const ADVariableValue *> _disp;

  /// Gradient of displacements
  std::vector<const ADVariableGradient *> _grad_disp;

  /// Whether to apply volumetric locking correction
  const bool _volumetric_locking_correction;

  /// The current element volume
  const Real & _current_elem_volume;

  /// The total deformation gradient
  ADMaterialProperty<RankTwoTensor> & _F;
  ADMaterialProperty<RankTwoTensor> & _Fnobar;
  ADMaterialProperty<RankTwoTensor> & _F_store_Fbar;
  const MaterialProperty<RankTwoTensor> & _F_store_Fbar_old;
  // The mechanical deformation gradient (after excluding eigen deformation gradients from the total
  // deformation gradient)
  ADMaterialProperty<RankTwoTensor> & _Fm;

  ADMaterialProperty<RankTwoTensor> & _F_store_noFbar;
  const MaterialProperty<RankTwoTensor> & _F_store_noFbar_old;

  // @{ Eigen deformation gradients
  std::vector<MaterialPropertyName> _Fg_names;
  std::vector<const ADMaterialProperty<RankTwoTensor> *> _Fgs;
  // @}

  ADMaterialProperty<Real> & _weights;

  // is this recovering?
  const bool _recover;

  const SolutionUserObject * _solution_object_ptr;

  std::vector<const ADVariableValue *> _F_recover;

  QpMapping::Element _element = QpMapping::Element::HEX8_3rd;

  unsigned int _qpnum = 0;
  MaterialProperty<Real> & _Frobenius;
  MaterialProperty<Real> & _Jacobian;

  // Polar decomposition output (R and U from F = R*U)
  ADMaterialProperty<RankTwoTensor> & _rotation_tensor;
  const MaterialProperty<RankTwoTensor> & _rotation_tensor_old;
  ADMaterialProperty<RankTwoTensor> & _stretch_tensor;

  // Recovery mode flag
  const bool _recover_from_polar;

  /// When true, store R^(1/2) (rotation by theta/2) instead of R in the output.
  /// Halving the rotation angle keeps it in [0, pi/2), improving conditioning
  /// of the matrix logarithm used by the remapping algorithm.
  const bool _output_half_rotation;

  /// When true, the rotation tensor being read from the recovery file contains R^(1/2)
  /// and must be squared before reconstructing F = R*U.
  /// Set this to match what output_half_rotation_tensor was in the run that produced the file.
  const bool _input_half_rotation;

  /// Use Higham's iterative algorithm for polar decomposition instead of the default
  /// eigendecomposition of C = F^T*F.  Higham operates directly on F (kappa(F) vs kappa(F)^2),
  /// avoids inverting U, requires only 3x3 matrix inverses, and converges quadratically.
  const bool _use_iterative_polar;

private:
  const std::unordered_map<int, int> * _lookup;

  /// Compute the principal square root of a rotation matrix R in SO(3).
  /// R_half_old is the stored rotation_tensor from the previous timestep; its skew
  /// part is used to detect and correct axis-sign flips that occur when the physical
  /// rotation crosses π, maintaining component continuity across timesteps.
  static RankTwoTensor computeHalfRotation(const RankTwoTensor & R,
                                            const RankTwoTensor & R_half_old);

  /// Higham iterative polar decomposition: F = R * U where R in SO(3) and U is symmetric PD.
  /// X_{k+1} = (X_k + X_k^{-T}) / 2, converges quadratically to R.
  /// More numerically stable than getRUDecompositionRotation for ill-conditioned F.
  void polarDecompositionIterative(const RankTwoTensor & F, RankTwoTensor & R, RankTwoTensor & U);
};
