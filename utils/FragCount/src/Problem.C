#include "Problem.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <limits>
#include <fstream>
#include <set>
#include <iostream>
#include <iomanip>

Problem::Problem(ProblemDefinition config) : _config(config)
{
  _mesh = new Mesh(config);
  _num_elems = _mesh->elems().size();

  // initialize queues
  _all = new Cluster(_mesh->elems());
  _unclassified = new Cluster(_num_elems);
  _intact = new Cluster(_num_elems);
  _broken = new Cluster(_num_elems);

  plt::backend(_config.output.fragments.backend);
}

void
Problem::preClassify()
{
  if (_config.output.statistics.enable)
  {
    _stat_out.open(_config.exodus.file_name + "_" + _config.output.statistics.append + ".dat");
    _stat_out << "step";
    if (_config.output.statistics.number_of_fragments)
      _stat_out << " number_of_fragments";
    if (_config.output.statistics.mean_fragment_size)
      _stat_out << " mean_fragment_size";
    if (_config.output.statistics.std_fragment_size)
      _stat_out << " std_fragment_size";
    _stat_out << "\n";
    _stat_out.flush();
  }

  // initially, put all elems in Cluster 0
  // all elements' Cluster id are set to 0 already
  Cluster * new_cluster = new Cluster(_mesh->elems());
  _clusters.push_back(new_cluster);
}

void
Problem::postClassify()
{
  if (_config.output.statistics.enable)
    _stat_out.close();
}

void
Problem::classifyAllTimes()
{
  preClassify();

  for (int i = _config.algorithm.time_step_begin;
       i <= std::min(_config.algorithm.time_step_end, numTimeSteps());
       i++)
  {
    goToTimeStep(i);
    classify();
    std::cout << "================================================================\n";
  }

  postClassify();
}

void
Problem::classify()
{
  dessociateBroken();

  decluster();

  reclassify();

  if (_config.algorithm.preserve_volume)
    associateBroken();

  if (_config.output.fragments.enable)
    plotDamage();

  if (_config.output.statistics.enable)
    statistics();

  if (_config.output.PCA.enable)
    PCA();

  if (_config.output.fragment_size.enable)
    fragment_size();
}

void
Problem::plotDamage()
{
  // leave if it is not time to output
  if (_config.output.fragments.interval == 0)
  {
    if (_step != std::min(_config.algorithm.time_step_end, numTimeSteps()))
      return;
  }
  else if ((_step - _config.algorithm.time_step_begin) % _config.output.fragments.interval != 0)
    return;

  plt::clf();
  std::vector<double> x, y, d;
  std::vector<Node *> Nodes = _mesh->Nodes();
  for (size_t i = 0; i < Nodes.size(); i++)
  {
    x.push_back(Nodes[i]->x());
    y.push_back(Nodes[i]->y());
    d.push_back(Nodes[i]->d());
  }
  plt::scatter(x, y, 1, d, _config.output.fragments.colormap);

  for (size_t i = 0; i < _clusters.size(); i++)
  {
    if (!_clusters[i])
      continue;
    if (!_config.output.fragments.include_boundary_fragments &&
        _mesh->isBoundaryCluster(_clusters[i]))
      continue;
    for (size_t j = 0; j < _num_elems; j++)
    {
      if (!(*_clusters[i])[j])
        continue;
      if ((*_clusters[i])[j]->good())
        continue;
      std::vector<T3 *> connected_elems = (*_clusters[i])[j]->neighbors();
      for (size_t k = 0; k < connected_elems.size(); k++)
      {
        if ((*_clusters[i])[j]->cluster() != connected_elems[k]->cluster())
        {
          std::vector<double> x, y;
          if ((*_clusters[i])[j]->commonEdge(connected_elems[k], x, y))
            plt::plot(x, y, "k-");
        }
      }
    }
  }

  plt::save(_config.exodus.file_name + "_" + _config.output.fragments.append + "_step_" +
            std::to_string(_step) + ".png");
}

void
Problem::PCA()
{
  // leave if it is not time to output
  if (_config.output.PCA.interval == 0)
  {
    if (_step != std::min(_config.algorithm.time_step_end, numTimeSteps()))
      return;
  }
  else if ((_step - _config.algorithm.time_step_begin) % _config.output.PCA.interval != 0)
    return;

  _pca_out.open(_config.exodus.file_name + "_" + _config.output.PCA.append + "_step_" +
                std::to_string(_step) + ".dat");

  for (size_t i = 0; i < _clusters.size(); i++)
  {
    if (!_config.output.PCA.include_boundary_fragments && _mesh->isBoundaryCluster(_clusters[i]))
      continue;
    if (!_clusters[i]->empty())
    {
      _clusters[i]->PCA();
      _pca_out << _clusters[i]->xp() << "," << _clusters[i]->yp() << std::endl;
    }
  }

  _pca_out.flush();

  _pca_out.close();
  std::cout << "Stage 5: PCA complete!\n";
}

void
Problem::fragment_size()
{
  // leave if it is not time to output
  if (_config.output.fragment_size.interval == 0)
  {
    if (_step != std::min(_config.algorithm.time_step_end, numTimeSteps()))
      return;
  }
  else if ((_step - _config.algorithm.time_step_begin) % _config.output.fragment_size.interval != 0)
    return;

  _size_out.open(_config.exodus.file_name + "_" + _config.output.fragment_size.append + "_step_" +
                 std::to_string(_step) + ".dat");

  for (size_t i = 0; i < _clusters.size(); i++)
  {
    if (!_config.output.fragment_size.include_boundary_fragments &&
        _mesh->isBoundaryCluster(_clusters[i]))
      continue;
    if (!_clusters[i]->empty())
      _size_out << _clusters[i]->area() << std::endl;
  }

  _size_out.flush();

  _size_out.close();
  std::cout << "Stage 5: Fragment size complete!\n";
}

void
Problem::reinitClusters()
{
  for (size_t i = 0; i < _clusters.size(); i++)
    if (!_clusters[i]->empty())
    {
      _clusters[i]->computeArea();
      _clusters[i]->computeCentroid();
    }
    else
      _clusters[i]->clear();
}

void
Problem::dessociateBroken()
{
  // remove bad elements from all Clusters

  for (size_t i = 0; i < _clusters.size(); i++)
    for (size_t j = 0; j < _num_elems; j++)
    {
      if (!(*_clusters[i])[j])
        continue;
      if (!(*_clusters[i])[j]->good())
      {
        (*_clusters[i])[j]->setCluster(0);
        _clusters[i]->moveElemTo(j, _broken);
      }
    }
  reinitClusters();

  std::cout << "Stage 1: Dessociated preserved bad elements.\n";
}

void
Problem::associateBroken()
{
  // volume preserving clustring -- classify bad elements into its closest Cluster

  reinitClusters();

  size_t to_dequeue = _broken->first();
  while (to_dequeue < _num_elems)
  {
    std::vector<T3 *> connected_elems = (*_broken)[to_dequeue]->neighbors();
    double dist = std::numeric_limits<double>::max();
    size_t Cluster_id = 0;
    for (size_t i = 0; i < connected_elems.size(); i++)
      if (connected_elems[i]->cluster() != 0)
      {
        double dist_new = connected_elems[i]->distance((*_broken)[to_dequeue]);
        if (dist_new < dist)
        {
          dist = dist_new;
          Cluster_id = connected_elems[i]->cluster();
        }
      }
    if (Cluster_id != 0)
    {
      (*_broken)[to_dequeue]->setCluster(Cluster_id);
      _broken->moveElemTo(to_dequeue, _clusters[Cluster_id]);
    }
    to_dequeue = _broken->next(to_dequeue);
  }

  reinitClusters();

  std::cout << "Stage 4: Grouped bad elements into existing Clusters.\n";
}

void
Problem::decluster()
{
  std::cout << "Stage 2: Number of Clusters pending update: " << _clusters_pending_update.size()
            << std::endl;
  for (auto i = _clusters_pending_update.begin(); i != _clusters_pending_update.end(); i++)
  {
    for (size_t j = 0; j < _num_elems; j++)
    {
      if (!(*_clusters[*i])[j])
        continue;
      _clusters[*i]->moveElemTo(j, _unclassified);
    }
  }
  reinitClusters();
}

void
Problem::reclassify()
{
  double progress = 0.0;
  double threshold = 0.0;
  double total = (double)_unclassified->size();
  while (!_unclassified->empty())
  {
    // find a good element to start
    for (size_t i = 0; i < _num_elems; i++)
      if ((*_unclassified)[i])
      {
        if ((*_unclassified)[i]->good())
        {
          _unclassified->moveElemTo(i, _intact);
          break;
        }
        else
          _unclassified->moveElemTo(i, _broken);
      }

    // create new Cluster
    if (_intact->empty())
      break;
    Cluster * new_cluster = new Cluster(_num_elems);
    int Cluster_id = _clusters.size();
    _clusters.push_back(new_cluster);

    // dequeue the first element in good unclassified:
    // 1. move all of its connected good elements from raw to good unclassified
    // 2. move all of its connected bad elements from raw to bad
    // 3. label it and move it to good classified

    size_t to_dequeue = _intact->first();
    while (to_dequeue < _num_elems)
    {
      std::vector<T3 *> connected_elems = (*_intact)[to_dequeue]->neighbors();
      for (size_t i = 0; i < connected_elems.size(); i++)
        if ((*_unclassified)[connected_elems[i]->id()])
          _unclassified->moveElemTo(connected_elems[i]->id(), _intact);
      (*_intact)[to_dequeue]->setCluster(Cluster_id);
      _intact->moveElemTo(to_dequeue, _clusters[Cluster_id]);
      to_dequeue = _intact->first();

      progress = (1.0 - (double)_unclassified->size() / total) * 100.0;
      if (progress >= threshold)
      {
        std::cout << "Stage 3: classifying... [" << std::setw(3) << (int)progress << "%]\n";
        threshold += 20.0;
      }
    }
    reinitClusters();
  }
}

void
Problem::statistics()
{
  // leave if it is not time to output
  if (_config.output.statistics.interval == 0)
  {
    if (_step != std::min(_config.algorithm.time_step_end, numTimeSteps()))
      return;
  }
  else if ((_step - _config.algorithm.time_step_begin) % _config.output.statistics.interval != 0)
    return;

  // areas
  std::vector<double> areas;
  double total;
  for (size_t i = 0; i < _clusters.size(); i++)
    if (!_clusters[i]->empty())
    {
      if (!_config.output.statistics.include_boundary_fragments &&
          _mesh->isBoundaryCluster(_clusters[i]))
        continue;
      areas.push_back(_clusters[i]->area());
      total += _clusters[i]->area();
    }

  _num_fragments = areas.size();
  std::cout << "Stage 5: Number of Clusters: " << _num_fragments << std::endl;

  if (_num_fragments >= 1)
  {
    _mean = total / _num_fragments;
    std::cout << "Stage 5: Cluster area mean: " << _mean << std::endl;

    _sigma = 0.0;
    for (size_t i = 0; i < _num_fragments; i++)
      _sigma += (areas[i] - _mean) * (areas[i] - _mean);
    _sigma /= _num_fragments;
    _sigma = std::sqrt(_sigma);

    std::cout << "Stage 5: Cluster area std: " << _sigma << std::endl;
  }
  else
  {
    _mean = 0.0;
    _sigma = 0.0;
  }

  _stat_out << _step;
  if (_config.output.statistics.number_of_fragments)
    _stat_out << " " << _num_fragments;
  if (_config.output.statistics.mean_fragment_size)
    _stat_out << " " << _mean;
  if (_config.output.statistics.std_fragment_size)
    _stat_out << " " << _sigma;
  _stat_out << "\n";
  _stat_out.flush();
}
