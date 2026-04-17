//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADStressDivergenceTensors.h"
#include "SolutionUserObject.h"

/**
 * ADDynamicStressDivergenceTensorsRecover is the automatic
 * differentiation version of DynamicStressDivergenceTensors.
 * This kernel derives from ADStressDivergenceTensors and
 * adds stress related Rayleigh and HHT time integration terms.
 */
class ADDynamicStressDivergenceTensorsRecover : public ADStressDivergenceTensors
{
public:
  static InputParameters validParams();

  ADDynamicStressDivergenceTensorsRecover(const InputParameters & parameters);

protected:
  ADReal computeQpResidual();

  ///{@ The old and older states of the stress tensor that the divergence operator operates on
  const MaterialProperty<RankTwoTensor> & _stress_older;
  const MaterialProperty<RankTwoTensor> & _stress_old;
  const ADMaterialProperty<RankTwoTensor> & _stress_older_sol;
  const ADMaterialProperty<RankTwoTensor> & _stress_old_sol;
  ///@}

  // Rayleigh damping parameter _zeta and HHT time integration parameter _alpha
  const MaterialProperty<Real> & _zeta;
  const Real _alpha;
  const bool _static_initialization;

  const SolutionUserObject * _solution_object_ptr;
  Assembly & _assembly_undisplaced;
  const MooseArray<Point> & _q_point_undisplaced;
};
