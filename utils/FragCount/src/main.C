// FragCount includes
#include "problem.h"

int
main(int argc, char const * argv[])
{
  problem p(argv[1]);
  p.classify_all_times();
  p.PCA();

  return 0;
}
