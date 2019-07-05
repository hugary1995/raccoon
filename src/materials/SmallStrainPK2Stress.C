//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainPK2Stress.h"

registerADMooseObject("raccoonApp", SmallStrainPK2Stress);

defineADValidParams(
    SmallStrainPK2Stress,
    ADComputeStressBase,
    params.addClassDescription("Compute stress following small deformation elasticity");
    params.addParam<MaterialPropertyName>("elastic_energy_name",
                                          "E_el",
                                          "name of the material that holds the elastic energy"););

template <ComputeStage compute_stage>
SmallStrainPK2Stress<compute_stage>::SmallStrainPK2Stress(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _E_el_name(getParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_pos(declareADProperty<Real>(_E_el_name))
{
}

template <ComputeStage compute_stage>
void
SmallStrainPK2Stress<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  _stress[_qp] = _elasticity_tensor[_qp] * _mechanical_strain[_qp];

  // elastic strain in reference configuration
  _elastic_strain[_qp] = _mechanical_strain[_qp];
}
