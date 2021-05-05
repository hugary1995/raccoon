[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
  []
[]

[Variables]
  [d]
  []
  [disp_x]
  []
  [disp_y]
  []
  [strain_zz]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
[]

[Bounds]
  [irreversibility]
    type = VariableOldValueBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = lower
  []
  [upper]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  []
  [plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
  [react_x]
    type = ADCoefMatReaction
    variable = 'disp_x'
    coefficient = 0.1
    prop_names = 'g'
  []
  [react_y]
    type = ADCoefMatReaction
    variable = 'disp_y'
    coefficient = 0.1
    prop_names = 'g'
  []
  [pff_diff]
    type = ADPFFDiffusion
    variable = d
    mobility_name = L
    kappa_name = kappa
  []
  [pff_barr]
    type = ADPFFBarrier
    variable = d
    mobility_name = L
    local_dissipation_name = alpha
  []
  [pff_react]
    type = ADPFFReaction
    variable = d
    driving_energy_mat = 'E_el_active'
  []
[]

[BCs]
  # [disp_x_fix]
  #   type = DirichletBC
  #   variable = disp_x
  #   value = 0
  #   boundary = 'top bottom left right'
  # []
  # [disp_y_fix]
  #   type = DirichletBC
  #   variable = disp_y
  #   value = 0
  #   boundary = 'top bottom left right'
  # []
  # There is an issue with periodic BC due to ghosting functors.
  # This issue should be fixed pretty soon.
  [Periodic]
    [x]
      variable = disp_x
      auto_direction = 'x y'
    []
    [y]
      variable = disp_y
      auto_direction = 'x y'
    []
    [d]
      variable = d
      auto_direction = 'x y'
    []
  []
[]

[Materials]
  [eigen_strain]
    type = ADComputeEigenstrainFromFunctionEigenstress
    eigen_stress = 't 0 0 0 t 0 0 0 0'
    eigenstrain_name = 'es'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    eigenstrain_names = 'es'
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_NoSplit
    d = 'd'
    d_crit = 0.6
  []
  [mobility]
    type = ADParsedMaterial
    f_name = L
    material_property_names = 'Gc c0 l'
    function = 'Gc/c0/l'
  []
  [kappa]
    type = ADParsedMaterial
    f_name = kappa
    material_property_names = 'l'
    function = '2*l^2'
  []
  [length_scale]
    type = ADGenericConstantMaterial
    prop_names = 'l Gc psic'
    prop_values = '1 1 1'
  []
  [crack_geometric]
    type = ADDerivativeParsedMaterial
    f_name = alpha
    function = 'd'
    args = d
    derivative_order = 1
  []
  [degradation]
    type = ADDerivativeParsedMaterial
    f_name = g
    args = d
    function = (1-d)^p*(1-eta)+eta
    constant_names = 'p eta'
    constant_expressions = '2 1e-6'
    derivative_order = 1
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  dt = 1e-3
  end_time = 0.22

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
[]
