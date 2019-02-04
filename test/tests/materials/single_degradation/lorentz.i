[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./d]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = d
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = d
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = d
    boundary = right
    value = 1
  [../]
[]

[Materials]
  [./lorentz]
    type = Degradation
    f_name = 'g_d'
    args = 'd'
    material_property_names = 'E Gc_d sigmac_d l_d'
    constant_names       = 'eta p'
    constant_expressions = '0.1 1'
    function = '(1-d)^2/(1+(1.5*E*Gc_d/sigmac_d/sigmac_d/l_d-2)*d+(1+p*1.5*E*Gc_d/sigmac_d/sigmac_d/l_d)*d^2)*(1-eta)+eta'
    outputs = exodus
  [../]
  [./other]
    type = GenericConstantMaterial
    prop_names  = 'E   Gc_d sigmac_d l_d'
    prop_values = '100 1    10       0.2'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
