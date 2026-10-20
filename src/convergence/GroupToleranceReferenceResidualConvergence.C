//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GroupToleranceReferenceResidualConvergence.h"

registerMooseObject("raccoonApp", GroupToleranceReferenceResidualConvergence);

InputParameters
GroupToleranceReferenceResidualConvergence::validParams()
{
  InputParameters params = ReferenceResidualConvergence::validParams();

  params.addRequiredParam<std::vector<Real>>(
      "group_abs_tol",
      "Absolute tolerance for each group in unscaled units, in the order the groups are printed. "
      "Zero uses the relative check against the reference residual instead");

  // The group residuals must be in physical units for the absolute tolerances to mean anything
  params.set<bool>("unscale_the_residual") = true;
  params.suppressParameter<bool>("unscale_the_residual");

  params.addClassDescription("Reference residual convergence with an optional absolute tolerance, "
                             "in unscaled units, for each variable group");

  return params;
}

GroupToleranceReferenceResidualConvergence::GroupToleranceReferenceResidualConvergence(
    const InputParameters & parameters)
  : ReferenceResidualConvergence(parameters),
    _group_abs_tol(getParam<std::vector<Real>>("group_abs_tol"))
{
  for (const auto tol : _group_abs_tol)
    if (tol < 0)
      paramError("group_abs_tol", "Tolerances must be non-negative");
}

void
GroupToleranceReferenceResidualConvergence::initialSetup()
{
  ReferenceResidualConvergence::initialSetup();

  if (_group_abs_tol.size() != _group_resid.size())
    paramError("group_abs_tol",
               "Expected one tolerance per group (",
               _group_resid.size(),
               " groups), but ",
               _group_abs_tol.size(),
               " were given");
}

bool
GroupToleranceReferenceResidualConvergence::checkResidualConvergence(const unsigned int n_iter,
                                                                     const Real fnorm,
                                                                     const Real ref_norm,
                                                                     const Real rel_tol,
                                                                     const Real abs_tol,
                                                                     std::ostringstream & oss)
{
  // Without a reference vector there are no groups to check
  if (!_reference_vector)
    return ReferenceResidualConvergence::checkResidualConvergence(
        n_iter, fnorm, ref_norm, rel_tol, abs_tol, oss);

  if (checkGroups(rel_tol, abs_tol))
  {
    oss << "Converged normally";
    return true;
  }
  else if (n_iter >= _accept_iters &&
           checkGroups(rel_tol * _accept_mult, abs_tol * _accept_mult))
  {
    oss << "Converged due to larger acceptable tolerances";
    _console << "  Converged due to ACCEPTABLE tolerances" << std::endl;
    return true;
  }

  return false;
}

bool
GroupToleranceReferenceResidualConvergence::checkGroups(const Real rel_tol,
                                                        const Real abs_tol) const
{
  for (const auto i : index_range(_group_resid))
  {
    // A group with its own absolute tolerance is judged on that alone
    if (_group_abs_tol[i] > 0)
    {
      if (_group_resid[i] >= _group_abs_tol[i])
        return false;
      continue;
    }

    // Otherwise, the same test as ReferenceResidualConvergence::checkConvergenceIndividVars
    const bool converged =
        (!_local_norm && _group_resid[i] < _group_ref_resid[i] * rel_tol) ||
        (_local_norm && _group_ref_resid[i] < rel_tol) || _group_resid[i] < abs_tol ||
        (!_group_ref_resid[i] && !_local_norm &&
         ((_zero_ref_type == ZeroReferenceType::ZERO_TOLERANCE && !_group_resid[i]) ||
          (_zero_ref_type == ZeroReferenceType::RELATIVE_TOLERANCE &&
           _group_resid[i] <= rel_tol)));
    if (!converged)
      return false;
  }

  return true;
}
