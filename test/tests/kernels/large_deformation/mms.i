[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  uniform_refine = 0
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Functions]
  [./ux]
    type = ParsedFunction
    value = '0.1*(sin(0.5*pi*x) + y^3)'
  [../]
  [./uy]
    type = ParsedFunction
    value = '0.1*(sin(0.5*pi*y) + x^3)'
  [../]
[]

[Kernels]
  [./solid_x]
    type = PiolaKirchhoffStressDivergence
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = PiolaKirchhoffStressDivergence
    variable = disp_y
    component = 1
  [../]
  [./bx]
    type = BodyForce
    variable = disp_x
    function = '(pi^2*sin((pi*x)/2)*((63*x^4)/2000 + (27*y^4)/2000 + (7*((pi*cos((pi*x)/2))/20 + 1)^2)/20 + (3*((pi*cos((pi*y)/2))/20 + 1)^2)/20 - 1/2))/40
     - ((3*y*((pi*cos((pi*x)/2))/20 + 1))/25 - (3*x^2*pi^2*sin((pi*y)/2))/2000)*((pi*cos((pi*x)/2))/20 + 1)
      - (3*y*((27*x^4)/2000 + (63*y^4)/2000 + (3*((pi*cos((pi*x)/2))/20 + 1)^2)/20 + (7*((pi*cos((pi*y)/2))/20 + 1)^2)/20 - 1/2))/5
       - (3*y^2*((3*x*((pi*cos((pi*y)/2))/20 + 1))/25 - (3*y^2*pi^2*sin((pi*x)/2))/2000))/10
        - ((pi*cos((pi*x)/2))/20 + 1)*((63*x^3)/500 - (7*pi^2*sin((pi*x)/2)*((pi*cos((pi*x)/2))/20 + 1))/400)
         - (3*y^2*((63*y^3)/500 - (7*pi^2*sin((pi*y)/2)*((pi*cos((pi*y)/2))/20 + 1))/400))/10'
  [../]
  [./by]
    type = BodyForce
    variable = disp_y
    function = '(pi^2*sin((pi*y)/2)*((27*x^4)/2000 + (63*y^4)/2000 + (3*((pi*cos((pi*x)/2))/20 + 1)^2)/20 + (7*((pi*cos((pi*y)/2))/20 + 1)^2)/20 - 1/2))/40
     - ((3*x*((pi*cos((pi*y)/2))/20 + 1))/25 - (3*y^2*pi^2*sin((pi*x)/2))/2000)*((pi*cos((pi*y)/2))/20 + 1)
      - (3*x*((63*x^4)/2000 + (27*y^4)/2000 + (7*((pi*cos((pi*x)/2))/20 + 1)^2)/20 + (3*((pi*cos((pi*y)/2))/20 + 1)^2)/20 - 1/2))/5
       - (3*x^2*((3*y*((pi*cos((pi*x)/2))/20 + 1))/25 - (3*x^2*pi^2*sin((pi*y)/2))/2000))/10
        - ((pi*cos((pi*y)/2))/20 + 1)*((63*y^3)/500 - (7*pi^2*sin((pi*y)/2)*((pi*cos((pi*y)/2))/20 + 1))/400)
         - (3*x^2*((63*x^3)/500 - (7*pi^2*sin((pi*x)/2)*((pi*cos((pi*x)/2))/20 + 1))/400))/10'
  [../]
[]

[BCs]
  [./xdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'top bottom left right'
    function = ux
  [../]
  [./ydisp]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'top bottom left right'
    function = uy
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.52
    poissons_ratio = 0.3
  [../]
  [./greem_strain]
    type = GreenStrain
  [../]
  [./second_piola_kirchhoff_stress]
    type = GreenStrainLinearElasticStress
  [../]
[]

[Postprocessors]
  [./error_ux]
    type = ElementL2Error
    variable = disp_x
    function = ux
  [../]
  [./error_uy]
    type = ElementL2Error
    variable = disp_y
    function = uy
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels'
  petsc_options_value = 'asm      ilu          200         200                0                    '

  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-50
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
