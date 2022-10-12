//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

// libMesh include files.
#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/exodusII_io_helper.h"

#include <boost/math/distributions.hpp>
#include <ctime>
#include <random>

// Bring in everything from the libMesh namespace
using namespace libMesh;

std::vector<Real> sample_gaussian(const std::vector<Real> & eigvals,
                                  const std::vector<std::map<dof_id_type, Real>> & eigvecs,
                                  std::default_random_engine & generator);

void compute_correlated_Gamma_fields(const std::vector<Real> & Xi_1,
                                     const std::vector<Real> & Xi_2,
                                     std::vector<Real> & P_1,
                                     Real mean_1,
                                     Real CV_1,
                                     std::vector<Real> & P_2,
                                     Real mean_2,
                                     Real CV_2,
                                     Real rho);

int
main(int argc, char ** argv)
{
  // Check for proper usage.
  if (argc != 1)
    libmesh_error_msg("\nUsage: " << argv[0]);

  // Initialize libMesh and the dependent libraries.
  LibMeshInit init(argc, argv);

  if (init.comm().size() > 1)
    libmesh_error_msg("Parallel sampling is not supported, use 1 processor only.");

  // Read the mesh
  Mesh mesh(init.comm());
  mesh.read("basis.e");

  // Read the eigenpairs
  std::vector<Real> eigvals;
  std::vector<std::map<dof_id_type, Real>> eigvecs;

  ExodusII_IO basis(mesh);
  basis.read("basis.e");
  ExodusII_IO_Helper & basis_helper = basis.get_exio_helper();
  for (int i = 1; i < basis.get_num_time_steps(); i++)
  {
    // read eigenvalue
    std::vector<Real> eigval;
    basis.read_global_variable({"d"}, i, eigval);
    eigvals.push_back(eigval[0]);

    // read eigenvector
    basis_helper.read_nodal_var_values("v", i);
    eigvecs.push_back(basis_helper.nodal_var_values);
  }

  // sample Gaussian fields
  std::default_random_engine generator;
  generator.seed(std::time(NULL));
  std::vector<Real> Xi_1 = sample_gaussian(eigvals, eigvecs, generator);
  std::vector<Real> Xi_2 = sample_gaussian(eigvals, eigvecs, generator);

  // transform to marginal Gamma fields
  std::vector<Real> Gc;
  std::vector<Real> psic;
  compute_correlated_Gamma_fields(Xi_1, Xi_2, Gc, 8e-4, 0.03, psic, 3e-5, 0.03, 0);

  // write random field
  ExodusII_IO fields(mesh);
  fields.write("fields.e");
  ExodusII_IO_Helper & fields_helper = fields.get_exio_helper();
  fields_helper.initialize_nodal_variables({"Gc", "psic"});
  fields_helper.write_nodal_values(1, Gc, 1);
  fields_helper.write_nodal_values(2, psic, 1);

  return EXIT_SUCCESS;
}

std::vector<Real>
sample_gaussian(const std::vector<Real> & eigvals,
                const std::vector<std::map<dof_id_type, Real>> & eigvecs,
                std::default_random_engine & generator)
{
  unsigned int ndof = eigvecs[0].size();
  std::vector<Real> Xi(ndof);
  std::normal_distribution<Real> distribution(0.0, 1.0);

  for (unsigned int i = 0; i < eigvals.size(); i++)
  {
    Real eta = distribution(generator);
    for (unsigned int j = 0; j < ndof; j++)
      Xi[j] += std::sqrt(eigvals[i]) * eta * eigvecs[i].at(j);
  }

  return Xi;
}

void
compute_correlated_Gamma_fields(const std::vector<Real> & Xi_1,
                                const std::vector<Real> & Xi_2,
                                std::vector<Real> & P_1,
                                Real mean_1,
                                Real CV_1,
                                std::vector<Real> & P_2,
                                Real mean_2,
                                Real CV_2,
                                Real rho)
{
  unsigned int ndof = Xi_1.size();

  // Normal distribution
  auto normal = boost::math::normal_distribution<Real>(0, 1);

  // Gamma distribution for the first field
  Real std_1 = CV_1 * mean_1;
  Real var_1 = std_1 * std_1;
  Real theta_1 = var_1 / mean_1;
  Real k_1 = mean_1 / theta_1;

  // Gamma distribution for the second field
  Real std_2 = CV_2 * mean_2;
  Real var_2 = std_2 * std_2;
  Real theta_2 = var_2 / mean_2;
  Real k_2 = mean_2 / theta_2;

  // Transform into the first field
  P_1.resize(ndof);
  for (unsigned int i = 0; i < ndof; i++)
    P_1[i] = theta_1 * boost::math::gamma_p_inv<Real, Real>(k_1, boost::math::cdf(normal, Xi_1[i]));

  // Transform into the first field
  P_2.resize(ndof);
  for (unsigned int i = 0; i < ndof; i++)
    P_2[i] = theta_2 *
             boost::math::gamma_p_inv<Real, Real>(
                 k_2, boost::math::cdf(normal, rho * Xi_1[i] + std::sqrt(1 - rho * rho) * Xi_2[i]));
}
