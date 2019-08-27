#pragma once

#include "mesh.h"
#include "cluster.h"

class problem
{
public:
  problem(const char * filename);
  ~problem()
  {
    delete _mesh;
    delete _all;
    delete _raw;
    delete _good;
    delete _bad;
    for (size_t i = 0; i < _clusters.size(); i++)
      delete _clusters[i];
  }
  void plot_damage();
  int num_time_steps() { return _mesh->num_time_steps(); }
  void go_to_time_step(int step) { _clusters_pending_update = _mesh->read_damage_at_step(step); }
  void dessociate_bad();
  void associate_bad();
  void decluster();
  void reclassify();
  void classify();

protected:
private:
  mesh * _mesh;

  /// number of elements
  size_t _num_elems;

  /// all elems
  cluster * _all;

  /// queue: unclassified elements
  cluster * _raw;

  /// queue: good unclassified elements
  cluster * _good;

  /// queue: bad elements
  cluster * _bad;

  /// vector of classified clusters
  std::vector<cluster *> _clusters;

  /// clusters pending update after damage changed
  std::set<size_t> _clusters_pending_update;
};
