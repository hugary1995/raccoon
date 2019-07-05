[Modules]
  [./KLExpansion]
    [./E_crit]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '0.0228897927516298	-0.261995434966092	-1.75021236844679	-0.285650971595330	-0.831366511567624	-0.979206305167302	-1.15640165566400	-0.533557109315987	-2.00263573588306	0.964229422631628	0.520060101455458	-0.0200278516425381	-0.0347710860284830	-0.798163584564142	1.01868528212858	-0.133217479507735	-0.714530163787158	1.35138576842666	-0.224771056052584	-0.589029030720801	-0.293753597735416	-0.847926243637934	-1.12012830124373	2.52599969211831	1.65549759288735	0.307535159238252	-1.25711835935205	-0.865468030554804	-0.176534114231451	0.791416061628634	-1.33200442131525	-2.32986715580508	-1.44909729283874	0.333510833065806	0.391353604432901	0.451679418928238	-0.130284653145721'
      mean = 3e-05
      CV = 0.3
    [../]
    [./Gc]
      file_name = 'gold/KL/kl_info.txt'
      field_distribution = GAMMA
      perturbation = CUSTOM
      custom_Gaussian_weights = '0.183689095861942	-0.476153016619074	0.862021611556922	-1.36169447087075	0.455029556444334	-0.848709379933659	-0.334886938964048	0.552783345944550	1.03909065350496	-1.11763868326521	1.26065870912090	0.660143141046978	-0.0678655535426873	-0.195221197898754	-0.217606350143192	-0.303107621351741	0.0230456244251053	0.0512903558487747	0.826062790211596	1.52697668673337	0.466914435684700	-0.209713338388737	0.625190357087626	0.183227263001437	-1.02976754356662	0.949221831131023	0.307061919146703	0.135174942099456	0.515246335524849	0.261406324055383	-0.941485770955434	-0.162337672803828	-0.146054634331526	-0.532011376808821	1.68210359466318	-0.875729346160017	-0.483815050110121'
      mean = 8e-04
      CV = 0.3
    [../]
  [../]
[]

[MultiApps]
  [./mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./get_E_el]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
  [../]
  [./send_load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
  [../]
  [./send_d]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
  [./get_disp_x]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
  [../]
  [./get_disp_y]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
  [../]
[]

[Mesh]
  type = FileMesh
  file = 'gold/geo.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
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
    bounded_var = d
    upper = 1
    lower = d_ref
  [../]
[]

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_var = E_el
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
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
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = 1e-4

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]
