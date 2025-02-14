//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LoadingUnloadingDirichletBC.h"
#include "FEProblemBase.h"

registerMooseObject("raccoonApp", LoadingUnloadingDirichletBC);

InputParameters
LoadingUnloadingDirichletBC::validParams()
{
  InputParameters params = DirichletBCBase::validParams();
  params.addClassDescription(
      "This class applies a loading/unloading BC. The load ramps up linearly until it reaches the "
      "load cap. Once the load cap is reached, the load cap is incremented and unlading begins. "
      "Once the unloaded indicator becomes negative, loading starts. If the load cap exceeds the "
      "ultimate load, the entire loading/unloading terminates, and the current simulation is "
      "terminated.");
  params.addRequiredParam<Real>(
      "initial_load_cap",
      "Initial cap of the loading. The load is decreased once it reaches the cap, the cap is "
      "then increased according to the load cap increment.");
  params.addRequiredParam<Real>("load_cap_increment",
                                "The amount to increase the load cap everytime it is reached.");
  params.addRequiredParam<Real>("load_step", "The amount to increase the load every step.");
  params.addRequiredParam<Real>("ultimate_load",
                                "The load, upon reached, to terminate the simulation.");
  params.addRequiredParam<PostprocessorName>(
      "unloaded_indicator",
      "A postprocessor whose value serves as an indicator for unloaded status. Once its value is "
      "below zero, the system is completely unloaded, and loading starts again.");
  return params;
}

LoadingUnloadingDirichletBC::LoadingUnloadingDirichletBC(const InputParameters & parameters)
  : DirichletBCBase(parameters),
    _load(0.0),
    _load_cap(getParam<Real>("initial_load_cap")),
    _load_cap_inc(getParam<Real>("load_cap_increment")),
    _load_step(getParam<Real>("load_step")),
    _ultimate_load(getParam<Real>("ultimate_load")),
    _unloaded_indicator(getPostprocessorValue("unloaded_indicator")),
    _loading(true)
{
}

void
LoadingUnloadingDirichletBC::timestepSetup()
{
  _console << COLOR_BLUE << "Unloading indicator = " << COLOR_DEFAULT << std::scientific
           << _unloaded_indicator << std::endl;
  if (_unloaded_indicator < 0.0)
  {
    _loading = true;
    _console << COLOR_BLUE << "Completely unloaded, start loading next." << COLOR_DEFAULT
             << std::endl;
  }

  if (_loading)
  {
    _load += _load_step;
    if (_load > _load_cap)
    {
      _load_cap += _load_cap_inc;
      _load -= 2.0 * _load_step;
      _loading = false;
      _console << COLOR_BLUE << "Load cap exceeded, start unloading next." << COLOR_DEFAULT
               << std::endl;
      _console << COLOR_BLUE << "Load cap increased to " << COLOR_DEFAULT << std::scientific
               << _load_cap << COLOR_DEFAULT << std::endl;
    }
  }
  else
  {
    _load -= _load_step;
  }

  _console << COLOR_BLUE << "Current load = " << COLOR_DEFAULT << std::scientific << _load
           << std::endl
           << std::flush;

  if (_load > _ultimate_load)
    _fe_problem.terminateSolve();

  fflush(stdout);
}

Real
LoadingUnloadingDirichletBC::computeQpValue()
{
  return _load;
}
