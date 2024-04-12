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
  [props]
    type = ADGenericConstantMaterial
    prop_names = 'Gc psic xi c0 l'
    prop_values = '1 1 2 3.14159 1'
  []
  [degradation]
    type = RationalDegradationFunction
    property_name = g
    phase_field = d
    parameter_names = 'p a2 a3 eta '
    parameter_values = '2 -0.5 0 1e-6'
    material_property_names = 'Gc psic xi c0 l '
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
