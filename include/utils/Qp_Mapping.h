#pragma once
#include <unordered_map>
#include <stdexcept>
#include "MooseEnum.h"
#include "MooseError.h"
namespace QpMapping
{
// Moose → SIERRA
inline const std::unordered_map<int, int> TET4_2nd_lookup = {{1, 4}, {2, 1}, {3, 2}, {4, 3}};
inline const std::unordered_map<int, int> TET4_4th_lookup = {
    {1, 1}, {2, 5}, {3, 4}, {4, 3}, {5, 2}};
inline const std::unordered_map<int, int> TET10_4th_lookup = {
    {1, 1}, {2, 5}, {3, 4}, {4, 3}, {5, 2}, {6, 6}, {7, 11}, {8, 10}, {9, 7}, {10, 9}, {11, 8}};
inline const std::unordered_map<int, int> HEX8_3rd_lookup = {
    {1, 8}, {2, 7}, {3, 6}, {4, 5}, {5, 4}, {6, 3}, {7, 2}, {8, 1}};

// SIERRA → Moose
inline const std::unordered_map<int, int> TET4_2nd_lookup_rev = {{4, 1}, {1, 2}, {2, 3}, {3, 4}};
inline const std::unordered_map<int, int> TET4_4th_lookup_rev = {
    {1, 1}, {5, 2}, {4, 3}, {3, 4}, {2, 5}};
inline const std::unordered_map<int, int> TET10_4th_lookup_rev = {
    {1, 1}, {5, 2}, {4, 3}, {3, 4}, {2, 5}, {6, 6}, {11, 7}, {10, 8}, {7, 9}, {9, 10}, {8, 11}};
inline const std::unordered_map<int, int> HEX8_3rd_lookup_rev = {
    {8, 1}, {7, 2}, {6, 3}, {5, 4}, {4, 5}, {3, 6}, {2, 7}, {1, 8}};

// Element types
enum class Element
{
  TET4_2nd,
  TET4_4th,
  TET10_4th,
  HEX8_3rd
};

inline constexpr const char * ELEMENT_ENUM_DEFINITION = "TET4_2nd TET4_4th TET10_4th HEX8_3rd";

// Helper to get qp number and catch out of range qps
unsigned int getQP(unsigned int qp, const std::unordered_map<int, int> * lookup);

// Helper to get either Moose→SIERRA or SIERRA→Moose map with qpnum
const std::unordered_map<int, int> *
getLookup(Element elem, unsigned int & qpnum, bool reversed = false);
}
