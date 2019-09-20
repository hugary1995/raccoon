#pragma once

#include <string>
#include <vector>

class ProblemDefinition
{
public:
  ProblemDefinition(std::string input);

  struct exodus_t
  {
    std::string filename;
    std::string damage_variable;
    std::vector<std::string> boundaries;
  } exodus;

  struct algorithm_t
  {
    bool preserve_volume;
    double cut_off;
    int time_step_begin;
    int time_step_end;
  } algorithm;

  struct output_t
  {
    struct fragments_t
    {
      bool enable;
      std::string append;
      bool include_boundary_fragments;
      int interval;
      std::string colormap;
    } fragments;

    struct statistics_t
    {
      bool enable;
      std::string append;
      bool include_boundary_fragments;
      int interval;
      bool number_of_fragments;
      bool mean_fragment_size;
      bool std_fragment_size;
    } statistics;

    struct PCA_t
    {
      bool enable;
      std::string append;
      bool include_boundary_fragments;
      int interval;
    } PCA;
  } output;

protected:
private:
};
