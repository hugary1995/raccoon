#include "BrittleFractureMaterial.h"

registerMooseObject("raccoonApp", BrittleFractureMaterial);

template <>
InputParameters
validParams<BrittleFractureMaterial>()
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
      "kappa_base_name", "kappa", "base name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_base_name", "mobility", "base name of the material that holds the mobility");

  return params;
}

BrittleFractureMaterial::BrittleFractureMaterial(const InputParameters & parameters)
  : Material(parameters),
    _Gc(getFunction("Gc")),
    _L(getFunction("L")),
    _w_norm(getParam<Real>("local_dissipation_norm")),
    _kappa(declareProperty<Real>(getParam<MaterialPropertyName>("kappa_base_name") + "_" +
                                 getVar("d", 0)->name())),
    _mobility(declareProperty<Real>(getParam<MaterialPropertyName>("mobility_base_name") + "_" +
                                    getVar("d", 0)->name()))
{
}

void
BrittleFractureMaterial::computeQpProperties()
{
  Real L = _L.value(_t, _q_point[_qp]);
  Real Gc = _Gc.value(_t, _q_point[_qp]);

  _kappa[_qp] = 2.0 * L * L;
  _mobility[_qp] = Gc / 4.0 / L / _w_norm;
}
