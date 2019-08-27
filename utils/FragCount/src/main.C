// external
// #include "csv.h"

// FragCount includes
#include "problem.h"

node *
find_node(std::vector<node *> & nodes, unsigned int nid)
{
  for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
    if ((*itr)->id() == nid)
      return *itr;

  return NULL;
}

std::vector<T3 *>
find_connected_elems(std::vector<T3 *> & elems, T3 * e)
{
  std::vector<T3 *> connected_elems;

  for (auto itr = elems.begin(); itr != elems.end(); itr++)
    if ((*itr)->is_connected_to(e))
      connected_elems.push_back(*itr);

  return connected_elems;
}

int
main(int argc, char const * argv[])
{
  problem p(argv[1]);

  for (int i = 1; i <= p.num_time_steps(); i++)
  {
    p.go_to_time_step(i);
    p.classify();
  }

  return 0;
}
