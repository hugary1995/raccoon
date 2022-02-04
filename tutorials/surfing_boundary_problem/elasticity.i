E = 9.8e3 #9.8Gpa
nu = 0.13
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
Lambda = '${fparse E*nu/(1+nu)/(1-2*nu)}'

Gc = 9.1e-2 # 91N/m
l = 0.35
sigma_ts = 27
sigma_cs = 77
delta = 1.16
c1 = '${fparse (1+nu)*sqrt(Gc)/sqrt(2*pi*E)}'
c2 = '${fparse (3-nu)/(1+nu)}'

nx = 90
ny = 30
refine = 3

[Functions]
  [bc_func]
    type = ParsedFunction
    value = c1*((x-20*t)^2+y^2)^(0.25)*(c2-cos(atan2(y,(x-20*t))))*sin(0.5*atan2(y,(x-20*t)))
    vars = 'c1 c2'
    vals = '${c1} ${c2}'
  []
[]

[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = fracture.i
    cli_args = 'E=${E};K=${K};G=${G};Lambda=${Lambda};Gc=${Gc};l=${l};nx=${nx};ny=${ny};refine=${refi'
               'ne}'
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    variable = 'd'
    source_variable = 'd'
  []
  [to_psie_active]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    variable = 'psie_active ce'
    source_variable = 'psie_active ce'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${nx}
    ny = ${ny}
    xmax = 30
    ymin = -5
    ymax = 5
  []
  [gen2]
    type = ExtraNodesetGenerator
    input = gen
    new_boundary = fix_point
    coord = '0 -5'
  []
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = 3
  stop_time = 0
  max_h_level = ${refine}
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '0 -0.7 0'
      top_right = '30 0.7 0'
      outside = DO_NOTHING
      inside = REFINE
    []
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
    [InitialCondition]
      type = FunctionIC
      function = 'if(y=0&x>=0&x<=5,1,0)'
    []
  []
[]

[Kernels]
  [solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
[]

[BCs]
  [fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = fix_point
    value = 0
  []
  [bottom_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = bottom
    function = bc_func
  []
  [top_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = top
    function = bc_func
  []
[]

[Materials]
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'E K G lambda Gc l'
    prop_values = '${E} ${K} ${G} ${Lambda} ${Gc} ${l}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 0'
  []
  [strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = 'strain_zz'
    displacements = 'disp_x disp_y'
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
    output_properties = 'psie_active psie'
    outputs = exodus
  []
  [stress]
    type = SmallDeformationEquilibriumStress
    elasticity_model = elasticity
    output_properties = 'stress'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [kumar_material]
    type = NucleationMicroForce
    normalization_constant = c0
    tensile_strength = '${sigma_ts}'
    compressive_strength = '${sigma_cs}'
    delta = '${delta}'
    external_driving_force_name = ce
    output_properties = 'ce'
    outputs = exodus
  []
[]

[Postprocessors]
  [Jint]
    type = PhaseFieldJIntegral
    J_direction = '1 0 0'
    strain_energy_density = psie
    displacements = 'disp_x disp_y'
    boundary = 'left top right bottom'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '
  automatic_scaling = true

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  dt = 2e-2
  end_time = 5e-1

  fixed_point_max_its = 20
  accept_on_max_fixed_point_iteration = false
  fixed_point_rel_tol = 1e-3
  fixed_point_abs_tol = 1e-5
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
