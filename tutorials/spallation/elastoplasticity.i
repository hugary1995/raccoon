E = 1.2e5
nu = 0.24
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

alpha = 1.5e-5
T0 = 835

h0 = 0.2
H = 11

l = 0.5
Gc = 1
psic = 1

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 88
    ymax = 88
    nx = 88
    ny = 88
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [strain_zz]
  []
[]

[AuxVariables]
  [d]
  []
  [T]
    initial_condition = ${T0}
  []
  [T0]
    initial_condition = ${T0}
  []
  [h]
    initial_condition = ${h0}
  []
[]

[AuxKernels]
  [cooling]
    type = FunctionAux
    variable = T
    function = '${T0}-5*t'
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
  []
  [plane_stress]
    type = ADWeakOutOfPlaneStress
    variable = strain_zz
    out_of_plane_stress = -17
  []
  [react_x]
    type = ADCoefMatReaction
    variable = disp_x
    prop_names = 'c'
  []
  [react_y]
    type = ADCoefMatReaction
    variable = disp_y
    prop_names = 'c'
  []
[]

[BCs]
  [Periodic]
    [x]
      variable = disp_x
      auto_direction = 'x'
    []
    [y]
      variable = disp_y
      auto_direction = 'x'
    []
  []
  [fix_bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  []
  [fix_bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic alpha'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic} ${alpha}'
  []
  # fracture
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = w
    function = 'd'
    phase_field = d
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    phase_field = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
  []
  # elasticity
  [thermal_expansion]
    type = ComputeThermalExpansionEigenDeformationGradient
    eigen_deformation_gradient_name = F_thermal
    temperature = T
    reference_temperature = T0
    thermal_expansion_coefficient = alpha
  []
  [deformation_gradient]
    type = ComputeDeformationGradient
    out_of_plane_strain = strain_zz
    eigen_deformation_gradient_names = 'F_thermal'
  []
  [hencky]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
    output_properties = 'psie_active'
    outputs = exodus
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    output_properties = 'stress'
    outputs = exodus
  []
  # shear lag
  [shear_lag]
    type = ADParsedMaterial
    f_name = c
    function = 'g*G/h/H'
    args = 'h'
    material_property_names = 'g G'
    constant_names = 'H'
    constant_expressions = '${H}'
  []
[]

[Postprocessors]
  [stress_xx]
    type = ElementAverageValue
    variable = stress_00
  []
  [stress_yy]
    type = ElementAverageValue
    variable = stress_11
  []
  [stress_zz]
    type = ElementAverageValue
    variable = stress_22
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist'
  dt = 1
  end_time = 100

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  automatic_scaling = true
  abort_on_solve_fail = true
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]
