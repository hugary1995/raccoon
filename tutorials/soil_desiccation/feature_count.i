[Problem]
  solve = false
[]

[UserObjects]
  [solution]
    type = SolutionUserObject
    mesh = 'elasticity_out.e'
  []
[]

[Mesh]
  [fmg]
    type = FileMeshGenerator
    mesh = 'elasticity_out.e'
  []
[]

[AuxVariables]
  [d]
  []
  [unique_grains]
    order = CONSTANT
    family = MONOMIAL
  []
  [halos]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [read_d]
    type = SolutionAux
    variable = 'd'
    from_variable = 'd'
    solution = 'solution'
    scale_factor = -1
    add_factor = 1
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
  [unique_grains]
    type = FeatureFloodCountAux
    variable = 'unique_grains'
    flood_counter = 'feature_counter'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
    field_display = UNIQUE_REGION
  []
  [halos]
    type = FeatureFloodCountAux
    variable = 'halos'
    flood_counter = 'feature_counter'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
    field_display = HALOS
  []
[]

[VectorPostprocessors]
  [feature_volumes]
    type = FeatureVolumeVectorPostprocessor
    flood_counter = feature_counter
    execute_on = 'initial timestep_end'
  []
[]

[Postprocessors]
  [feature_counter]
    type = FeatureFloodCount
    variable = 'd'
    compute_var_to_feature_map = true
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [volume]
    type = VolumePostprocessor
    execute_on = 'INITIAL'
  []
  [volume_fraction]
    type = FeatureVolumeFraction
    mesh_volume = volume
    feature_volumes = feature_volumes
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  dt = 1e-3
  end_time = 0.22
[]

[Outputs]
  exodus = true
  [csv]
    type = CSV
    file_base = 'count/statistics'
  []
[]
