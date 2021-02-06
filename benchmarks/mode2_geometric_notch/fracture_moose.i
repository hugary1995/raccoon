[Mesh]
  type = FileMesh
  file = 'gold/geo0.msh'
[]

[Variables]
  [d]
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
  [ACBulk]
    type = AllenCahn
    variable = d
    f_name = F
  []
  [ACInterface]
    type = ACInterface
    variable = d
    kappa_name = kappa_op
  []
[]

[Materials]
  [strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
  [pfbulkmat]
    type = GenericConstantMaterial
    prop_names = 'gc_prop l visco'
    prop_values = '${Gc} ${l} 1e-4'
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
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  []
  [damage_stress]
    type = ComputeLinearElasticPFFractureStress
    c = d
    E_name = 'elastic_energy'
    D_name = 'degradation'
    F_name = 'local_fracture_energy'
    decomposition_type = strain_vol_dev
  []
  [degradation]
    type = DerivativeParsedMaterial
    f_name = degradation
    args = 'd'
    function = '(1.0-d)^2*(1.0 - eta) + eta'
    constant_names = 'eta'
    constant_expressions = '0.0'
    derivative_order = 2
  []
  [local_fracture_energy]
    type = DerivativeParsedMaterial
    f_name = local_fracture_energy
    args = 'd'
    material_property_names = 'gc_prop l'
    function = 'd^2 * gc_prop / 2 / l'
    derivative_order = 2
  []
  [fracture_driving_energy]
    type = DerivativeSumMaterial
    args = d
    sum_materials = 'elastic_energy local_fracture_energy'
    derivative_order = 2
    f_name = F
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  automatic_scaling = true

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [console]
    type = Console
    outlier_variable_norms = false
  []
[]
