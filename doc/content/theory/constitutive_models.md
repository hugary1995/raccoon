# Constitutive models

RACCOON features the plug-n-play design, which allows (almost) any physically admissible combinations of available constitutive models.

The constitutive models can be roughly categorized as

- Elasticity models
- Viscoelasticity models
- Plasticity (creep) models
- Plastic hardening models
- Phase-field fracture models

!alert tip title=Small deformation v.s. large deformation
We support both small deformation and large deformation. Several hyperelastic models are provided, including the Saint Vernant-Kirchhoff model, the Mooney-Rivlin model, the compressible Neo-Hookean model, and the Hencky model. Small deformation plasticity is modeled using additive decomposition of strain, while large deformation plasticity is modeled using multiplicative decomposition of the deformation gradient. Plastic hardening models and phase-field fracture models can be applied regardless of geometric nonlinearity.

## Elasticity models

- [Small deformation isotropic elasticity](SmallDeformationIsotropicElasticity.md)
- [Compressible Neo-Hookean material](CNHIsotropicElasticity.md)
- [A Hencky-type material](HenckyIsotropicElasticity.md)

## Viscoelasticity models

- [Large deformation Newtonian viscosity](LargeDeformationNewtonianViscosity.md)

## Plasticity models

- [Small deformation $J_2$ plasticity](SmallDeformationJ2Plasticity.md)
- [Large deformation $J_2$ plasticity](LargeDeformationJ2Plasticity.md)
- [Large deformation $J_2$ power-law creep](LargeDeformationJ2PowerLawCreep.md)

## Plastic hardening models

- [Power-law hardening](PowerLawHardening.md)
- [Arrhenius-law hardening](ArrheniusLawHardening.md)
- [Johnson-Cook Hardening](JohnsonCookHardening.md)

## Phase-field fracture models

- [No degradation](NoDegradation.md)
- [Power law degradation function](PowerDegradationFunction.md)
- [Rational degradation function](RationalDegradationFunction.md)
- [Crack geometric function](CrackGeometricFunction.md)

!content pagination use_title=True
                    previous=theory/minimization.md
