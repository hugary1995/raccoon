#include "Qp_Mapping.h"

namespace QpMapping
{

unsigned int
getQP(unsigned int qp, const std::unordered_map<int, int> * lookup)
{
  auto it = lookup->find(qp);
  if (it == lookup->end())
  {
    mooseError("QP not found in lookup table");
  }
  return it->second;
}

const std::unordered_map<int, int> *
getLookup(Element elem, unsigned int & qpnum, bool reversed)
{
  switch (elem)
  {
    case Element::TET4_2nd:
      qpnum = 4;
      return reversed ? &TET4_2nd_lookup_rev : &TET4_2nd_lookup;
    case Element::TET4_4th:
      qpnum = 5;
      return reversed ? &TET4_4th_lookup_rev : &TET4_4th_lookup;
    case Element::TET10_4th:
      qpnum = 11;
      return reversed ? &TET10_4th_lookup_rev : &TET10_4th_lookup;
    case Element::HEX8_3rd:
      qpnum = 8;
      return reversed ? &HEX8_3rd_lookup_rev : &HEX8_3rd_lookup;
    default:
      throw std::runtime_error("Unknown element type in QpMapping");
  }
}
}