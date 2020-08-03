# Solves the problem:
#   div(stress) = 0 ,   x in [-1, 1]*[-1, 1]
# with boundary conditions
#   u_x = t , on x = -1
#   u_y = 0 , on x = 1
# This amounts to a rightward rigid body translation with unit velocity
# The kinetic energy is then always exactly 4*0.5*1^2 = 2.

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
  [density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = 1
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 1
    poissons_ratio = 0.3
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
    type = ADFunctionDirichletBC
    variable = 'disp_x'
    boundary = 'left'
    function = 't'
  []
  [fix_y]
    type = ADDirichletBC
    variable = 'disp_y'
    boundary = 'left'
    value = 0.0
  []
[]

[Postprocessors]
  [strain_energy]
    type = KineticEnergy
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 1
  num_steps = 2
[]

[Outputs]
  print_linear_residuals = false
  [csv]
    type = CSV
    file_base = 'kineticenergy'
    delimiter = ','
  []
[]
