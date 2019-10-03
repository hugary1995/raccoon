// FragCount includes
#include "Problem.h"

int
main(int argc, char const * argv[])
{
  ProblemDefinition config(argv[1]);
  Problem p(config);
  p.classifyAllTimes();

  return 0;
}
