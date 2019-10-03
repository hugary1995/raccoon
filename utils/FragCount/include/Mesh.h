#pragma once

#include "Cluster.h"
#include <string>
#include <cstring>
#include <set>
#include "ProblemDefinition.h"

class Mesh
{
public:
  Mesh(ProblemDefinition config);
  ~Mesh();

  std::vector<Node *> Nodes() { return _nodes; }
  std::vector<T3 *> elems() { return _elems; }
  int numTimeSteps() { return _nun_time_steps; }
  std::set<size_t> readDamageAtStep(int step);
  bool isBoundaryElem(T3 * e);
  bool isBoundaryCluster(Cluster * c);

protected:
private:
  void readExodus();
  void readMeshParameters();
  void readBoundaryNodeSet();
  void readNodes();
  void readBlocks();
  void readElements();
  void readTimes();
  void buildElementToNeighborElementsMap();

  ProblemDefinition _config;
  int _exoid;
  int _error;
  int _num_dim;
  int _num_nodes;
  int _num_elems;
  int _num_node_sets;
  int _num_side_sets;
  int _num_blks;
  std::set<int> _boundary_nodes;
  int * _blk_ids;
  int * _num_nodes_per_elem;
  int * _num_elems_in_blk;
  float _version;
  int _num_nodal_vars;
  int _nun_time_steps;
  float * _time_values;

  std::vector<Node *> _nodes;
  std::vector<T3 *> _elems;
};
