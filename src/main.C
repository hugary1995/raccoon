//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonTestApp.h"
#include "MooseMain.h"

// Begin the main program.
int
main(int argc, char * argv[])
{
  Moose::main<raccoonTestApp>(argc, argv);

  return 0;
}
