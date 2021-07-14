[Problem]
  kernel_coverage_check = false
  material_coverage_check = false
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = ${mesh}
  []
[]

[Variables]
  [d]
    block = oxide
    [InitialCondition]
      type = FunctionIC
      function = 'theta:=atan(y/x); if(theta>30*pi/180 & theta<60*pi/180 & (abs(z-7.5)<0.25 | '
                 'abs(z-22.5)<0.25), 1, 0)'
    []
  []
[]

[AuxVariables]
  [bounds_dummy]
    block = oxide
  []
  [psie_active]
    order = CONSTANT
    family = MONOMIAL
  []
  [effective_creep_strain]
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
    block = oxide
  []
  [upper]
    type = ConstantBoundsAux
    variable = bounds_dummy
    bounded_variable = d
    bound_type = upper
    bound_value = 1
    block = oxide
  []
[]

[Kernels]
  [pff_diff]
    type = ADPFFDiffusion
    variable = d
    block = oxide
  []
  [pff_source]
    type = ADPFFSource
    variable = d
    free_energy = psi
    block = oxide
  []
[]

[Materials]
  [fracture_properties]
    type = ADGenericConstantMaterial
    prop_names = 'l Gc psic'
    prop_values = '${l} ${Gc} ${psic}'
    block = oxide
  []
  [gc]
    type = ADParsedMaterial
    f_name = gc
    args = effective_creep_strain
    function = '1-(1-beta)*(1-exp(-effective_creep_strain/ep0))'
    constant_names = 'beta ep0'
    constant_expressions = '${beta} ${ep0}'
    block = oxide
  []
  [degradation]
    type = RationalDegradationFunction
    f_name = gip
    function = (1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta
    phase_field = d
    material_property_names = 'Gc psic xi c0 l '
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 1 0 1e-6'
    block = oxide
  []
  [crack_geometric]
    type = CrackGeometricFunction
    f_name = alpha
    function = 'd'
    phase_field = d
    block = oxide
  []
  [psi]
    type = ADDerivativeParsedMaterial
    f_name = psi
    function = 'gc*alpha*Gc/c0/l+gip*psie_active'
    # function = 'gc*alpha*Gc/c0/l'
    args = 'd psie_active'
    material_property_names = 'alpha(d) gip(d) Gc c0 l gc'
    derivative_order = 1
    block = oxide
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'

  nl_rel_tol = 1e-08
  nl_abs_tol = 1e-10

  dt = 1e20

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  exodus = true
[]
