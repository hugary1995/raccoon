# Wave propogation in 1D using Newmark time integration
#
# The test is for an 1D bar element of length 4m  fixed on one end
# with a sinusoidal pulse dirichlet boundary condition applied to the other end.
# beta and gamma are Newmark time integration parameters
# The equation of motion in terms of matrices is:
#
# M*accel +  K*disp = 0
#
# Here M is the mass matrix, K is the stiffness matrix
#
# This equation is equivalent to:
#
# density*accel + Div Stress= 0
#
# The first term on the left is evaluated using the Inertial force kernel
# The last term on the left is evaluated using StressDivergenceTensors
#
# The displacement at the second, third and fourth node at t = 0.1 are
# -8.021501116638234119e-02, 2.073994362053969628e-02 and  -5.045094181261772920e-03, respectively

#[Mesh]
#  type = GeneratedMesh
#  dim = 2
#  nx = 1
#  ny = 4
#  nz = 1
#  xmin = 0.0
#  xmax = 0.1
#  ymin = 0.0
#  ymax = 4.0
#  zmin = 0.0
#  zmax = 0.1
#[]

[Mesh]
  file = /home/yl306/ASFracture/mesh/innermesh2.msh
[]


[Variables]
  [./d]
  [../]
  [./b]
  [../]
[]

[AuxVariables]
  [./disp_x_in]
  [../]
  [./disp_y_in]
  [../]
[]

[Kernels]
# phase filed equation
   [./pfbulk]
      type = CohesivePFFracBulkRate
      variable = d
      ifOld = true
      l = 0.004
      p = 3
      beta = b
      visco = 1.e-6
      gc_prop_var = 'gc_prop'
      G0_var = 'G0_pos'
      dG0_dstrain_var = 'dG0_pos_dstrain'
      Emod = 'E'
      sigmac = 'sc'
      disp_x = disp_x_in
      disp_y = disp_y_in
   [../]
   [./dcdt]
      type = TimeDerivativeExp
      variable = d
#      lumping = true
   [../]
   [./pfintvar]
       type = Reaction
       variable = b
   [../]
   [./pfintcoupled]
       type = PFFracCoupledInterfaceExp
       variable = b
       c = d
    [../]
[]


[Materials]
  [./Elasticity_tensor]
    type = ComputeElasticityTensor
    fill_method = symmetric_isotropic
    C_ijkl = '0.01305 0.01073'
  [../]

  [./strain]
    type = ComputeSmallStrain
    displacements = 'disp_x_in disp_y_in'
  [../]

  [./elastic]
       type = CohesiveLinearIsoElasticPFDamage
       c = d
       historyEng = true
       kdamage = 0.0
       store_stress_old = true
       gc_prop_var = 'gc_prop'
       Emod = 'E'
       sigmac = 'sc'
       l = 0.004
       p = 3
  [../]

  [./pfbulkmat]
       type = PFFracBulkRateMaterial
       gc = 5.226e-10
  [../]

  [./constant]
      type = GenericConstantMaterial
      prop_names = 'E sc'
      prop_values = '0.02735 1e-5'
  [../]

[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    full = true
  [../]
[]


[Executioner]
  type = Transient
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly       lu           1'

#  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
#  petsc_options_value = '        lu       superlu_dist'

  start_time = 0
  end_time = 3.0
  l_max_its = 50
  nl_max_its = 20

  l_tol = 1e-9
  nl_rel_tol = 1e-12
  dt = 1e-3
[]

[Outputs]
  csv = true
  [./console]
        type = Console
        max_rows = 10
  [../]
  [./exodus]
        type = Exodus
        interval = 10
        file_base = DamageOut
  [../]
[]
