[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'gold/mesh.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d]
  [../]
[]

# [Modules]
#   [./PhaseFieldFracture]
#     [./CohesiveFracture]
#       [./d]
#         viscosity = 1e-6
#         Gc = 2.7
#         L = 0.015
#         p = 10
#         fracture_energy_barrier = 5
#         residual_degradation = 1e-12
#       [../]
#     [../]
#     [./ElasticCoupling]
#       [./all]
#         damage_fields = 'd'
#         strain = FINITE
#         decomposition = NO_DECOMP
#         irreversibility = NONE
#       [../]
#     [../]
#   [../]
# []

[AuxVariables]
  [./dummy]
  [../]
[]

[Bounds]
  [./d]
    type = BoundsAux
    bounded_variable = d
    variable = dummy
    upper = 1.0
    lower = 0.0
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
  [./damage]
    type = PFFractureADTest
    variable = d
  [../]
[]

[BCs]
  [./xdisp_top]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'top'
    function = '(x-1)*(cos(t)-1)+0.5*sin(t)'
  [../]
  [./ydisp_top]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 'top'
    function = '-(x-1)*sin(t)+0.5*(cos(t)-1)'
  [../]
  [./xdisp_right]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'right_upper'
    function = '(y-0.5)*sin(t)'
  [../]
  [./ydisp_right]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 'right_upper'
    function = '(y-0.5)*(cos(t)-1)'
  [../]
  [./xfix_bottom]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'bottom'
    function = '0'
  [../]
  [./yfix_bottom]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 'bottom'
    function = '0'
  [../]
  [./xfix_right]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'right_lower'
    function = '0'
  [../]
  [./yfix_right]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 'right_lower'
    function = '0'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = HLCGStrain
  [../]
  [./stress]
    type = HLCGStrainDegradedPK1Stress
    history = false
  [../]
  [./fracture]
    type = FractureMaterial
    d = d
    Gc = 2.7
    L = 0.015
    local_dissipation_norm = 0.6666666666666667
  [../]
  [./degradation]
    type = DegradationLorentz
    d = d
    p = 10
  [../]
  [./barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'b'
    prop_values = '5'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'

  end_time = 1.57
  dtmax = 1e-3
  dtmin = 1e-5

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-5
    optimal_iterations = 5
    iteration_window = 1
    cutback_factor = 0.5
    growth_factor = 1.1
  [../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  interval = 1
[]
