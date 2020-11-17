[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 16
    ny = 8
    xmin = 0
    xmax = 4
    ymin = 2
    ymax = 4
  []
[]

[Variables]
  [c]
  []
[]

[AuxVariables]
  [disp_x]
  []
  [disp_y]
  []
  [bounds_dummy]
  []
[]

[Adaptivity]
  marker = box
  max_h_level = 5
  initial_steps = 5
  [Markers]
    [box]
      type = BoxMarker
      bottom_left = '1.2 1.8 0'
      top_right = '2.8 2.2 0'
      inside = refine
      outside = do_nothing
    []
  []
[]

[ICs]
  [ini_c]
    type = BoundingBoxIC
    variable = c
    x1 = 1.8
    y1 = 1.99999
    x2 = 2.2
    y2 = 2.00001
    inside = 1.0
    outside = 0.0
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'c'
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = 'bounds_dummy'
    bounded_variable = 'c'
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [ACBulk]
    type = AllenCahn
    variable = c
    f_name = F
  []
  [ACInterface]
    type = ACInterface
    variable = c
    kappa_name = kappa_op
  []
[]

[Materials]
  [pressure]
    type = GenericFunctionMaterial
    prop_names = 'fracture_pressure'
    prop_values = 't'
  []
  [pfbulkmat]
    type = GenericConstantMaterial
    prop_names = 'gc_prop l visco'
    prop_values = '0.001 0.04 1e-6'
  []
  [define_mobility]
    type = ParsedMaterial
    material_property_names = 'gc_prop visco'
    f_name = L
    function = '1.0/(gc_prop * visco)'
  []
  [define_kappa]
    type = ParsedMaterial
    material_property_names = 'gc_prop l'
    f_name = kappa_op
    function = 'gc_prop * l'
  []
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.2
  []
  [strain]
    type = ComputeSmallStrain
  []
  [damage_stress]
    type = ComputeLinearElasticPFFractureStress
    c = c
    E_name = 'elastic_energy'
    D_name = 'degradation'
    F_name = 'local_fracture_energy'
    I_name = 'indicator_function'
    decomposition_type = none
    use_current_history_variable = true
    use_snes_vi_solver = true
  []
  [indicator_function]
    type = DerivativeParsedMaterial
    f_name = indicator_function
    args = 'c'
    function = 'c'
    derivative_order = 2
  []
  [degradation]
    type = DerivativeParsedMaterial
    f_name = degradation
    args = 'c'
    function = '(1.0-c)^2*(1.0 - eta) + eta'
    constant_names = 'eta'
    constant_expressions = '1e-8'
    derivative_order = 2
  []
  [local_fracture_energy]
    type = DerivativeParsedMaterial
    f_name = local_fracture_energy
    args = 'c'
    material_property_names = 'gc_prop l'
    function = 'c^2 * gc_prop / 2 / l'
    derivative_order = 2
  []
  [fracture_driving_energy]
    type = DerivativeSumMaterial
    args = c
    sum_materials = 'elastic_energy local_fracture_energy'
    derivative_order = 2
    f_name = F
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'

  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  dt = 1e-3
  automatic_scaling = true
[]

[Outputs]
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  print_linear_residuals = false
  console = false
[]
