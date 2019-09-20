#include "ProblemDefinition.h"

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

ProblemDefinition::ProblemDefinition(std::string input)
{
  YAML::Node config = YAML::LoadFile(input);
  std::cout << config << std::endl;
}
