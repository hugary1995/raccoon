E = 16
nu = 0.1812

K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'

Gc = 9.49e-9
l = 0.75

domain_size = 80.0
crack_size = 8.0
end_time = 4.0

[Mesh]
  [generated_mesh]
    type = GeneratedMeshGenerator
    dim = 2
    elem_type = QUAD4
    nx = ${domain_size}
    ny = ${domain_size}
    nz = 0
    xmin = 0
    xmax = ${domain_size}
    ymin = 0
    ymax = ${domain_size}
    zmin = 0
    zmax = 0
  []
[]

[ICs]
  [d]
    type = CohesiveDamageIC
    variable = d
    d0 = 1.0
    l = ${l}
    x1 = '${fparse 0.5*(domain_size-crack_size)}'
    y1 = '${fparse 0.5*domain_size}'
    z1 = 0.0
    x2 = '${fparse 0.5*(domain_size+crack_size)}'
    y2 = '${fparse 0.5*domain_size}'
    z2 = 0
  []
[]

[MultiApps]
  [mechanical]
    type = TransientMultiApp
    input_files = 'poromechanics.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_BEGIN'
    cli_args = 'G=${G};K=${K};l=${l};domain_size=${domain_size};end_time=${end_time};crack_size=${cra'
               'ck_size}'
  []
[]

[Transfers]
  [get_E_el_active]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'psie_active'
    variable = 'psie_active'
  []
  [send_d]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  []
[]

[Variables]
  [d]
  []
[]

[AuxVariables]
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
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
  [diff]
    type = ADPFFDiffusion
    variable = 'd'
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

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'Gc l'
    prop_values = '${Gc} ${l}'
  []
  [degradation]
    type = PowerDegradationFunction
    f_name = g
    function = (1-d)^p*(1-eta)+eta
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 1e-3'
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd^2'
    phase_field = d
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'alpha*Gc/c0/l+g*psie_active'
    args = 'd psie_active'
    material_property_names = 'alpha(d) g(d) Gc c0 l'
    derivative_order = 1
  []
[]

[Postprocessors]
  [norm_d]
    type = ElementL2Norm
    variable = 'd'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu       vinewtonrsls'

  dt = 1e-1
  end_time = ${end_time}
  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  fixed_point_max_its = 20
  custom_pp = 'norm_d'
  custom_abs_tol = 1e-03
  custom_rel_tol = 1e-03
  disable_fixed_point_residual_norm_check = true
  accept_on_max_fixed_point_iteration = false
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
[]
