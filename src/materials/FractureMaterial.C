#include "FractureMaterial.h"

registerMooseObject("raccoonApp", FractureMaterial);

template <>
InputParameters
validParams<FractureMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription(
      "Compute interface coefficient kappa and mobility for a damage field based on provided "
      "energy release rate Gc and crack length scale L");
  params.addRequiredCoupledVar("d", "damage variable that these material paramters apply to");
  params.addRequiredParam<FunctionName>("Gc", "fracture energy release rate");
  params.addRequiredParam<FunctionName>("L", "characteristic crack length scale");
  params.addRequiredParam<Real>("local_dissipation_norm", "norm of the local dissipation ||w(d)||");

  params.addParam<MaterialPropertyName>(
      "kappa_name", "kappa", "name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mob_name", "M", "name of the material that holds the mobility");

  return params;
}

FractureMaterial::FractureMaterial(const InputParameters & parameters)
  : Material(parameters),
    _Gc(getFunction("Gc")),
    _L(getFunction("L")),
    _w_norm(getParam<Real>("local_dissipation_norm")),
    _kappa(declareProperty<Real>(getParam<MaterialPropertyName>("kappa_name") + "_" +
                                 getVar("d", 0)->name())),
    _M(declareProperty<Real>(getParam<MaterialPropertyName>("mob_name") + "_" +
                             getVar("d", 0)->name()))
{
}

void
FractureMaterial::computeQpProperties()
{
  Real L = _L.value(_t, _q_point[_qp]);
  Real Gc = _Gc.value(_t, _q_point[_qp]);

  _kappa[_qp] = 2.0 * L * L;
  _M[_qp] = Gc / 4.0 / L / _w_norm;
}
