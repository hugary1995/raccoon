[Modules]
  [./KLExpansion]
    [./E_crit]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '-1.94884717689890	1.02049801445265	0.861716302393419	0.00116208348351385	-0.0708372131604802	-2.48628392070328	0.581172322675923	-2.19243491996591	-2.31928030664330	0.0799337102984397	-0.948480983570505	0.411490621423374	0.676977805684030	0.857732545205355	-0.691159125382991	0.449377623166851	0.100633350315076	0.826069998469923	0.536157079925919	0.897888425985076	-0.131937867924581	-0.147201456151267	1.00777340530544	-2.12365546241575	-0.504586405514010	-1.27059444980866	-0.382584802707648	0.648679262048621	0.825727149241758	-1.01494364268014	-0.471069912683167	0.137024874130050	-0.291863375753573	0.301818555261006	0.399930942955802	-0.929961558940129	-0.176830265929232	-2.13209459916153	1.14536171051847	-0.629090760994156	-1.20384997400222	-0.253944683422812	-1.42864686463210	-0.0208576177016003	-0.560664996528185	2.17777870918415	1.13846538732960	-2.49688650318450	0.441326931727609	-1.39813787581107'
      mean = 3e-05
      CV = 0.3
    [../]
    [./Gc]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '-0.255055179880806	0.164404073318729	0.747734028808123	-0.273046949402907	1.57630014654608	-0.480937151778845	0.327512120829686	0.664734120627595	0.0851885927211602	0.880952785381048	0.323213137847598	-0.784146183664088	-1.80537335138609	1.85859294855571	-0.604530088774459	0.103359722310648	0.563166955145978	0.113596996734002	-0.904726212798032	-0.467714582451856	-0.124889947223107	1.47895849150403	-0.860815690006961	0.784668467287822	0.308623139948530	-0.233860042129153	-1.05697274596013	-0.284140954626164	-0.0866902824593051	-1.46939507448636	0.192182244870785	-0.822293276290175	-0.0942405879597235	0.336213340954240	-0.904654059246837	-0.288256361205696	0.350062757534178	-1.83585914250431	1.03597590824548	2.42446114493936	0.959400509407976	-0.315771995009409	0.428622679859396	-1.03598477851339	1.87786546049586	0.940704403352132	0.787345779935246	-0.875874261956672	0.319949134382331	-0.558294284850904'
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
