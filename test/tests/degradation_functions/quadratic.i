[Problem]
  solve = false
[]

[Mesh]
  [one]
    type = GeneratedMeshGenerator
    dim = 1
  []
[]

[AuxVariables]
  [d]
  []
[]

[AuxKernels]
  [d]
    type = FunctionAux
    variable = d
    function = 't'
  []
[]

[Materials]
  [degradation]
    type = PowerDegradationFunction
    property_name = g
    phase_field = d
    parameter_names = 'p eta '
    parameter_values = '2 1e-6'
  []
[]

[Postprocessors]
  [d]
    type = ElementAverageValue
    variable = d
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [g]
    type = ADElementAverageMaterialProperty
    mat_prop = g
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [dg_dd]
    type = ADElementAverageMaterialProperty
    mat_prop = dg/dd
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  dt = 0.1
  end_time = 1
[]

[Outputs]
  exodus = true
[]
