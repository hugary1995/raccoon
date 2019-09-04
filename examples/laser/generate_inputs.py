#!/usr/bin/env python
#* This file is part of the RACCOON application
#* being developed at Dolbow lab at Duke University
#* http://dolbow.pratt.duke.edu

import os
import sys
import shutil
import math
import csv

if len(sys.argv) != 1:
    print("usage: ./analytical.py")
    exit()

mechanical_content = """\
[Mesh]
  type = FileMesh
  file = ../gold/geo2.msh
[]

[Problem]
  coord_type = RZ
[]

[Variables]
  [./disp_r]
  [../]
  [./disp_z]
  [../]
  [./T]
    initial_condition = 300
  [../]
[]

[AuxVariables]
  [./vel_r]
  [../]
  [./vel_z]
  [../]
  [./accel_r]
  [../]
  [./accel_z]
  [../]
  [./stress]
    family = MONOMIAL
  [../]
  [./d]
  [../]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./bounds_dummy]
  [../]
  [./Gc]
  [../]
[]

[Postprocessors]
  [./thermal_ablation_volume]
    type = ThermalAblationVolume
    d = d
    temperature = T
    ablation_threshold = 1612
  [../]

  [./d_integral]
    type = ElementIntegralVariablePostprocessor
    variable = d
  [../]
[]

[AuxKernels]
  [./Gc]
    type = FunctionAux
    variable = Gc
    function = Gc
    execute_on = 'INITIAL'
  [../]
[]

[Bounds]
  [./T]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = T
    lower = 300
    execute_on = 'LINEAR TIMESTEP_BEGIN'
  [../]
[]

[AuxKernels]
  [./E_el]
    type = MaterialRealAux
    property = 'E_el'
    variable = 'E_el'
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = ../gold/Gc2.txt
  [../]
[]

[Materials]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '2.0e-9'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.74e4
    poissons_ratio = 0.25
  [../]
  [./strain]
    type = ADComputeAxisymmetricRZSmallStrain
    displacements = 'disp_r disp_z'
    eigenstrain_names = 'thermal_eigenstrain'
  [../]
  [./stress]
    type = SmallStrainDegradedPK2Stress_StrainSpectral
    d = d
    history = false
  [../]

  [./thermal]
    type = HeatConductionMaterial
    temp = 'T'
    thermal_conductivity = 2.259
    specific_heat = 8.343e8
  [../]
  [./thermal_expansion]
    type = ADComputeThermalExpansionEigenstrain
    stress_free_temperature = '300'
    temperature = 'T'
    thermal_expansion_coeff = 8e-6
    eigenstrain_name = thermal_eigenstrain
  [../]

  [./fracture_energy_barrier]
    type = GenericFunctionMaterial
    prop_names = 'b'
    prop_values = '0.0009'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.18
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[AuxKernels]
  [./accel_r]
    type = NewmarkAccelAux
    variable = accel_r
    displacement = disp_r
    velocity = vel_r
    beta = 0.25
    execute_on = timestep_end
  [../]
  [./vel_r]
    type = NewmarkVelAux
    variable = vel_r
    acceleration = accel_r
    gamma = 0.5
    execute_on = timestep_end
  [../]
  [./accel_z]
    type = NewmarkAccelAux
    variable = accel_z
    displacement = disp_z
    velocity = vel_z
    beta = 0.25
    execute_on = timestep_end
  [../]
  [./vel_z]
    type = NewmarkVelAux
    variable = vel_z
    acceleration = accel_z
    gamma = 0.5
    execute_on = timestep_end
  [../]
  [./stress]
    type = RankTwoScalarAux
    variable = stress
    rank_two_tensor = stress
    scalar_type = MaxPrincipal
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Kernels]
  [./inertia_r]
    type = InertialForce
    variable = disp_r
    velocity = vel_r
    acceleration = accel_r
    beta = 0.25
    gamma = 0.5
  [../]
  [./inertia_z]
    type = InertialForce
    variable = disp_z
    velocity = vel_z
    acceleration = accel_z
    beta = 0.25
    gamma = 0.5
  [../]
  [./solid_r]
    type = ADStressDivergenceRZTensors
    variable = disp_r
    component = 0
    displacements = 'disp_r disp_z'
  [../]
  [./solid_z]
    type = ADStressDivergenceRZTensors
    variable = disp_z
    component = 1
    displacements = 'disp_r disp_z'
  [../]

  [./Hcond_time]
    type = ADHeatConductionTimeDerivative
    variable = T
  [../]
  [./Hcond]
    type = ADHeatConduction
    variable = T
  [../]
  [./Hsource]
    type = HeatSource
    variable = T
    function = 'if (t<%E, %E*t, if (t<%E, %E, if (t<%E, %E-%E*t, 0)))'
    block = source
  []
[]

[BCs]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -sub_pc_type -ksp_max_it -ksp_gmres_restart -sub_pc_factor_levels -snes_type'
  petsc_options_value = 'asm      ilu          200         200                0                     vinewtonrsls'

  dt = 5e-9
  end_time = %E

  automatic_scaling = false
[]

[Outputs]
  file_base = run%d
  print_linear_residuals = false
  exodus = true
  csv = true
[]
"""
fracture_content = """\
[MultiApps]
  [./thermomechanical]
    type = TransientMultiApp
    input_files = 'thermal_mechanical_dynamic.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./get_E_el]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'E_el'
    variable = 'E_el'
  [../]
  [./send_d]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = to_multiapp
    source_variable = 'd'
    variable = 'd'
  [../]
  [./get_d_ref]
    type = MultiAppMeshFunctionTransfer
    multi_app = thermomechanical
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd_ref'
  [../]
[]

[Mesh]
  type = FileMesh
  file = '../gold/geo2.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./E_el]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./Gc]
  [../]
[]

[AuxKernels]
 [./Gc]
   type = FunctionAux
   variable = Gc
   function = Gc
   execute_on = 'INITIAL'
  [../]
[]


[Bounds]
  [./irreversibility]
    type = Irreversibility
    variable = bounds_dummy
    bounded_variable = d
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

[Functions]
  [./Gc]
    type = PiecewiseMultilinear
    data_file = ../gold/Gc2.txt
  [../]
[]

[Materials]
  [./fracture_energy_barrier]
    type = StationaryGenericFunctionMaterial
    prop_names = 'b'
    prop_values = '0.0009'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties]
    type = FractureMaterial
    Gc = Gc
    L = 0.18
    local_dissipation_norm = 8/3
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
    residual_degradation = 1e-06
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'

  dt = 5e-9
  end_time = %E

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  print_linear_residuals = false
[]
"""

###################
###   prepare   ###
###################

duration_in_micro_second = [71.5,101,117.5,135,157.5,179.5,209.5,220,247,260,279,287]
one_percent_intensity = [13356053.464282,18910055.8949735,24381901.8560723,28295046.5984049,30316121.3554338,31920595.7447186,34187034.2253997,39066456.3830249,38662260.0281848,45911433.7834694,51341818.424525,58229135.5302539]
num_runs = len(duration_in_micro_second)

for run in range(num_runs):

    run_dir = "run"+str(run)

    print("")
    print("Remove existing run folder: "+run_dir)
    try:
        shutil.rmtree(run_dir)
    except OSError as e:
        print ("     %s - %s." % (e.filename, e.strerror))

    print("Create new run folder: "+run_dir)

    # Create target Directory if don't exist
    if not os.path.exists(run_dir):
        os.mkdir(run_dir)
    else:
        print("     Directory "+run_dir+" already exists")

    os.chdir(run_dir)

    ##################
    ### mechanical ###
    ##################

    print("Writing mechanical input file...")
    f = open("thermal_mechanical_dynamic.i", "w")

    # convert duration to second
    duration = duration_in_micro_second[run]*1e-6

    # target intensity
    intensity = one_percent_intensity[run]

    # time intervals
    t1 = 1e-6
    t2 = duration-t1
    slope = intensity/t1
    b = slope*duration

    f.write(mechanical_content % (t1,slope,t2,intensity,duration,b,slope,duration,run))
    f.close()

    ##################
    ###  fracture  ###
    ##################

    print("Writing fracture input file...")
    f = open("fracture.i", "w")
    f.write(fracture_content % (duration))
    f.close()

    os.chdir("..")
