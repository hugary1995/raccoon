hardening_model_name = linear_hardening

sigma_y = 320
H = 3.44e3

[Materials]
  [linear_hardening]
    type = LinearHardening
    phase_field = d
    yield_stress = ${sigma_y}
    hardening_modulus = ${H}
    degradation_function = g
  []
[]
