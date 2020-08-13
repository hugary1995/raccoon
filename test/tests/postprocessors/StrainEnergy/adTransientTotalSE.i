# Solves the problem:
#   div(stress) = rho u,tt ,   x in [-1, 1]*[-1, 1]
# with boundary conditions
#   u_x = u_y = 0     , on y = -1
#   sigma . n = 0.1*t , on y = 1
# Without Poisson effects, this is a quasi-1D problem, with u_x = 0,
# u_y = 0.1*(y+1). The total strain energy is then 4*0.5*(0.1*t)^2 = 0.02*t^2,
# with the factor of 4 representing the area of the domain.

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
    type = ADStrainEnergy
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
    file_base = 'adTransientstrainenergy'
    delimiter = ','
  []
[]
