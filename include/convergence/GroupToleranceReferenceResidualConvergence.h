//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ReferenceResidualConvergence.h"

/**
 * ReferenceResidualConvergence with an optional absolute tolerance per group, in unscaled units.
 * A group with a positive tolerance is converged when its unscaled residual is below it, and the
 * other groups use the usual relative check against the reference residual. This is meant for
 * groups whose reference residual is no measure of their size, e.g. mortar contact Lagrange
 * multipliers, whose rows only ever see min(lambda, c * gap).
 */
class GroupToleranceReferenceResidualConvergence : public ReferenceResidualConvergence
{
public:
  static InputParameters validParams();

  GroupToleranceReferenceResidualConvergence(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual bool checkResidualConvergence(const unsigned int n_iter,
                                        const Real fnorm,
                                        const Real ref_norm,
                                        const Real rel_tol,
                                        const Real abs_tol,
                                        std::ostringstream & oss) override;

private:
  /// Whether every group meets its tolerance: its own absolute tolerance if it has one, else the
  /// ReferenceResidualConvergence test
  bool checkGroups(const Real rel_tol, const Real abs_tol) const;

  /// Absolute tolerance for each group in unscaled units; zero means use the relative check
  const std::vector<Real> & _group_abs_tol;
};
