Gc = 20000
l = 0.002


[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fractureQuartWeibull.i'
    cli_args = 'Gc=${Gc};l=${l}'
  []
[]

[Distributions]
  [Weibull]
    type = Weibull
    scale = '2.13e11'
    shape = '57.7753'
    location = 0
  []
[]

[ICs]
  [E]
    type = RandomIC
    variable = E
    distribution = Weibull
  []
[]



[Transfers]
  [from_d]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
  []
  [EtoFracture]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = to_multiapp
    source_variable = 'E'
    variable = 'E'
[]
  [to_E_el_active]
    type = MultiAppCopyTransfer
    multi_app = 'fracture'
    direction = to_multiapp
    source_variable = 'E_el_active'
    variable = 'E_el_active'
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = 'gold/quarter.msh'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [stress]
    order = CONSTANT
    family = MONOMIAL
  []
  [d]
  []
  [E_el_active]
    family = MONOMIAL
  []
  [E]
  []
[]

[AuxKernels]
  [stress]
    type = ADRankTwoScalarAux
    variable = 'stress'
    rank_two_tensor = 'stress'
    scalar_type = 'MaxPrincipal'
    execute_on = 'TIMESTEP_END'
  []
  [E_el_active]
    type = ADMaterialRealAux
    variable = 'E_el_active'
    property = 'E_el_active'
  []
[]

[Kernels]
  [inertia_x]
    type = InertialForce
    variable = 'disp_x'
    density = 'reg_density'
  []
  [inertia_y]
    type = InertialForce
    variable = 'disp_y'
    density = 'reg_density'
  []
  [solid_x]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  []
  [solid_y]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
    displacements = 'disp_x disp_y'
  []
[]

[BCs]
  [./Pressure]
  [innerpressure]
    type = Pressure
    boundary = 'Inner'
    function = '(4e8)*exp(t/1e-4)'
    displacements = 'disp_x disp_y'
  []
  [../]

  [VertSurfNoDisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'Vertical'
    value = 0
  []
  [HorzSurfNoDisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'Horizontal'
    value = 0
  []
[]
[Materials]

  [critical_fracture_energy]
    type = ADParsedMaterial
    f_name = 'critical_fracture_energy'
    material_property_names = 'youngs_modulus'
    args = 'E'
    function = '((1e9)^2)/E'
  []


  [bulk]
    type = ADGenericConstantMaterial
   prop_names = 'density phase_field_regularization_length energy_release_rate poissons_ratio'

    prop_values = '7850 0.002 20000 0.3'

  []

  [youngs_modulus]
    type = ADParsedMaterial
      f_name = 'E'
      args = 'E'
      function = 'E'
  []


  [elasticity_tensor]
    type = ADComputeVariableIsotropicElasticityTensor
    youngs_modulus = E
    poissons_ratio = poissons_ratio
  []
  [strain]
    type = ADComputeSmallStrain
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = SmallStrainDegradedElasticPK2Stress_StrainSpectral
    d = 'd'
  []
  [fracture_properties]
    type = ADFractureMaterial
    local_dissipation_norm = 8/3
  []
  [degradation]
    type = LorentzDegradation
    d = 'd'
    residual_degradation = 1e-09
    critical_fracture_energy_name = critical_fracture_energy
  []
  [reg_density]
    type = MaterialConverter
    ad_props_in = 'density'
    reg_props_out = 'reg_density'
  []
[]

[Executioner]

dt = 5e-9
end_time = 80e-6


  type = Transient


  [TimeIntegrator]
    type = CentralDifference
    solve_type = lumped

  []


[]

[Outputs]
  file_base = 'Weibull'
  [Exodus]
    type = Exodus

    show_material_properties = 'critical_fracture_energy'
    output_material_properties = true
  []
  interval = 20
[]
