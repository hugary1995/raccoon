//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

// Forward Declarations
template <ComputeStage>
class PFBrittleFractureADTest;

declareADValidParams(PFBrittleFractureADTest);

template <ComputeStage compute_stage>
class PFBrittleFractureADTest : public ADKernel<compute_stage>,
                                public DerivativeMaterialPropertyNameInterface
{
public:
  PFBrittleFractureADTest(const InputParameters & parameters);

protected:
  virtual ADResidual computeQpResidual() override;

  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _M;
  const MaterialPropertyName _g_name;
  const ADMaterialProperty(Real) & _dg_dd;
  const MaterialProperty<Real> & _E_el_pos_old;

  usingKernelMembers;
};
