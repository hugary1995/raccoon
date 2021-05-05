//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

/**
 * This class computes the deformation gradient
 */
class ComputeDeformationGradient : public Material
{
public:
  static InputParameters validParams();

  ComputeDeformationGradient(const InputParameters & parameters);

  void initialSetup() override;

  void computeProperties() override;

protected:
  virtual void initQpStatefulProperties() override;

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

  /// Base name of the material system
  const std::string _base_name;

  /// Whether to apply volumetric locaking correction
  const bool _volumetric_locking_correction;

  /// The current element volume
  const Real & _current_elem_volume;

  /// Deformation gradient
  ADMaterialProperty<RankTwoTensor> & _F;
};
