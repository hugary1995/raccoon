Gc=22.2
l=0.35
psic=7.9

[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic}'
  [../]
[]

[Transfers]
  [./from_d]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
  [./to_E_el_active]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = to_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
  [../]
[]

[Mesh]
  [./fmg]
    type = FileMeshGenerator
    file = 'gold/half_notched_plate.msh'
  [../]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./stress]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d]
  [../]
  [./E_el_active]
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress]
    type = ADRankTwoScalarAux
    variable = 'stress'
    rank_two_tensor = 'stress'
    scalar_type = 'MaxPrincipal'
    execute_on = 'TIMESTEP_END'
  [../]
  [./E_el_active]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
  [../]
[]

[Kernels]
  [./inertia_x]
    type = InertialForce
    variable = 'disp_x'
  [../]
  [./inertia_y]
    type = InertialForce
    variable = 'disp_y'
  [../]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  [../]
[]

[BCs]
  [./xdisp]
    type = FunctionDirichletBC
    variable = 'disp_x'
    boundary = 'load'
    function = 'if(t<1e-6, 0.5*1.65e10*t*t, 1.65e4*t-0.5*1.65e-2)'
    preset = false
  [../]
  [./y_bot]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = '0'
  [../]
[]

[Materials]
  [./bulk]
    type = GenericConstantMaterial
    prop_names = 'density phase_field_regularization_length energy_release_rate critical_fracture_energy'
    prop_values = '8e-9 0.35 22.2 7.9'
  [../]
  [./elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 1.9e5
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
  [../]
  [./fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 1e-09
  [../]
[]

[Executioner]
  type = Transient
  dt = 5e-9
  end_time = 9e-5

  [./TimeIntegrator]
    type = CentralDifference
    solve_type = lumped
  [../]
[]

[Outputs]
  file_base = 'visualize'
  exodus = true
  interval = 20
[]
