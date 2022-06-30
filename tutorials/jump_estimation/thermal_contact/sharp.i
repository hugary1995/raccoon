crack_length = 0.8
left = '${fparse (1.0-crack_length)/2.0}'
right = '${fparse left+crack_length}'

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 20
    xmin = 0.0
    xmax = 1.0
    ymin = 0.0
    ymax = 1.0
    elem_type = QUAD4
  []
  [subdomain1]
    type = SubdomainBoundingBoxGenerator
    input = gen
    block_name = block1
    block_id = 1
    bottom_left = '0 0 0'
    top_right = '${left} 1.0 0'
  []
  [subdomain2]
    type = SubdomainBoundingBoxGenerator
    input = subdomain1
    block_name = block2
    block_id = 2
    bottom_left = '${right} 0 0'
    top_right = '1.0 1.0 0'
  []
  [subdomain3]
    type = SubdomainBoundingBoxGenerator
    input = subdomain2
    block_name = block3
    block_id = 3
    bottom_left = '${left} 0.5 0'
    top_right = '${right} 1.0 0'
  []
  [subdomain4]
    type = SubdomainBoundingBoxGenerator
    input = subdomain3
    block_name = block4
    block_id = 4
    bottom_left = '${left} 0 0'
    top_right = '${right} 0.5 0'
  []
  [breakmesh]
    type = BreakMeshByBlockGenerator
    block_pairs = '3 4'
    input = subdomain4
    add_interface_on_two_sides = true
    split_interface = true
  []
[]

[Variables]
  [T]
  []
[]

[Modules/TensorMechanics/Master]
  [all]
    strain = SMALL
    add_variables = true
  []
[]

[BCs]
  [bottomx]
    type = DirichletBC
    boundary = bottom
    variable = disp_x
    value = 0.0
  []
  [bottomy]
    type = DirichletBC
    boundary = bottom
    variable = disp_y
    value = 0.0
  []
  [topx]
    type = DirichletBC
    boundary = top
    variable = disp_x
    value = 0.0
  []
  [topy]
    type = FunctionDirichletBC
    boundary = top
    variable = disp_y
    function = '0.001*t'
  []
  [bottom_temp]
    type = DirichletBC
    variable = T
    value = 0
    boundary = bottom
  []
  [top_temp]
    type = DirichletBC
    variable = T
    value = 100
    boundary = top
  []
[]

[Kernels]
  [heat]
    type = HeatConduction
    variable = T
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.3
  []
  [elastic]
    type = ComputeLinearElasticStress
  []
  [thermal]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '1'
  []
[]

[ThermalContact]
  [gap_conductivity]
    type = GapHeatTransfer
    variable = T
    primary = block3_block4
    secondary = block4_block3
    emissivity_primary = 0
    emissivity_secondary = 0
    gap_conductivity = 0.005
    quadrature = true
    min_gap = 0.001
  []
[]

[VectorPostprocessors]
  [T_0.1]
    type = LineValueSampler
    variable = T
    start_point = '0.1 0 0'
    end_point = '0.1 1 0'
    sort_by = y
    num_points = 1000
  []
  [T_0.2]
    type = LineValueSampler
    variable = T
    start_point = '0.2 0 0'
    end_point = '0.2 1 0'
    sort_by = y
    num_points = 1000
  []
  [T_0.3]
    type = LineValueSampler
    variable = T
    start_point = '0.3 0 0'
    end_point = '0.3 1 0'
    sort_by = y
    num_points = 1000
  []
  [T_0.4]
    type = LineValueSampler
    variable = T
    start_point = '0.4 0 0'
    end_point = '0.4 1 0'
    sort_by = y
    num_points = 1000
  []
  [T_0.5]
    type = LineValueSampler
    variable = T
    start_point = '0.5 0 0'
    end_point = '0.5 1 0'
    sort_by = y
    num_points = 1000
  []
[]

[Executioner]
  type = Transient

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  line_search = 'none'

  # controls for linear iterations
  l_max_its = 10
  l_tol = 1e-2

  # controls for nonlinear iterations
  nl_max_its = 15
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-9

  # time control
  start_time = 0.0
  dt = 1.0
  end_time = 10
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
  [csv]
    type = CSV
    file_base = 'data/sharp'
    execute_vector_postprocessors_on = FINAL
  []
[]
