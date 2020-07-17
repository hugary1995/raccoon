[Mesh]
  type = GeneratedMesh
  dim = 2
  xmin = -1
  xmax = 1
  ymin = -1
  ymax = 1
  nx = 10
  ny = 10
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[Kernels]
  [ux]
    type = ADStressDivergenceTensors
    variable = 'disp_x'
    component = 0
  []
  [uy]
    type = ADStressDivergenceTensors
    variable = 'disp_y'
    component = 1
  []
[]

[Materials]
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0
  []
  [strain]
    type = ADComputeSmallStrain
  []
  [stress]
    type = ADComputeLinearElasticStress
  []
[]

[BCs]
  [fix_x]
    type = ADDirichletBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0.0
  []
  [fix_y]
    type = ADDirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0.0
  []
  [traction]
    type = ADPressure
    variable = 'disp_y'
    boundary = 'top'
    function = '0.1*t'
    component = 1
  []
[]

[Postprocessors]
  [strain_energy]
    type = StrainEnergy
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 1
  num_steps = 4
[]

[Outputs]
  print_linear_residuals = false
  [csv]
    type = CSV
    file_base = 'transientstrainenergy'
    delimiter = ','
  []
[]
