a = 250
R = 4

psic = 15
Gc = 1.38e5
l = '${fparse R * a}'

E = 6.88e4
nu = 0.33
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

sigma_y = 320
n = 5
ep0 = 0.01

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    xmax = ${a}
    ymax = ${a}
    zmax = ${a}
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
  [d]
  []
[]

[AuxVariables]
  [bounds_dummy]
  []
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [F]
    order = CONSTANT
    family = MONOMIAL
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

[AuxKernels]
  [stress]
    type = ADRankTwoAux
    variable = stress
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
  []
  [F]
    type = ADRankTwoAux
    variable = F
    rank_two_tensor = deformation_gradient
    index_i = 1
    index_j = 1
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
  [solid_z]
    type = ADStressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
  []
  [pff_diff]
    type = ADPFFDiffusion
    variable = d
  []
  [pff_source]
    type = ADPFFSource
    variable = d
    free_energy = psi
  []
[]

[Materials]
  [bulk_properties]
    type = ADGenericConstantMaterial
    prop_names = 'K G l Gc psic'
    prop_values = '${K} ${G} ${l} ${Gc} ${psic}'
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = g
    function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
  []
  [defgrad]
    type = ComputeDeformationGradient
  []
  [hencky]
    type = HenckyIsotropicElasticity
    bulk_modulus = K
    shear_modulus = G
    phase_field = d
    degradation_function = g
  []
  [power_law_hardening]
    type = PowerLawHardening
    yield_stress = ${sigma_y}
    exponent = ${n}
    reference_plastic_strain = ${ep0}
    phase_field = d
    degradation_function = g
  []
  [J2]
    type = LargeDeformationJ2Plasticity
    hardening_model = power_law_hardening
  []
  [stress]
    type = ComputeLargeDeformationStress
    elasticity_model = hencky
    plasticity_model = J2
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+psie+psip'
    args = d
    material_property_names = 'alpha(d) g(d) Gc c0 l psie(d) psip(d)'
    derivative_order = 1
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = 'back'
    value = 0
  []
  [ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top'
    function = 't'
    preset = false
  []
[]

[Postprocessors]
  [F]
    type = ElementAverageValue
    variable = F
  []
  [stress]
    type = ElementAverageValue
    variable = stress
  []
  [d]
    type = ElementAverageValue
    variable = d
  []
  [ep]
    type = ADElementAverageMaterialProperty
    mat_prop = effective_plastic_strain
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -snes_type   -pc_factor_shift_type -pc_factor_shift_amount'
  petsc_options_value = 'lu       vinewtonrsls NONZERO               1e-10'

  line_search = none

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10
  nl_max_its = 50

  dt = '${fparse 0.0001 * a}'
  end_time = '${fparse 0.1 * a}'

  automatic_scaling = true

  abort_on_solve_fail = true
[]

[Outputs]
  file_base = stress_deformation
  print_linear_residuals = false
  csv = true
  exodus = true
[]
