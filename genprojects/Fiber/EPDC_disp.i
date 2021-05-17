# regularization length
l = 0.2
# fracture toughness
Gc = 20
# critical fracture energy
psic = 3.7
# beta epsilon_0 are paramters of the coalescence dissipation, see paper
# basically we use a function of beta, e0, ep to scale Gc
beta = 0.01
e0 = 0.1
# plastic yield stress
sigmay = 345
# plastic hardening modulus
Hp = 1000

# beta_effective is used to scale Gc and psic
beta_effective = '${fparse 1 - (1 - beta) * (1 - exp(-0.3 / e0))}'
density = 2.7e-3

[GlobalParams]
  displacements = 'disp_x disp_y'
  volumetric_locking_correction = false
[]

[MultiApps]
  [damage]
    type = TransientMultiApp
    app_type = raccoonApp
    input_files = 'EPDC_d.i'
    cli_args = 'l=${l};Gc=${Gc};psic=${psic};beta_effective=${beta_effective}'
  []
[]

[Transfers]
  [to_sub_E_el_active]
    type = MultiAppCopyTransfer
    direction = to_multiapp
    multi_app = damage
    source_variable = 'E_el_active'
    variable = 'E_el_active'
  []
  [to_sub_coalescence_mobility]
    type = MultiAppCopyTransfer
    direction = to_multiapp
    multi_app = damage
    source_variable = 'coalescence_mobility'
    variable = 'coalescence_mobility'
  []
  [from_sub_d]
    type = MultiAppCopyTransfer
    direction = from_multiapp
    multi_app = damage
    source_variable = 'd'
    variable = 'd'
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/domain.msh'
  []
  [bbnsg]
    type = BoundingBoxNodeSetGenerator
    bottom_left = '-8.01 -8.01 0'
    top_right = '-7.99 -7.99 0'
    new_boundary = 'fixedpoint'
    input = fmg
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
  []
[]

[Kernels]
  [solid_x]
    type = ADDynamicStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    # zeta = 0.5
    use_displaced_mesh = true
  []
  [solid_y]
    type = ADDynamicStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    # zeta = 0.5
    use_displaced_mesh = true
  []
  [plane_stress]
    type = ADWeakPlaneStress
    variable = 'strain_zz'
    displacements = 'disp_x disp_y'
    use_displaced_mesh = true

  []
[]

[Materials]
  [stiffness]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 7.25e4
    poissons_ratio = 0.33
  []
  [strain]
    type = PlaneGreenStrain
    out_of_plane_strain = 'strain_zz'
    outputs = 'exodus'
    output_properties = 'mechanical_strain'

  []
  [elastic_plastic]
    # stress calculator, which is also responsible for return mapping
    type = CNHDegradedElasticPlasticPK1Stress_LinearHardening
    d = 'd'
    degradation_mat = 'g'
    plastic_degradation_mat = 'gp'
    yield_stress = ${sigmay}
    slope = ${Hp}
    enforce_isochoricity = true
    use_cauchy_stress = true
    outputs = 'exodus'
    output_properties = 'E_el_active W_pl'
  []
  [bulk]
    type = GenericConstantMaterial
    prop_names = 'density phase_field_regularization_length energy_release_rate '
                 'critical_fracture_energy'
    prop_values = '${density} ${l} ${fparse Gc / beta_effective} ${fparse psic / beta_effective}'
  []
  # phase field stuff here on
  [local_dissipation]
    type = LinearLocalDissipation
    d = 'd'
  []
  [fracture_properties]
    type = FractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 1e-4
  []
  [plastic_degradation]
    # degradation function for plasticity
    # in this case, g^p(d) = 1
    type = NoDegradation
    degradation_name = 'gp'
    d = 'd'
    residual_degradation = 0
  []
  [coalescence]
    # This material calculates the mobility based on the scaled Gc,
    # as a function of plastic deformation ep
    type = CoalescenceEnergy
    beta = ${beta}
    e0 = ${e0}
    outputs = exodus
    output_properties = 'coalescence_mobility'
  []
[]

[BCs]
  [forcing]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'Top'
    #//function = '-t/50'
    function = 'if(t<40, -t/50, -40/50-(t/60-40/60))'
    #//'if(t<1e-6, 0.5*1.65e10*t*t, 1.65e4*t-0.5*1.65e-2)'
    preset = false
    use_displaced_mesh = true
  []
#  [BottomLeftFixed_x]
  #  type = DirichletBC
#    variable = 'disp_x'
#    boundary = 'BottomLeft'
#    value = 0
#  []
#  [BottomLeftFixed_y]
#    type = DirichletBC
#    variable = 'disp_y'
#    boundary = 'BottomLeft'
  #  value = 0
  #  []
  [BottomFixed_y]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'Bottom'
    value = 0
    use_displaced_mesh = true
  []
[]
[NodalKernels]
  [ux]
      type = PenaltyDirichletNodalKernel
    penalty = 1e16
    variable = 'disp_x'
    boundary = 'fixedpoint'
  []
  [uy]
      type = PenaltyDirichletNodalKernel
    penalty = 1e16
    variable = 'disp_y'
    boundary = 'fixedpoint'
  []
[]

[Executioner]
  type = Transient

  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu       superlu_dist                 '

  nl_rel_tol = 1e-06
  nl_abs_tol = 1e-08

  dt = 0.90
  end_time = 150

  automatic_scaling = true
[]

[Outputs]
  print_linear_residuals = false
  print_linear_converged_reason = false
  print_nonlinear_converged_reason = false
  [exodus]
    type = Exodus
    interval = 1
    file_base = 'Square_FiniteDisp_beta_${beta}_e0_${e0}'
    #show_material_properties = 'strain'
    #output_material_properties = true
  []
[]
