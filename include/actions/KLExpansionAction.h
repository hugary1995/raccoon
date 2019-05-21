//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once
#ifdef LIBMESH_HAVE_EXTERNAL_BOOST

// MOOSE includes
#include "Action.h"
#include "AddAuxVariableAction.h"
// LIBMESH includes
#include "libmesh/fe_type.h"
// BOOST
#include "libmesh/ignore_warnings.h"
#include <boost/math/distributions.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/generator_iterator.hpp>
#include "libmesh/restore_warnings.h"

// Forward declaration
class KLExpansionAction;

template <>
InputParameters validParams<KLExpansionAction>();

class KLExpansionAction : public Action
{
public:
  KLExpansionAction(const InputParameters & params);

  virtual void act();

protected:
  virtual void generateDataFile();
  virtual void addFunctionInterpolator();
  virtual void addFieldGenerator();

  /// FEType for the variable being created
  const FEType _fe_type;
  // name of the file containing KL expansion information
  const FileName _file_name;
  // perturbation type
  enum class GaussianPerturbation
  {
    Random,
    Custom
  } _Gaussian_perturbation;
  // distribution type
  enum class FieldDistribution
  {
    Normal,
    Gamma
  } _field_distribution;
  // whether to output auxvariables to visualize bases
  const bool _monitor_bases;
  // whether to output auxvariables to visualize field
  const bool _monitor_field;
  // number of bases
  unsigned int _num_bases;
  // basis function names
  std::vector<FunctionName> _functions;
  // Gaussian weights
  std::vector<Real> _Gaussian_weights;

private:
  boost::mt19937 ENGINE;
  boost::normal_distribution<> GAUSSIAN_DIST;
  // normal RV generator
  boost::variate_generator<boost::mt19937 &, boost::normal_distribution<>> _Gaussian_gen;
};

#endif // KLExpansionAction_H
