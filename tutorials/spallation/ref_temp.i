# oxide parameters
kappa_oxide = 3

# metal parameters
kappa_metal = 30

# boundary condition
h_gas = 0.1
h_steam = 2.8

# boundary condition
T_gas = '${fparse 900}'
T_steam = '${fparse 525}'

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = ${mesh}
  []
[]

[Variables]
  [temp]
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = 'temp'
  []
[]

[BCs]
  [convection_left]
    type = ConvectiveHeatFluxBC
    variable = temp
    boundary = 'inner'
    T_infinity = ${T_steam}
    heat_transfer_coefficient = ${h_steam}
  []
  [convection_right]
    type = ConvectiveHeatFluxBC
    variable = temp
    boundary = 'outer'
    T_infinity = ${T_gas}
    heat_transfer_coefficient = ${h_gas}
  []
[]

[Materials]
  [metal_thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_metal}'
    block = metal
  []
  [oxide_thermal_properties]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '${kappa_oxide}'
    block = oxide
  []
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true
[]
