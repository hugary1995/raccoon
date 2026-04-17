//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFDiffusionRecover.h"
#include "MaterialProperty.h"
#include "MooseTypes.h"
#include "RankTwoTensorForward.h"
#include "SolutionUserObject.h"

registerMooseObject("raccoonApp", ADPFFDiffusionRecover);

InputParameters
ADPFFDiffusionRecover::validParams()
{
  InputParameters params = ADPFFDiffusion::validParams();
  params.addClassDescription("The diffusion term in the phase-field evolution equation. The weak "
                             "form is $(\\grad w, \\dfrac{2\\Gc l}{c_0} \\grad d)$.");

  params.addParam<Real>("qp", 8, "Number of qps");
  params.addParam<MaterialPropertyName>("Fbarprev", "Fbar from previous simulation");
  return params;
}

ADPFFDiffusionRecover::ADPFFDiffusionRecover(const InputParameters & parameters)
  : ADPFFDiffusion(parameters), _Fbarprev(getADMaterialProperty<RankTwoTensor>("Fbarprev"))
// _d_diff(dynamic_cast<MooseVariable &>(_subproblem.getVariable(0, "diff")))
// _d_diff(_sys.addVector("d_diff", true, GHOSTED))
{
}

ADReal
ADPFFDiffusionRecover::computeQpResidual()
{

  ADReal value = 0;

  auto mult = (_grad_u[_qp] * _Fbarprev[_qp]);
  value = _grad_test[_i][_qp] * _Fbarprev[_qp] * mult;
  return 2 * _Gc[_qp] * _l[_qp] / _c0[_qp] * value;
}
