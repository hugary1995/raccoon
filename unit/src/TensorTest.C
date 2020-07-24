//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "libmesh/ignore_warnings.h"
#include "gtest/gtest.h"
#include "libmesh/restore_warnings.h"

#include "RankTwoTensor.h"
#include "RankFourTensor.h"
#include "ADReal.h"

#include "metaphysicl/raw_type.h"

TEST(TensorTest, inverse)
{
  RankFourTensor C;
  C.fillSymmetricIsotropicEandNu(4, 0.2);

  // stress = [ 1 2 3
  //            2 4 5
  //            3 5 6 ]
  // tr(stress) = 11
  // strain = 0.3*stress - 0.05*tr(stress)*I
  //        = [ 0.3 0.6 0.9       [ 0.55    0   0
  //            0.6 1.2 1.5    -       0 0.55   0
  //            0.9 1.5 1.8 ]          0    0 0.55 ]
  //        = [ -0.25 0.60 0.90
  //             0.60 0.65 1.50
  //             0.90 1.50 1.25 ]
  RankTwoTensor stress(1, 4, 6, 5, 3, 2);
  RankTwoTensor strain(-0.25, 0.65, 1.25, 1.5, 0.9, 0.6);

  // expect stress = C : strain
  RankTwoTensor stress_test = C * strain;
  EXPECT_EQ(stress(0, 0), stress_test(0, 0));
  EXPECT_EQ(stress(0, 1), stress_test(0, 1));
  EXPECT_EQ(stress(0, 2), stress_test(0, 2));
  EXPECT_EQ(stress(1, 0), stress_test(1, 0));
  EXPECT_EQ(stress(1, 1), stress_test(1, 1));
  EXPECT_EQ(stress(1, 2), stress_test(1, 2));
  EXPECT_EQ(stress(2, 0), stress_test(2, 0));
  EXPECT_EQ(stress(2, 1), stress_test(2, 1));
  EXPECT_EQ(stress(2, 2), stress_test(2, 2));

  // expect strain = C.inv() : stress
  RankTwoTensor strain_test = C.invSymm() * stress;
  EXPECT_NEAR(strain(0, 0), strain_test(0, 0), 1e-09);
  EXPECT_NEAR(strain(0, 1), strain_test(0, 1), 1e-09);
  EXPECT_NEAR(strain(0, 2), strain_test(0, 2), 1e-09);
  EXPECT_NEAR(strain(1, 0), strain_test(1, 0), 1e-09);
  EXPECT_NEAR(strain(1, 1), strain_test(1, 1), 1e-09);
  EXPECT_NEAR(strain(1, 2), strain_test(1, 2), 1e-09);
  EXPECT_NEAR(strain(2, 0), strain_test(2, 0), 1e-09);
  EXPECT_NEAR(strain(2, 1), strain_test(2, 1), 1e-09);
  EXPECT_NEAR(strain(2, 2), strain_test(2, 2), 1e-09);

  Real lambda = C(0, 0, 1, 1);
  Real G = C(0, 1, 0, 1);
  Real K = lambda + 2 * G / LIBMESH_DIM;
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  RankTwoTensor strain_formula = stress.trace() / 9 / K * I2 + stress.deviatoric() / 2 / G;
  EXPECT_NEAR(strain(0, 0), strain_formula(0, 0), 1e-09);
  EXPECT_NEAR(strain(0, 1), strain_formula(0, 1), 1e-09);
  EXPECT_NEAR(strain(0, 2), strain_formula(0, 2), 1e-09);
  EXPECT_NEAR(strain(1, 0), strain_formula(1, 0), 1e-09);
  EXPECT_NEAR(strain(1, 1), strain_formula(1, 1), 1e-09);
  EXPECT_NEAR(strain(1, 2), strain_formula(1, 2), 1e-09);
  EXPECT_NEAR(strain(2, 0), strain_formula(2, 0), 1e-09);
  EXPECT_NEAR(strain(2, 1), strain_formula(2, 1), 1e-09);
  EXPECT_NEAR(strain(2, 2), strain_formula(2, 2), 1e-09);
}
