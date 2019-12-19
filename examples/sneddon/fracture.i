# [Mesh]
#   type = FileMesh
#   file = 'gold/geo.msh'
# []

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 16
  ny = 16
  xmin = 0
  xmax = 4
  ymin = 0
  ymax = 4
[]

[Adaptivity]
  steps = 1
  marker = box
  max_h_level = 5
  initial_steps = 5
  stop_time = 1.0e-10
  [./Markers]
    [./box]
      bottom_left = '1.6 1.8 0'
      inside = refine
      top_right = '2.4 2.2 0'
      outside = do_nothing
      type = BoxMarker
    [../]
  [../]
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./p]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = d_ref
  [../]
[]

[Kernels]
  [./diff]
    type = PhaseFieldFractureEvolutionDiffusion
    variable = d
  [../]
  [./react]
    type = PhaseFieldFractureEvolutionReaction
    variable = d
    driving_energy_name = 'E_el'
  [../]
  [./pressure]
    type = PhaseFieldFractureEvolutionPressure
    variable = d
    p = p
    displacements = 'disp_x disp_y'
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.2
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    history = false
  [../]
  [./fracture_properties]
    type = GenericConstantMaterial
    prop_names = 'energy_release_rate phase_field_regularization_length b'
    prop_values = '1 0.0221 0'
  [../]
  [./local_dissipation]
    type = QuadraticLocalDissipation
    d = d
  [../]
  [./phase_field_properties]
    type = FractureMaterial
    local_dissipation_norm = 2
  [../]
  [./degradation]
    type = QuadraticDegradation
    d = d
    residual_degradation = 1e-12
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 1e-6

  nl_abs_tol = 1e-08
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
[]
