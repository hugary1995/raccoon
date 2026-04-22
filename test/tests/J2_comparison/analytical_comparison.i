E = 200e3
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

sigma_y = 200
H = 20e3
disp_rate = 0.005
t_yield = '${fparse sigma_y / (E * disp_rate)}'

end_time = 1.0
dt = 0.1

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 1
    ny = 1
    nz = 1
    elem_type = HEX8
  []
[]

[Variables]
  [disp_x][]
  [disp_y][]
  [disp_z][]
[]

[AuxVariables]
  [d][]
[]

[Kernels]
  [x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
  []
  [y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
  []
  [z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
  []
[]

# Small-strain analytical solution for uniaxial tension with linear isotropic hardening:
#   ep = 0                                    for t <= t_yield
#   ep = (E * disp_rate * t - sigma_y)/(E+H)  for t >  t_yield
[Functions]
  [ep_analytical_fn]
    type = ParsedFunction
    expression = 'if(t <= t_yield, 0, (E * disp_rate * t - sigma_y) / (E + H))'
    symbol_names = 'E sigma_y H disp_rate t_yield'
    symbol_values = '${E} ${sigma_y} ${H} ${disp_rate} ${t_yield}'
  []
[]

[Materials]
  [defgrad]
    type = ComputeDeformationGradient
  []
  [constants]
    type = ADGenericConstantMaterial
    prop_names = 'K G yield_stress hardening_modulus'
    prop_values = '${K} ${G} ${sigma_y} ${H}'
  []
  [nodeg]
    type = NoDegradation
    phase_field = d
    property_name = nodeg
  []
  [hencky]
    type = CNHIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = nodeg
    decomposition = NONE
  []
  [J2]
    type = LargeDeformationJ2PlasticityBeBar
    phase_field = d
    hardening_model = hardening
    relative_tolerance = 1e-10
    apply_strain_energy_split = false
  []
  [hardening]
    type = LinearHardening
    yield_stress = yield_stress
    hardening_modulus = hardening_modulus
    phase_field = d
    degradation_function = nodeg
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
    preset = false
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
    preset = false
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = front
    value = 0
    preset = false
  []
  [ypull]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = top
    function = '${disp_rate} * t'
  []
[]

[Postprocessors]
  [ep_numerical]
    type = ADElementIntegralMaterialProperty
    mat_prop = effective_plastic_strain
    use_displaced_mesh = false
  []
  [ep_analytical]
    type = FunctionValuePostprocessor
    function = ep_analytical_fn
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-12
  nl_max_its = 50

  dt = ${dt}
  end_time = ${end_time}
  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  [csv]
    type = CSV
    file_base = analytical_comparison
  []
[]
