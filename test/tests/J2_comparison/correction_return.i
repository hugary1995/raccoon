#

E = 201.8e3
nu = 0.3
K = '${fparse E/3/(1-2*nu)}'
G = '${fparse E/2/(1+nu)}'
rho = 7900

end_time = 1.2
dt = 0.1

[GlobalParams]
    displacements = 'disp_x disp_y disp_z'
    volumetric_locking_correction = true
[]

[Mesh]
    [gmg]
        type = GeneratedMeshGenerator
        dim = 3
        nx = 1
        ny = 1
        nz = 1
        xmin = 0
        xmax = 1
        ymin = 0
        ymax = 1
        zmin = 0
        zmax = 1
        elem_type = HEX8
    []
[]

[Variables]
    [disp_x]
    []
    [disp_y]
    []
    [disp_z]
    []
[]

[AuxVariables]
    [d]
    []
[]

[Kernels]
    [x]
        type = ADStressDivergenceTensors
        variable = disp_x
        component = 0
        use_displaced_mesh = true
    []
    [y]
        type = ADStressDivergenceTensors
        variable = disp_y
        component = 1
        use_displaced_mesh = true
    []
    [z]
        type = ADStressDivergenceTensors
        variable = disp_z
        component = 2
        use_displaced_mesh = true
    []
[]

[Materials]
    [defgrad]
        type = ComputeDeformationGradient
    []
    [bulk_properties]
        type = ADGenericConstantMaterial
        prop_names = 'K G density'
        prop_values = '${K} ${G} ${rho}'
    []
    [reg_density]
        type = MaterialADConverter
        ad_props_in = 'density'
        reg_props_out = 'reg_density'
    []
    [nodeg]
        type = NoDegradation
        phase_field = d
        property_name = nodeg
    []
    [hencky]
        type = CNHIsotropicElasticity
        bulk_modulus = K
        shear_modulus = G
        phase_field = d
        degradation_function = nodeg
        decomposition = NONE
    []
    [J2]
        type = LargeDeformationJ2PlasticityBeBar
        phase_field = d
        hardening_model = hardening
        relative_tolerance = 1e-08
        output_properties = 'psie_active effective_plastic_strain'
        outputs = exodus
        apply_strain_energy_split = false
    []
    [hardening]
        type = PowerLawHardening
        exponent = 2
        phase_field = d
        reference_plastic_strain = 1
        yield_stress = 300
        degradation_function = nodeg
    []
    [stress]
        type = ComputeLargeDeformationStress
        elasticity_model = hencky
        plasticity_model = J2
    []
[]

[BCs]
    [xfix]
        type = DirichletBC
        variable = disp_x
        boundary = 'left'
        value = 0
        preset = false
    []
    [yfix]
        type = DirichletBC
        variable = disp_y
        boundary = bottom
        value = 0
        preset = false
    []
    [zfix]
        type = DirichletBC
        variable = disp_z
        boundary = 'front'
        value = 0
        preset = false
    []
    [ypull]
        type = FunctionDirichletBC
        variable = disp_y
        boundary = top
        function = '0.05*t'
    []
[]

[Postprocessors]
    [psie_active_int]
        type = ADElementIntegralMaterialProperty
        mat_prop = psie_active
        use_displaced_mesh = false
    []
    [psip_active_int]
        type = ADElementIntegralMaterialProperty
        mat_prop = psip_active
        use_displaced_mesh = false
    []
    [ep_int]
        type = ADElementIntegralMaterialProperty
        mat_prop = effective_plastic_strain
        use_displaced_mesh = false
    []
[]

[Executioner]
    type = Transient
    solve_type = NEWTON
    line_search = none
    petsc_options_iname = '-pc_type'
    petsc_options_value = 'lu'

    [TimeIntegrator]
        type = ImplicitEuler
    []

    nl_rel_tol = 1e-8
    nl_abs_tol = 1e-10
    nl_max_its = 50

    [TimeStepper]
        type = ConstantDT
        dt = ${dt}
    []

    end_time = ${end_time}

    automatic_scaling = true
[]

[Outputs]
    print_linear_residuals = false
    [exodus]
        type = Exodus
        file_base = correction_return
        use_displaced = false
    []
    [csv]
        type = CSV
        file_base = correction_return
    []
[]
