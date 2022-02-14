hardening_model_name = arrhenius_law_hardening

sigma_0 = 2e-7
Q = 1.4054e8
R = 8.3143e3
T = 800
eps = 688

tqf = 0

[AuxVariables]
  [T]
    initial_condition = ${T}
  []
[]

[Materials]
  [plasticity_properties]
    type = ADGenericConstantMaterial
    prop_names = 'sigma_0 Q'
    prop_values = '${sigma_0} ${Q}'
  []
  [arrhenius_law]
    type = ArrheniusLaw
    arrhenius_coefficient = A
    activation_energy = Q
    ideal_gas_constant = ${R}
    temperature = T
  []
  [arrhenius_law_hardening]
    type = ArrheniusLawHardening
    reference_stress = sigma_0
    arrhenius_coefficient = A
    eps = ${eps}
    phase_field = d
    degradation_function = g
    taylor_quinney_factor = ${tqf}
    temperature = T
  []
[]
