Gc = 20
l = 0.16
psic = 0.4498


[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'domain.msh'
    cli_args = 'Gc=${Gc};l=${l};psic=${psic}'
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
    file = 'gold/QuarterHairline.msh'
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
  [forcing]
    type = FunctionDirichletBC
    variable = 'disp_y'
    boundary = 'Top'
    #//function = '-t/50'
    function = '-0.01*t'
    #//'if(t<1e-6, 0.5*1.65e10*t*t, 1.65e4*t-0.5*1.65e-2)'
    preset = false
    use_displaced_mesh = true
  []

  [FixedHole_x]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'Hole'
    value = 0
    use_displaced_mesh = true
  []
  [FixedHole_y]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'Hole'
    value = 0
    use_displaced_mesh = true
  []
[]
[Materials]
#[density]
#  type = GenericConstantMaterial
#  prop_names = 'density'
  #prop_values = 7850
#[]
  [bulk]
    type = ADGenericConstantMaterial
   prop_names = 'density phase_field_regularization_length energy_release_rate '
                 'critical_fracture_energy'
    prop_values = '0.2 0.16 20 0.4498'

    #type = ADGenericConstantMaterial
  #  prop_names = 'phase_field_regularization_length energy_release_rate '
  #               'critical_fracture_energy'
  #  prop_values = '0.002 20000 2.381e6'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 7.25e4
    poissons_ratio = 0.25
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
  []
  [reg_density]
    type = MaterialConverter
    ad_props_in = 'density'
    reg_props_out = 'reg_density'
  []
[]

[Executioner]

dt = 0.90
end_time = 150


  type = Transient


  [TimeIntegrator]
    type = CentralDifference
    solve_type = lumped

  []


[]

[Outputs]
  file_base = 'hairline'
  exodus = true
  interval = 1
[]
