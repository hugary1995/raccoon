[Modules]
  [./KLExpansion]
    [./E_crit]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '1.37029854009523	-1.71151641885370	-0.102242446085491	-0.241447041607358	0.319206739165502	0.312858596637428	-0.864879917324457	-0.0300512961962686	-0.164879019209038	0.627707287528727	1.09326566903948	1.10927329761440	-0.863652821988714	0.0773590911304249	-1.21411704361541	-1.11350074148676	-0.00684932810334806	1.53263030828475	-0.769665913753682	0.371378812760058	-0.225584402271252	1.11735613881447	-1.08906429505224	0.0325574641649735	0.552527021112224	1.10061021788087	1.54421189550395	0.0859311331754255	-1.49159031063761	-0.742301837259857	-1.06158173331999	2.35045722400204	-0.615601881466894	0.748076783703985	-0.192418510588264	0.888610425420721	-0.764849236567874	-1.40226896933876'
      mean = 3e-05
      CV = 0.3
    [../]
    [./Gc]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '-1.42237592509150	0.488193909859941	-0.177375156618825	-0.196053487807333	1.41931015064255	0.291584373984183	0.197811053464361	1.58769908997406	-0.804465956349547	0.696624415849607	0.835088165072682	-0.243715140377952	0.215670086403744	-1.16584393148205	-1.14795277889859	0.104874716016494	0.722254032225002	2.58549125261624	-0.666890670701386	0.187331024578940	-0.0824944253709554	-1.93302291785099	-0.438966153934773	-1.79467884145512	0.840375529753905	-0.888032082329010	0.100092833139322	-0.544528929990548	0.303520794649354	-0.600326562133734	0.489965321173948	0.739363123604474	1.71188778298155	-0.194123535758265	-2.13835526943994	-0.839588747336614	1.35459432800464	-1.07215528838425'
      mean = 8e-04
      CV = 0.3
    [../]
  [../]
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./strain_zz]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./E_el]
    type = MaterialRealAux
    property = 'E_el'
    variable = 'E_el'
    execute_on = 'TIMESTEP_END'
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
  [./plane_stress]
    type = ADWeakPlaneStress
    variable = strain_zz
  [../]
  [./react_x]
    type = DegradedCoefReaction
    variable = disp_x
    coefficient = 0.1
  [../]
  [./react_y]
    type = DegradedCoefReaction
    variable = disp_y
    coefficient = 0.1
  [../]
[]

[BCs]
  [./Periodic]
    [./x_left_right]
      variable = disp_x
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./y_left_right]
      variable = disp_y
      primary = 'left'
      secondary = 'right'
      translation = '100 0 0'
    [../]
    [./x_top_bottom]
      variable = disp_x
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
    [./y_top_bottom]
      variable = disp_y
      primary = 'top'
      secondary = 'bottom'
      translation = '0 -100 0'
    [../]
  [../]
  [./x_pin]
    type = DirichletBC
    variable = disp_x
    value = 0
    boundary = 'center'
  [../]
  [./y_pin]
    type = DirichletBC
    variable = disp_y
    value = 0
    boundary = 'center pin'
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    initial_stress_yy = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.2
    youngs_modulus = 4
  [../]
  [./strain]
    type = ADComputePlaneSmallStrain
    out_of_plane_strain = strain_zz
    eigenstrain_names = is
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_NoSplit
    d = d
    history = false
  [../]
  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = 'E_crit'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = 'Gc'
    L = 0.5
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  # petsc_options_iname = '-pc_type'
  # petsc_options_value = 'lu'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                0                    '
  dt = 1e-4

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06

  automatic_scaling = true
[]

[Outputs]
  hide = 'load'
  exodus = true
  print_linear_residuals = false
[]
