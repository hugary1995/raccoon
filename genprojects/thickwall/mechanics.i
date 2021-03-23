Gc = 20000
l = 0.002
psic = 2.381e6


[MultiApps]
  [fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
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
    file = 'gold/thickwall.msh'
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
    type = ExplicitInertialForce
    variable = 'disp_x'
    density = 'reg_density'
  []
  [inertia_y]
    type = ExplicitInertialForce
    variable = 'disp_y'
    density = 'reg_density'
  []
  [solid_x]
    type = ADExplicitStressDivergenceTensors
    variable = 'disp_x'
    component = 0
    displacements = 'disp_x disp_y'
  []
  [solid_y]
    type = ADExplicitStressDivergenceTensors
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
    prop_values = '7850 0.002 20000 2.381e6'

    #type = ADGenericConstantMaterial
  #  prop_names = 'phase_field_regularization_length energy_release_rate '
  #               'critical_fracture_energy'
  #  prop_values = '0.002 20000 2.381e6'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 210e9
    poissons_ratio = 0.3
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

type = Transient
solve_type = 'NEWTON'
petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels '
                      '-snes_type'
petsc_options_value = 'asm      ilu          200         200                0                     '
                      'vinewtonrsls'
nl_abs_tol = 1e-08
nl_rel_tol = 1e-06
automatic_scaling = true

dt = 5e-8
end_time = 80e-6


#  type = Transient


  #[TimeIntegrator]
    #type = CentralDifference
    #solve_type = lumped

  #[]


[]

[Outputs]
  file_base = 'thickwallneg'
  exodus = true
  interval = 20
[]
