#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "yaml-cpp/yaml.h"

class ProblemDefinition
{
public:
  ProblemDefinition(std::string input)
  {
    YAML::Node config = YAML::LoadFile(input);

    configureExodus(config);
    configureAlgorithm(config);
    configureOutput(config);
  }

  template <typename T>
  void parseRequiredParam(T & to, YAML::Node & from, const std::string parameter)
  {
    if (from[parameter])
      to = from[parameter].as<T>();
    else
      throw std::runtime_error("missing required parameter " + parameter);
  }

  template <typename T>
  void parseOptionalParam(T & to, YAML::Node & from, const std::string parameter)
  {
    if (from[parameter])
      to = from[parameter].as<T>();
  }

  template <typename T>
  void parseRequiredList(std::vector<T> & to, YAML::Node & from, const std::string parameter)
  {
    if (from[parameter])
    {
      to.clear();
      for (size_t i = 0; i < from[parameter].size(); i++)
        to.push_back(from[parameter][i].as<T>());
    }
    else
      throw std::runtime_error("missing required parameter " + parameter);
  }

  template <typename T>
  void parseOptionalList(std::vector<T> & to, YAML::Node & from, const std::string parameter)
  {
    if (from[parameter])
    {
      to.clear();
      for (size_t i = 0; i < from[parameter].size(); i++)
        to.push_back(from[parameter][i].as<T>());
    }
  }

  void configureExodus(YAML::Node config)
  {
    YAML::Node exo_node = config["exodus"];

    parseRequiredParam<std::string>(exodus.file_name, exo_node, "file name");
    parseOptionalParam<std::string>(exodus.damage_variable, exo_node, "damage variable");
    parseOptionalList<std::string>(exodus.boundaries, exo_node, "external boundaries");
  }

  void configureAlgorithm(YAML::Node config)
  {
    YAML::Node alg_node = config["algorithm"];

    parseOptionalParam<bool>(algorithm.preserve_volume, alg_node, "preserve_volume");
    parseOptionalParam<double>(algorithm.cut_off, alg_node, "damage cut off");
    parseOptionalParam<int>(algorithm.time_step_begin, alg_node, "time step begin");
    parseOptionalParam<int>(algorithm.time_step_end, alg_node, "time step end");
  }

  void configureOutput(YAML::Node config)
  {
    YAML::Node out_node = config["output"];
    YAML::Node frag_node = out_node["fragments"];
    YAML::Node stat_node = out_node["statistics"];
    YAML::Node pca_node = out_node["PCA"];
    YAML::Node size_node = out_node["fragment_size"];

    if (frag_node)
    {
      parseOptionalParam<bool>(output.fragments.enable, frag_node, "enable");
      parseOptionalParam<std::string>(output.fragments.append, frag_node, "append");
      parseOptionalParam<bool>(
          output.fragments.include_boundary_fragments, frag_node, "include boundary fragments");
      parseOptionalParam<int>(output.fragments.interval, frag_node, "interval");
      parseOptionalParam<std::string>(output.fragments.colormap, frag_node, "colormap");
      parseOptionalParam<std::string>(output.fragments.backend, frag_node, "backend");
    }

    if (stat_node)
    {
      parseOptionalParam<bool>(output.statistics.enable, stat_node, "enable");
      parseOptionalParam<std::string>(output.statistics.append, stat_node, "append");
      parseOptionalParam<bool>(
          output.statistics.include_boundary_fragments, stat_node, "include boundary fragments");
      parseOptionalParam<int>(output.statistics.interval, stat_node, "interval");
      parseOptionalParam<bool>(
          output.statistics.number_of_fragments, stat_node, "number of fragments");
      parseOptionalParam<bool>(
          output.statistics.mean_fragment_size, stat_node, "mean fragment size");
      parseOptionalParam<bool>(output.statistics.std_fragment_size, stat_node, "std fragment size");
    }

    if (pca_node)
    {
      parseOptionalParam<bool>(output.PCA.enable, pca_node, "enable");
      parseOptionalParam<std::string>(output.PCA.append, pca_node, "append");
      parseOptionalParam<bool>(
          output.PCA.include_boundary_fragments, pca_node, "include boundary fragments");
      parseOptionalParam<int>(output.PCA.interval, pca_node, "interval");
    }

    if (size_node)
    {
      parseOptionalParam<bool>(output.fragment_size.enable, size_node, "enable");
      parseOptionalParam<std::string>(output.fragment_size.append, size_node, "append");
      parseOptionalParam<bool>(
          output.fragment_size.include_boundary_fragments, size_node, "include boundary fragments");
      parseOptionalParam<int>(output.fragment_size.interval, size_node, "interval");
    }
  }

  struct exodus_t
  {
    std::string file_name;
    std::string damage_variable = "d";
    std::vector<std::string> boundaries = {"top", "bottom", "left", "right"};
  } exodus;

  struct algorithm_t
  {
    bool preserve_volume = true;
    double cut_off = 0.75;
    int time_step_begin = 1;
    int time_step_end = std::numeric_limits<int>::max();
  } algorithm;

  struct output_t
  {
    struct fragments_t
    {
      bool enable = true;
      std::string append = "frag";
      bool include_boundary_fragments = true;
      int interval = 0;
      std::string colormap = "jet";
      std::string backend = "TkAgg";
    } fragments;

    struct statistics_t
    {
      bool enable = true;
      std::string append = "stat";
      bool include_boundary_fragments = true;
      int interval = 1;
      bool number_of_fragments = true;
      bool mean_fragment_size = true;
      bool std_fragment_size = true;
    } statistics;

    struct PCA_t
    {
      bool enable = true;
      std::string append = "PCA";
      bool include_boundary_fragments = true;
      int interval = 0;
    } PCA;

    struct fragment_size_t
    {
      bool enable = true;
      std::string append = "size";
      bool include_boundary_fragments = true;
      int interval = 0;
    } fragment_size;
  } output;

protected:
private:
};
