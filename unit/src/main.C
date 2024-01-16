//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonUnitApp.h"
#include "gtest/gtest.h"

// Moose includes
#include "Moose.h"
#include "MooseInit.h"
#include "AppFactory.h"

#include <fstream>
#include <string>

GTEST_API_ int
main(int argc, char ** argv)
{
  // gtest removes (only) its args from argc and argv - so this  must be before moose init
  testing::InitGoogleTest(&argc, argv);

  MooseInit init(argc, argv);
  registerApp(raccoonUnitApp);
  Moose::_throw_on_error = true;

  return RUN_ALL_TESTS();
}
