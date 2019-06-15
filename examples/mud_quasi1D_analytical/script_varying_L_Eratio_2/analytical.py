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

mesh_content = """\
h = %.3f;
L = %E;
H = h*5;
e = L/200;
E = e*10;
r = e;

Point(0) = {L/2,h,0,e};
Point(1) = {0,0,0,E};
Point(2) = {0,h,0,E};
Point(3) = {L/2-r,h,0,e};
Point(4) = {L/2,h-r,0,e};
Point(5) = {L/2+r,h,0,e};
Point(6) = {L,h,0,E};
Point(7) = {L,0,0,E};
Point(8) = {L/2,0,0,e};
Point(9) = {0,-H,0,E};
Point(10) = {L,-H,0,E};

Line(1) = {1,2};
Line(2) = {2,3};
Circle(3) = {3,0,4};
Circle(4) = {4,0,5};
Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,1};
Line(9) = {1,9};
Line(10) = {9,10};
Line(11) = {10,7};
Line(12) = {4,8};

Line Loop(1) = {1,2,3,4,5,6,7,8};
Line Loop(2) = {9,10,11,7,8};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Line{12} In Surface{1};

Physical Surface("film") = {1};
Physical Surface("substrate") = {2};
Physical Line("bottom") = {10};
Physical Line("left") = {1,9};
Physical Line("right") = {6,11};
"""
master_content = """\
[Problem]
  solve = false
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
[]

[MultiApps]
  [./mechanical]
    type = TransientMultiApp
    input_files = 'mechanical.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
    sub_cycling = true
    detect_steady_state = true
    steady_state_tol = 1e-06
  [../]
[]

[Transfers]
  [./load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./get_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_END'
  [../]
  [./send_d_ref]
    type = MultiAppCopyTransfer
    multi_app = mechanical
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
    execute_on = 'TIMESTEP_BEGIN'
  [../]
  [./fracture_energy]
    type = MultiAppPostprocessorTransfer
    multi_app = mechanical
    direction = from_multiapp
    from_postprocessor = 'fracture_energy'
    to_postprocessor = 'fracture_energy'
    execute_on = SAME_AS_MULTIAPP
    reduction_type = average
  [../]
[]

[AuxVariables]
  [./load]
    family = SCALAR
  [../]
  [./d_ref]
  [../]
[]

[AuxScalarKernels]
  [./load]
    type = FunctionScalarAux
    variable = load
    function = t
    execute_on = 'TIMESTEP_BEGIN'
  [../]
[]

[Postprocessors]
  [./fracture_energy]
    type = Receiver
  [../]
[]

[UserObjects]
  [./termination]
    type = Terminator
    expression = 'fracture_energy > %E'
  [../]
[]

[Executioner]
  type = Transient
  dt = %E
[]

[Outputs]
  csv = true
[]
"""
mechanical_content = """\
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
[]

[MultiApps]
  [./fracture]
    type = TransientMultiApp
    input_files = 'fracture.i'
    app_type = raccoonApp
    execute_on = 'TIMESTEP_END'
  [../]
[]

[Transfers]
  [./disp_x]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_x'
    variable = 'disp_x'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./disp_y]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'disp_y'
    variable = 'disp_y'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./d_ref]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'd_ref'
    variable = 'd_ref'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./d]
    type = MultiAppCopyTransfer
    multi_app = fracture
    direction = from_multiapp
    source_variable = 'd'
    variable = 'd'
    execute_on = SAME_AS_MULTIAPP
  [../]
  [./load]
    type = MultiAppScalarToAuxScalarTransfer
    multi_app = fracture
    direction = to_multiapp
    source_variable = 'load'
    to_aux_scalar = 'load'
    execute_on = SAME_AS_MULTIAPP
  [../]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./d]
  [../]
  [./d_ref]
  [../]
  [./load]
    family = SCALAR
  [../]
[]

[Kernels]
  [./solid_x]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./solid_y]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
[]

[BCs]
  [./boundary_xdisp]
    type = DirichletBC
    variable = disp_x
    boundary = 'bottom'
    value = 0
  [../]
  [./boundary_ydisp]
    type = DirichletBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./Periodic]
    [./disp_x]
      variable = disp_x
      primary = 'left'
      secondary = 'right'
      translation = '%E 0 0'
    [../]
    [./disp_y]
      variable = disp_y
      primary = 'left'
      secondary = 'right'
      translation = '%E 0 0'
    [../]
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor_film]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = %E
    youngs_modulus = %E
    block = 'film'
  [../]
  [./elasticity_tensor_substrate]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = %E
    youngs_modulus = %E
    block = 'substrate'
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
  [../]
  [./stress_film]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
    block = 'film'
  [../]
  [./stress_substrate]
    type = SmallStrainPK2Stress
    block = 'substrate'
  [../]
  [./fracture_energy_barrier_film]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '%E'
    block = 'film'
  [../]
  [./fracture_energy_barrier_substrate]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '%E'
    block = 'substrate'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = %E
    L = %E
    local_dissipation_norm = 8/3
    block = 'film'
  [../]
  [./fracture_properties_substrate]
    type = FractureMaterial
    Gc = %E
    L = %E
    local_dissipation_norm = 8/3
    block = 'substrate'
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Postprocessors]
  [./fracture_energy]
    type = FractureEnergy
    d = d
  [../]
[]

[Executioner]
  type = TransientSupNorm
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = %E

  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load fracture_energy'
  exodus = true
  print_linear_residuals = false
[]
"""
fracture_content = """\
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = FileMesh
  file = 'geo.msh'
[]

[Variables]
  [./d]
  [../]
[]

[AuxVariables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d_ref]
  [../]
  [./bounds_dummy]
  [../]
  [./load]
    family = SCALAR
  [../]
[]

[Bounds]
  [./nonnegtivity]
    type = BoundsAux
    variable = bounds_dummy
    bounded_variable = d
    upper = 1
    lower = 0
  [../]
[]

[Kernels]
  [./damage]
    type = PhaseFieldFractureEvolution
    variable = d
    driving_energy_name = E_el
  [../]
[]

[Materials]
  [./eigen_strain]
    type = ComputeEigenstrainFromScalarInitialStress
    initial_stress_xx = load
    eigenstrain_name = is
  [../]
  [./elasticity_tensor_film]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = %E
    youngs_modulus = %E
    block = 'film'
  [../]
  [./elasticity_tensor_substrate]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = %E
    youngs_modulus = %E
    block = 'substrate'
  [../]
  [./strain]
    type = ADComputeSmallStrain
    eigenstrain_names = is
  [../]
  [./stress_film]
    type = SmallStrainDegradedPK2Stress_NoSplit
    history = false
    block = 'film'
  [../]
  [./stress_substrate]
    type = SmallStrainPK2Stress
    block = 'substrate'
  [../]
  [./fracture_energy_barrier_film]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '%E'
    block = 'film'
  [../]
  [./fracture_energy_barrier_substrate]
    type = GenericConstantMaterial
    prop_names = 'b'
    prop_values = '%E'
    block = 'substrate'
  [../]
  [./local_dissipation]
    type = LinearLocalDissipation
    d = d
  [../]
  [./fracture_properties_film]
    type = FractureMaterial
    Gc = %E
    L = %E
    local_dissipation_norm = 8/3
    block = 'film'
  [../]
  [./fracture_properties_substrate]
    type = FractureMaterial
    Gc = %E
    L = %E
    local_dissipation_norm = 8/3
    block = 'substrate'
  [../]
  [./degradation]
    type = LorentzDegradation
    d = d
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -snes_type'
  petsc_options_value = 'lu vinewtonrsls'
  dt = %E

  nl_abs_tol = 1e-06
  nl_rel_tol = 1e-06
[]

[Outputs]
  hide = 'load'
  print_linear_residuals = false
[]
"""

###################
###   prepare   ###
###################

run = 0

# material properties
E_film = 4.0
nu_film = 0.2
E_substrate = 8.0
nu_substrate = 0.2
Gc = 8e-4

# geometry
h = 1.0
Ls = [];
for i in range(15):
    Ls.append(pow(10.0,0.3+0.1*i))

# plane strain
E1 = E_film/(1.0-nu_film*nu_film)
E0 = E_substrate/(1.0-nu_substrate*nu_substrate)
mu1 = E_film/2.0/(1-nu_film)
mu0 = E_substrate/2.0/(1-nu_substrate)

# Dunder's parameters
alpha = (E1-E0)/(E1+E0)
beta = (mu1*(1.0-2.0*mu0)-mu0*(1.0-2.0*mu1))/(2.0*mu1*(1.0-mu0)+2.0*mu0*(1.0-mu1))

# Dimensionless constants
g = (1.258-0.4*alpha-0.26*pow(alpha,3.0)-0.3*pow(alpha,4.0))/(1.0-alpha)
c = 2.0/math.pi/g
d = math.sqrt(E1/mu1)*c

# critical stress
critical_stress = math.sqrt(Gc*E1*c/h)
critical_energy = critical_stress*critical_stress/E1

f = open("meta.csv", "w")
f.write("L,sigma\n")
f.close()

for L in Ls:

    ###################
    ### calculation ###
    ###################

    # crack
    sigma = math.sqrt(Gc*E1*c/h/(2.0*math.tanh(c*L/2.0/h)-math.tanh(c*L/h)))
    # critical_energy = sigma*sigma/E1-Gc/2.0

    ##################
    ###  directory ###
    ##################

    run_dir = "run"+str(run)

    print("")
    print("#################################################################################")
    print("Dimensionless fragment size = "+str(L/h))
    print("Suggested critical energy = "+str(critical_energy))
    print("Crack #1 fully develops at stress = "+str(sigma))
    print("#################################################################################")

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
    ###    mesh    ###
    ##################

    print("Writing mesh file...")
    f = open("geo.geo", "w")
    f.write(mesh_content % (h,L))
    f.close()
    os.system("gmsh -2 geo.geo > gmsh_output")

    ##################
    ###   master   ###
    ##################

    dt_master = sigma/100;

    print("Writing master input file...")
    f = open("master.i", "w")
    f.write(master_content % (Gc*h,dt_master))
    f.close()

    ##################
    ### mechanical ###
    ##################

    print("Writing mechanical input file...")
    f = open("mechanical.i", "w")
    f.write(mechanical_content % (L,L,nu_film,E1,nu_substrate,E0,critical_energy,critical_energy*10000,Gc,L/100,Gc,L/100,dt_master/100))
    f.close()

    ##################
    ###  fracture  ###
    ##################

    print("Writing fracture input file...")
    f = open("fracture.i", "w")
    f.write(fracture_content % (nu_film,E1,nu_substrate,E0,critical_energy,critical_energy*10000,Gc,L/100,Gc,L/100,dt_master/100))
    f.close()

    ##################
    ###     run    ###
    ##################

    print("Running simulation...")
    os.system("mpiexec -n 12 raccoon-opt -i master.i > simulation_log")

    ##################
    ### interpolate###
    ##################

    with open("master_out.csv") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        # skip header
        csv_reader.next()
        # read first line of data
        row = csv_reader.next()
        stress = [float(row[0])]
        energy = [float(row[1])]
        for row in csv_reader:
            # previous
            stress_prev = stress[len(stress)-1]
            energy_prev = energy[len(energy)-1]
            # current
            stress_curr = float(row[0])
            energy_curr = float(row[1])
            stress.append(stress_curr)
            energy.append(energy_curr)
            # critical stress?
            if energy_curr > Gc*h:
                # interpolate
                k = (stress_curr-stress_prev)/(energy_curr-energy_prev)
                s = stress_prev+k*(Gc*h-energy_prev)
                break
        print("critical stress = %E" % (s))

    ##################
    ###  collect   ###
    ##################

    os.chdir("..")
    print("Writing meta data...")
    f = open("meta.csv", "a")
    f.write("%E,%E\n" % (L,s))
    f.close()
    run = run+1
