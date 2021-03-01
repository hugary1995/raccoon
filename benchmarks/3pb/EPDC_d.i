[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/beam.msh'
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [coalescence_mobility]
    order = CONSTANT
    family = MONOMIAL
  []
  [E_el_active]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'd'
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [pff_diff]
    type = ADPFFDiffusion
    variable = 'd'
    mobility_name = 'coalescence_mobility'
  []
  [pff_barr_coalesce]
    type = ADPFFBarrier
    variable = 'd'
    mobility_name = 'coalescence_mobility'
  []
  [pff_react_elastic]
    type = ADPFFReaction
    variable = 'd'
    degradation_name = 'g'
    driving_energy_var = 'E_el_active'
  []
  [visco]
    type = CoefTimeDerivative
    variable = 'd'
    Coefficient = 0
  []
[]

[Materials]
  [bulk]
    type = GenericFunctionMaterial
    prop_names = 'phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '${l} ${fparse Gc / beta_effective} ${fparse psic / beta_effective}'
  []
  [local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  []
  [fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 0
  []
  [mobility]
    type = ADParsedMaterial
    args = 'coalescence_mobility'
    function = 'coalescence_mobility'
    f_name = 'coalescence_mobility'
    constant_on = ELEMENT
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  dt = 1.5

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
[]
