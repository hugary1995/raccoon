//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationPlasticityModel.h"
#include "Qp_Mapping.h"

/// Intermediate base for J2 plasticity models that support QP-mapped material recovery.
/// Provides the element type, QP-reordering lookup, and a default initQpStatefulProperties
/// that seeds _ep and _Fp from a SolutionUserObject when recover = true.
class LargeDeformationJ2PlasticityBase : public LargeDeformationPlasticityModel
{
public:
  static InputParameters validParams();

  LargeDeformationJ2PlasticityBase(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  /// Format a 1-based QP index as a fixed-width string matching the output convention.
  std::string formatQP(unsigned int qp) const;

  QpMapping::Element _element = QpMapping::Element::HEX8_3rd;
  unsigned int _qpnum = 0;
  const std::unordered_map<int, int> * _lookup = nullptr;
};
