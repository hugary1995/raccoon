hardening_model_name = exponential_hardening

sigma_y = 320
sigma_ult = 330
ep0 = 0.01
H = 6.88

[Materials]
  [exponential_hardening]
    type = ExponentialHardening
    yield_stress = ${sigma_y}
    ultimate_stress = ${sigma_ult}
    reference_plastic_strain = ${ep0}
    hardening_modulus = ${H}
    phase_field = d
    degradation_function = g
  []
[]
