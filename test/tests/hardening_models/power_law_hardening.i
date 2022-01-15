hardening_model_name = power_law_hardening

sigma_y = 320
n = 5
ep0 = 0.01

[Materials]
  [power_law_hardening]
    type = PowerLawHardening
    yield_stress = ${sigma_y}
    exponent = ${n}
    reference_plastic_strain = ${ep0}
    phase_field = d
    degradation_function = g
  []
[]
