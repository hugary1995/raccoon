

[Tensors]
  [GcbylbyCo]
    type = Scalar
    values = 67.5 # Gc/l/Co with Gc = 2.7 N/m, l = 0.02 mm, Co = 2
  []
[]

[Models]
  ####################################
  [degrade]
    type = PowerDegradationFunction
    phase = 'forces/d'
    degradation = 'state/g'
    power = '2'
  []
  [sed0]
    type = LinearIsotropicStrainEnergyDensity
    strain = 'forces/E'
    strain_energy_density_active = 'state/psie_active'
    strain_energy_density_inactive = 'state/psie_inactive'
    coefficient_types = 'YOUNGS_MODULUS POISSONS_RATIO'
    coefficients = '2.1e5 0.3'
    decomposition = 'VOLDEV'
  []
  [sed1]
    type = ScalarMultiplication
    from_var = 'state/g state/psie_active'
    to_var = 'state/psie_degraded'
  []
  [sed]
    type = ScalarLinearCombination
    from_var = 'state/psie_degraded state/psie_inactive'
    to_var = 'state/psie'
    coefficients = '1 1'
  []
  [energy1] # this guy maps from strain -> degraded energy
    type = ComposedModel
    models = 'degrade sed0 sed1 sed'
  []
  [stress]
    type = Normality
    model = 'energy1'
    function = 'state/psie'
    from = 'forces/E'
    to = 'state/S'
  []
  [mechanics]
    type = ComposedModel
    models = 'stress sed0'
  []
  ####################################
  [cracked]
    type = CrackGeometricFunctionAT2
    phase = 'forces/d'
    crack = 'state/alpha'
  []
  [sed2]
    type = ScalarMultiplication
    from_var = 'state/g forces/psie0'
    to_var = 'state/psie'
  []
  [sum]
    type = ScalarLinearCombination
    from_var = 'state/alpha state/psie'
    to_var = 'state/psi'
    # Gc/l/c0 = 67.5 with Gc = 2.7 N/m, l = 0.02 mm, c0 = 2
    coefficients = '67.5 1'
  []
  [energy2] # this guy maps from d -> energy
    type = ComposedModel
    models = 'degrade cracked sed2 sum'
  []
  [fracture]
    type = Normality
    model = 'energy2'
    function = 'state/psi'
    from = 'forces/d'
    to = 'state/dpsi_dd'
  []
[]
