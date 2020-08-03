# Solves the problem:
#   div(stress) = 0 ,   x in [-1, 1]*[-1, 1]
# with boundary conditions
#   u_x = u_y = 0   , on y = -1
#   sigma . n = 0.1 , on y = 1
# Without Poisson effects, this is a quasi-1D problem, with u_x = 0,
# u_y = 0.1*(y+1). The total strain energy is then 4*0.5*(0.1)^2 = 0.02,
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
    type = StressDivergenceTensors
    variable = 'disp_x'
    component = 0
  []
  [uy]
    type = StressDivergenceTensors
    variable = 'disp_y'
    component = 1
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0
  []
  [strain]
    type = ComputeSmallStrain
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[BCs]
  [fix_x]
    type = DirichletBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0.0
  []
  [fix_y]
    type = DirichletBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0.0
  []
  [traction]
    type = Pressure
    variable = 'disp_y'
    boundary = 'top'
    function = '0.1'
    component = 1
  []
[]

[Postprocessors]
  [strain_energy]
    type = StrainEnergy
  []
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
[]

[Outputs]
  print_linear_residuals = false
  [csv]
    type = CSV
    file_base = 'steadystrainenergy'
    delimiter = ','
  []
[]
