//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
//
// #pragma once
//
// #include "FEProblem.h"
//
// /**
//  * FEProblem derived class for lagging a material
//  */
// class FixedPointProblem : public FEProblem
// {
// public:
//   static InputParameters validParams();
//
//   FixedPointProblem(const InputParameters & params);
//
//   virtual void betweenIterations();
// };

#pragma once

#include "FEProblem.h"

/**
 * FEProblem derived class for lagging a tagged residual.
 */
class FixedPointProblem : public FEProblem
{
public:
  static InputParameters validParams();

  FixedPointProblem(const InputParameters & params);

  virtual void computeResidual(const NumericVector<Number> & soln,
                               NumericVector<Number> & residual) override;
  virtual void computeFullResidual(const NumericVector<Number> & soln,
                                   NumericVector<Number> & residual);

protected:
  TagName _tag_previous;
  TagID _tag_id;
  NumericVector<Number> & _residual_previous;
};
