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
  void plot_boundary_elems();
  void classify_all_times();
  void PCA();
  int num_time_steps() { return _mesh->num_time_steps(); }
  void go_to_time_step(int step)
  {
    _clusters_pending_update = _mesh->read_damage_at_step(step);
    _step = step;
  }
  void reinit_clusters();
  void dessociate_bad();
  void associate_bad();
  void decluster();
  void statistics();
  void reclassify();
  void classify();

protected:
private:
  mesh * _mesh;

  /// number of elements
  size_t _num_elems;

  /// element to elements map
  std::vector<std::vector<size_t>> _elem_to_elems_map;

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

  /// vector of boundary clusters
  std::vector<cluster *> _boundary_clusters;

  /// clusters pending update after damage changed
  std::set<size_t> _clusters_pending_update;

  /// number of clusters
  size_t _num_fragments;

  /// mean
  double _mean;

  /// std
  double _sigma;

  /// step
  int _step;
};
