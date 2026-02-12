
[Tensors]
  [GcbylbyCo]
    type = Scalar
    values = 0.0114 # Gc/l/Co with Gc = 0.095 N/mm, l = 3.125 mm, Co = 8/3
  []
[]

[Models]
  ####################################
  [degrade]
    type = RationalDegradationFunction
    phase = 'forces/d'
    degradation = 'state/g'
    power = '2'
    eta = '1e-6'
    fitting_param_1 = '84.51996868' # b1 = (4*E*Gc)/(pi*l*psic^2)
    fitting_param_2 = '1.3868'
    fitting_param_3 = '0.6567'
  []
  [sed0]
    type = LinearIsotropicStrainEnergyDensity
    strain = 'forces/E'
    strain_energy_density_active = 'state/psie_active'
    strain_energy_density_inactive = 'state/psie_inactive'
    coefficient_types = 'YOUNGS_MODULUS POISSONS_RATIO'
    coefficients = '25.84e3 0.18'
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
    type = CrackGeometricFunctionAT1
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
    # Gc/l/Co with Gc = 0.095 N/mm, l = 3.125 mm, Co = 8/3
    coefficients = '0.0114 1'
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
