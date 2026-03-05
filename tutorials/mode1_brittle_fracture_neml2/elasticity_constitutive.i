# Free energy
[Models]
  [degradation]
    type = PowerDegradationFunction
    phase = 'forces/d'
    degradation = 'state/g'
    power = 2
    eta = 1e-6
  []
  [elastic_energy]
    type = LinearIsotropicStrainEnergyDensity
    strain = 'forces/E'
    strain_energy_density_active = 'state/psie_active'
    strain_energy_density_inactive = 'state/psie_inactive'
    coefficient_types = 'YOUNGS_MODULUS POISSONS_RATIO'
    coefficients = '2.1e5 0.3'
    decomposition = 'NONE'
  []
  [sum]
    type = ScalarLinearCombination
    from_var = 'state/psie_active state/psie_inactive'
    to_var = 'state/psi'
    coefficients = 'degradation 1'
    coefficient_as_parameter = 'true false'
  []
  [energy]
    type = ComposedModel
    models = ' elastic_energy sum'
    additional_outputs = 'state/psie_active'
  []
  [stress]
    type = Normality
    model = 'energy'
    function = 'state/psi'
    from = 'forces/E'
    to = 'state/S'
  []
  [model]
    type = ComposedModel
    models = 'energy stress'
  []
[]
