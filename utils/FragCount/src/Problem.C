#include "Problem.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <limits>
#include <fstream>
#include <set>
#include <iostream>
#include <iomanip>

Problem::Problem(const char * filename)
{
  _mesh = new Mesh(filename);
  _num_elems = _mesh->elems().size();

  // initialize queues
  _all = new Cluster(_mesh->elems());
  _unclassified = new Cluster(_num_elems);
  _intact = new Cluster(_num_elems);
  _broken = new Cluster(_num_elems);

  // initially, put all elems in Cluster 0
  // all elements' Cluster id are set to 0 already
  Cluster * new_cluster = new Cluster(_mesh->elems());
  _clusters.push_back(new_cluster);

  plt::backend("TkAgg");
}

void
Problem::classifyAllTimes()
{
  std::ofstream out;
  out.open(_mesh->filename() + ".stat.csv");
  out << "step,num_fragments,mean,std\n";
  for (int i = 1; i <= numTimeSteps(); i++)
  {
    goToTimeStep(i);
    classify();
    out << i << "," << _num_fragments << "," << _mean << "," << _sigma << std::endl;
    // if (_num_fragments >= 1)
    //   plotDamage();
    std::cout << "================================================================\n";
  }
  out.close();
}

void
Problem::plotDamage()
{
  plt::clf();
  std::vector<double> x, y, d;
  std::vector<Node *> Nodes = _mesh->Nodes();
  for (size_t i = 0; i < Nodes.size(); i++)
  {
    x.push_back(Nodes[i]->x());
    y.push_back(Nodes[i]->y());
    d.push_back(Nodes[i]->d());
  }
  plt::scatter(x, y, 1, d);

  for (size_t i = 0; i < _clusters.size(); i++)
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

  plt::save(_mesh->filename() + ".damage_" + std::to_string(_step) + ".png");
}

void
Problem::plotBoundaryElems()
{
  plt::clf();
  std::vector<double> x, y;
  std::vector<T3 *> elems = _mesh->elems();
  for (unsigned int i = 0; i < elems.size(); i++)
  {
    if (!_mesh->isBoundaryElem(elems[i]))
      continue;
    x.push_back(elems[i]->xc());
    y.push_back(elems[i]->yc());
  }
  plt::plot(x, y, ".");
  plt::show();
}

void
Problem::PCA()
{
  std::ofstream out;
  out.open(_mesh->filename() + ".PCA.csv");
  for (size_t i = 0; i < _clusters.size(); i++)
    if (!_clusters[i]->empty() && !_mesh->isBoundaryCluster(_clusters[i]))
    {
      _clusters[i]->PCA();
      out << _clusters[i]->xp() << "," << _clusters[i]->yp() << std::endl;
    }
  out.close();
  std::cout << "Stage 5: PCA complete!\n";
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
  // calculate Cluster statistics

  // areas
  std::vector<double> areas;
  double total;
  for (size_t i = 0; i < _clusters.size(); i++)
    if (_clusters[i]->size() > 0)
    {
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
}

void
Problem::classify()
{
  dessociateBroken();

  decluster();

  reclassify();

  associateBroken();

  statistics();
}
