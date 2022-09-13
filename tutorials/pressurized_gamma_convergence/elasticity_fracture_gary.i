E = 3e4
nu = 0.2
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

Gc = 0.12
psic = 0.15
l = 0.16
h = '${fparse l/5}'

W = 8
H = 2.5
a = '${fparse 0.2*W}'
refine = 3
nx = '${fparse ceil(W/h/2^refine)}'
ny = '${fparse ceil(H/h/2^refine)}'

pref = '${fparse sqrt(E*Gc/((1-nu^2)*pi*0.2*W))}'
p = '${fparse pref/2}'

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = ${W}
    ymax = ${H}
    nx = ${nx}
    ny = ${ny}
  []
[]

[Adaptivity]
  marker = marker
  initial_marker = marker
  initial_steps = ${refine}
  stop_time = 0
  max_h_level = ${refine}
  [Markers]
    [marker]
      type = BoxMarker
      bottom_left = '0 ${fparse -12*l} 0'
      top_right = '${fparse W/2} ${fparse 12*l} 0'
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
  [d]
    [InitialCondition]
      type = CohesiveDamageIC
      d0 = 1
      l = ${l}
      x1 = 0
      y1 = 0
      z1 = 0
      x2 = '${fparse 0.2*W}'
      y2 = 0
      z2 = 0
    []
  []
[]

[AuxVariables]
  [q]
    [InitialCondition]
      type = BoundingBoxIC
      x1 = '${fparse 0.5*a}'
      y1 = 0
      x2 = '${fparse 2*a}'
      y2 = 0.8
      inside = 1
      outside = 0
    []
  []
  [bounds_dummy]
  []
[]

[Bounds]
  [lower]
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
    variable = disp_x
    component = 0
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [pressure_x]
    type = ADPressurizedCrack
    variable = disp_x
    component = 0
    indicator_function = I
    phase_field = d
    pressure = p
  []
  [pressure_y]
    type = ADPressurizedCrack
    variable = disp_y
    component = 1
    indicator_function = I
    phase_field = d
    pressure = p
  []
  [diff]
    type = ADPFFDiffusion
    variable = d
    fracture_toughness = Gc
    regularization_length = l
    normalization_constant = c0
  []
  [source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
[]

[BCs]
  [symmetry]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [surfing_x]
    type = ModeISurfingDirichletBC
    variable = disp_x
    boundary = 'top'
    G = ${G}
    K = ${K}
    Gc = ${Gc}
    component = 0
    initial_crack_tip_position = '0 0 0'
    crack_propagation_velocity = '${fparse 0.05*W} 0 0'
  []
  [surfing_y]
    type = ModeISurfingDirichletBC
    variable = disp_y
    boundary = 'top'
    G = ${G}
    K = ${K}
    Gc = ${Gc}
    component = 1
    initial_crack_tip_position = '0 0 0'
    crack_propagation_velocity = '${fparse 0.05*W} 0 0'
  []
[]

[Materials]
  [bulk]
    type = ADGenericConstantMaterial
    prop_names = 'K G Gc psic l p'
    prop_values = '${K} ${G} ${Gc} ${psic} ${l} ${p}'
  []
  [indicator]
    type = ADDerivativeParsedMaterial
    f_name = I
    function = 'd'
    args = d
    derivative_order = 1
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [degradation]
    type = PowerDegradationFunction
    phase_field = d
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    parameter_names = 'p eta '
    parameter_values = '2 1e-9'
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
  [elasticity]
    type = SmallDeformationIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
    decomposition = NONE
  []
  [stress]
    type = ComputeSmallDeformationStress
    elasticity_model = elasticity
    output_properties = 'stress'
  []
  [crack_surface_density]
    type = CrackSurfaceDensity
    crack_geometric_function = alpha
    crack_surface_density = gamma
    normalization_constant = c0
    phase_field = d
    regularization_length = l
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+psie'
    args = 'd'
    material_property_names = 'alpha(d) g(d) Gc c0 l psie(d)'
    derivative_order = 1
  []
[]

[Postprocessors]
  [Jint]
    type = JIntegralDomainForm
    domain = q
    strain_energy_density = psie
    J_direction = '1 0 0'
  []
  [Jint_p]
    type = JIntegralDomainFormPressureWork
    domain = q
    phase_field = d
    indicator_function = I
    pressure = p
    J_direction = '1 0 0'
  []
  [JR]
    type = ParsedPostprocessor
    function = '(Jint+Jint_p)/${fparse (1+3*h/(4*l))*Gc/2}' #need to be updated as we change c0
    pp_names = 'Jint Jint_p'
  []
  [crack_length_half]
    type = ADElementIntegralMaterialProperty
    mat_prop = gamma
    outputs = none
  []
  [crack_length]
    type = ParsedPostprocessor
    function = '2*crack_length_half'
    pp_names = 'crack_length_half'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_type'
  petsc_options_value = 'lu       superlu_dist                  vinewtonrsls'
  automatic_scaling = true
  # line_search = none

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  end_time = 5
  [TimeStepper]
    type = ConstantDT
    dt = 0.025
    growth_factor = 1
  []
[]

[Outputs]
  file_base = 'gary_l_${l}'
  csv = true
  exodus = true
  print_linear_residuals = false
[]
