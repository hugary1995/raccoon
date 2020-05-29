//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

// libMesh include files.
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/mesh_generation.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/eigen_system.h"
#include "libmesh/equation_systems.h"
#include "libmesh/slepc_eigen_solver.h"
#include "libmesh/fe.h"
#include "libmesh/quadrature_gauss.h"
#include "libmesh/dense_matrix.h"
#include "libmesh/sparse_matrix.h"
#include "libmesh/numeric_vector.h"
#include "libmesh/dof_map.h"
#include "libmesh/enum_eigen_solver_type.h"
#include "libmesh/petsc_matrix.h"
#include "slepcsys.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include "augment_sparsity_to_dense.h"
#include "covariance_functions.h"

// Bring in everything from the libMesh namespace
using namespace libMesh;

// Function prototype.  This is the function that will assemble
// the eigen system.
void assembly(EquationSystems & es);

int
main(int argc, char ** argv)
{
  // Check for proper usage.
  if (argc != 1)
    libmesh_error_msg("\nUsage: " << argv[0]);

  // Initialize libMesh and the dependent libraries.
  LibMeshInit init(argc, argv);

  // Create a mesh, with dimension to be overridden later, on the
  // default MPI communicator.
  // Use the internal mesh generator to create a uniform
  // 2D grid on a square.
  Mesh mesh(init.comm());
  MeshTools::Generation::build_square(mesh, 50, 50, 0, 100, 0, 100, QUAD4);
  mesh.print_info();

  // Create an equation systems object.
  EquationSystems equation_systems(mesh);

  // Create a EigenSystem named "Eigensystem" and (for convenience)
  // use a reference to the system we create.
  EigenSystem & eigen_system = equation_systems.add_system<EigenSystem>("Eigensystem");
  eigen_system.assemble_before_solve = false;

  // Declare the system variables.
  // Adds the variable "v" to "Eigensystem".
  // By default this is FIRST, LAGRANGE
  eigen_system.add_variable("v");

  // Augment the sparsity to dense since we are assemblying an integral equation.
  // In general you don't need to do this if your equation doesn't contain nonlocal contribution.
  std::shared_ptr<AugmentSparsityToDense> augment_sparsity(new AugmentSparsityToDense(mesh));
  eigen_system.get_dof_map().add_coupling_functor(augment_sparsity);

  // Set necessary parameters used in EigenSystem::solve(),
  // i.e. the number of requested eigenpairs nev and the number
  // of basis vectors ncv used in the solution algorithm. Note that
  // ncv >= nev must hold and ncv >= 2*nev is recommended.
  // Here we guess 1/10 eigenvalues will give us a good spectrum.
  equation_systems.parameters.set<unsigned int>("eigenpairs") = mesh.n_nodes();
  equation_systems.parameters.set<unsigned int>("basis vectors") = mesh.n_nodes();

  // You may optionally change the default eigensolver used by SLEPc.
  // The Krylov-Schur method is mathematically equivalent to implicitly
  // restarted Arnoldi, the method of Arpack, so there is currently no
  // point in using SLEPc with Arpack.
  // ARNOLDI     = default in SLEPc 2.3.1 and earlier
  // KRYLOVSCHUR default in SLEPc 2.3.2 and later
  // eigen_system.eigen_solver->set_eigensolver_type(KRYLOVSCHUR);

  // Set the solver tolerance and the maximum number of iterations.
  equation_systems.parameters.set<Real>("linear solver tolerance") = pow(TOLERANCE, 5. / 3.);
  equation_systems.parameters.set<unsigned int>("linear solver maximum iterations") = 1000;

  // Set the type of the problem, here we deal with
  // a generalized Hermitian problem.
  eigen_system.set_eigenproblem_type(GHEP);

  // Initialize the data structures for the equation system.
  libMesh::out << "Initializing equation system...";
  equation_systems.init();
  libMesh::out << "complete" << std::endl;
  equation_systems.print_info();

  // Assemly
  assembly(equation_systems);

  // Solve the system "Eigensystem".
  // We request eigenpairs with largest real eigenvalues.
  // As a result, eigenvalues will be sorted with descending real eigenvalues.
  libMesh::out << "Solving the generalized eigen system...";
  PetscOptionsSetValue(PETSC_NULL, "-eps_largest_real", PETSC_NULL);
  eigen_system.solve();
  libMesh::out << "complete\n" << std::endl;

  // Get the number of converged eigen pairs.
  unsigned int nconv = eigen_system.get_n_converged();

  ExodusII_IO exo(mesh);
  Real eigval_prev = std::numeric_limits<Real>::max();
  Real eigval_sum = 0;
  unsigned int eigval_num;
  Real TOL = 1e-6;
  for (eigval_num = 0; eigval_num < nconv; eigval_num++)
  {
    std::pair<Real, Real> eigval = eigen_system.get_eigenpair(eigval_num);
    exo.write_timestep("basis.e", equation_systems, eigval_num + 1, (Real)eigval_num);
    exo.write_global_data({eigval.first}, {"d"});
    if (eigval.first < eigval_sum * TOL && std::abs(eigval.first - eigval_prev) > 1e-12)
    {
      libMesh::out << "converged with " << eigval_num << " eigenvalues." << std::endl;
      break;
    }
    eigval_sum += eigval.first;
    eigval_prev = eigval.first;
  }
  libMesh::out << "Wrote " << eigval_num << " eigenvalues and eigenvectors." << std::endl;

  return EXIT_SUCCESS;
}

void
assembly(EquationSystems & es)
{
  libMesh::out << "Assemblying the eigen system" << std::endl;

  // Get a constant reference to the mesh object and the mesh dimension
  const MeshBase & mesh = es.get_mesh();
  const unsigned int dim = mesh.mesh_dimension();

  // Get a reference to our system.
  // Get a constant reference to the Finite Element type
  // for the first (and only) variable in the system.
  EigenSystem & eigen_system = es.get_system<EigenSystem>("Eigensystem");
  FEType fe_type = eigen_system.get_dof_map().variable_type(0);

  // A reference to the two system matrices
  SparseMatrix<Number> & matrix_A = *eigen_system.matrix_A;
  SparseMatrix<Number> & matrix_B = *eigen_system.matrix_B;

  // Build a Finite Element object of the specified type.  Since the
  // FEBase::build() member dynamically creates memory we will
  // store the object as a std::unique_ptr<FEBase>.  This can be thought
  // of as a pointer that will clean up after itself.
  std::unique_ptr<FEBase> fe(FEBase::build(dim, fe_type));
  std::unique_ptr<FEBase> fe_remote(FEBase::build(dim, fe_type));

  // A  Gauss quadrature rule for numerical integration.
  // Use the default quadrature order.
  QGauss qrule(dim, fe_type.default_quadrature_order());

  // Tell the finite element object to use our quadrature rule.
  fe->attach_quadrature_rule(&qrule);
  fe_remote->attach_quadrature_rule(&qrule);

  // The element Jacobian * quadrature weight at each integration point.
  const std::vector<Real> & JxW = fe->get_JxW();
  const std::vector<Real> & JxW_remote = fe_remote->get_JxW();

  // The element shape functions evaluated at the quadrature points.
  const std::vector<std::vector<Real>> & phi = fe->get_phi();
  const std::vector<std::vector<Real>> & phi_remote = fe_remote->get_phi();

  // The element shape function gradients evaluated at the quadrature
  // points.
  const std::vector<std::vector<RealGradient>> & dphi = fe->get_dphi();

  // A reference to the DofMap object for this system.  The DofMap
  // object handles the index translation from node and element numbers
  // to degree of freedom numbers.
  const DofMap & dof_map = eigen_system.get_dof_map();

  // The element mass and stiffness matrices.
  DenseMatrix<Number> Me;
  DenseMatrix<Number> Ke;

  // This vector will hold the degree of freedom indices for
  // the element.  These define where in the global system
  // the element degrees of freedom get mapped.
  std::vector<dof_id_type> dof_indices;
  std::vector<dof_id_type> dof_indices_remote;

  // Get quadrature point in the mapped space
  const std::vector<Point> & q_points = fe->get_xyz();
  const std::vector<Point> & q_points_remote = fe_remote->get_xyz();

  // Before we do the element loop, construct a covariance kernel
  PSE covariance_x(5, 100);
  PSE covariance_y(5, 100);

  // Now we will loop over all the elements in the mesh that
  // live on the local processor. We will compute the element
  // matrix and right-hand-side contribution.  In case users
  // later modify this program to include refinement, we will
  // be safe and will only consider the active elements;
  // hence we use a variant of the active_elem_iterator.
  unsigned int total = mesh.n_active_local_elem();
  unsigned int count = 0;
  Real progress_prev = 0;
  for (const auto & elem : mesh.active_local_element_ptr_range())
  {
    // Get the degree of freedom indices for the
    // current element.  These define where in the global
    // matrix and right-hand-side this element will
    // contribute to.
    dof_map.dof_indices(elem, dof_indices);

    // Compute the element-specific data for the current
    // element.  This involves computing the location of the
    // quadrature points (q_point) and the shape functions
    // (phi, dphi) for the current element.
    fe->reinit(elem);

    // Zero the element matrices before
    // summing them.  We use the resize member here because
    // the number of degrees of freedom might have changed from
    // the last element.  Note that this will be the case if the
    // element type is different (i.e. the last element was a
    // triangle, now we are on a quadrilateral).
    const unsigned int n_dofs = cast_int<unsigned int>(dof_indices.size());
    Me.resize(n_dofs, n_dofs);

    // Now loop over the quadrature points.  This handles
    // the numeric integration.
    //
    // We will build the element matrix.  This involves
    // a double loop to integrate the test functions (i) against
    // the trial functions (j).
    for (unsigned int qp = 0; qp < qrule.n_points(); qp++)
      for (unsigned int i = 0; i < n_dofs; i++)
        for (unsigned int j = 0; j < n_dofs; j++)
          Me(i, j) += JxW[qp] * phi[i][qp] * phi[j][qp];

    matrix_B.add_matrix(Me, dof_indices);

    for (unsigned int qp = 0; qp < qrule.n_points(); qp++)
      for (unsigned int i = 0; i < n_dofs; i++)
        for (const auto & elem_remote : mesh.active_element_ptr_range())
        {
          dof_map.dof_indices(elem_remote, dof_indices_remote);
          fe_remote->reinit(elem_remote);
          const unsigned int n_dofs_remote = cast_int<unsigned int>(dof_indices_remote.size());
          Ke.resize(n_dofs, n_dofs_remote);
          for (unsigned int qp_remote = 0; qp_remote < qrule.n_points(); qp_remote++)
          {
            Point lag = q_points[qp] - q_points_remote[qp_remote];
            Real R = covariance_x.covariance(std::abs(lag(0))) *
                     covariance_y.covariance(std::abs(lag(1)));
            for (unsigned int j = 0; j < n_dofs_remote; j++)
              Ke(i, j) +=
                  R * phi[i][qp] * JxW[qp] * phi_remote[j][qp_remote] * JxW_remote[qp_remote];
          }
          matrix_A.add_matrix(Ke, dof_indices, dof_indices_remote);
        }

    count++;
    Real progress = (Real)count / total;
    if (progress >= progress_prev + 0.1)
    {
      libMesh::out << "Assemblying the eigen system progress " << (progress_prev + 0.1) * 100 << "%"
                   << std::endl;
      progress_prev += 0.1;
    }
  }

  libMesh::out << "Assemblying the eigen system complete.\n" << std::endl;
}
