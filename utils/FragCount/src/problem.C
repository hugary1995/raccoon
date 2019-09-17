#include "problem.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <exception>
#include <limits>
#include <fstream>
#include <set>

problem::problem(const char * filename)
{
  _mesh = new mesh(filename);
  _num_elems = _mesh->elems().size();

  // initialize queues
  _all = new cluster(_mesh->elems());
  _raw = new cluster(_num_elems);
  _good = new cluster(_num_elems);
  _bad = new cluster(_num_elems);

  // initialize elem_to_elems map
  std::cout << "[  0%] Building element to connected elements map...\n";
  double progress = 0.0;
  double threshold = 0.0;
  _elem_to_elems_map.resize(_num_elems);
  for (size_t i = 0; i < _num_elems; i++)
  {
    for (size_t j = 0; j < _num_elems; j++)
      if (i != j && (*_all)[i]->is_connected_to((*_all)[j]))
        _elem_to_elems_map[i].push_back(j);
    progress = (double)i / (double)_num_elems * 100.0;
    if (progress >= threshold)
    {
      std::cout << "[" << std::setw(3) << (int)progress
                << "%] Building element to connected elements map...\n";
      threshold += 10.0;
    }
  }
  std::cout << "[100%] Building element to connected elements map...\n";

  // initially, put all elems in cluster 0
  // all elements' cluster id are set to 0 already
  cluster * new_cluster = new cluster(_mesh->elems());
  _clusters.push_back(new_cluster);

  plt::backend("TkAgg");
}

void
problem::classify_all_times()
{
  std::ofstream out;
  out.open(_mesh->filename() + ".stat.csv");
  out << "step,num_fragments,mean,std\n";
  for (int i = 1; i <= num_time_steps(); i++)
  {
    go_to_time_step(i);
    classify();
    out << i << "," << _num_fragments << "," << _mean << "," << _sigma << std::endl;
    if (_num_fragments >= 1)
      plot_damage();
    std::cout << "================================================================\n";
  }
  out.close();
}

void
problem::plot_damage()
{
  plt::clf();
  std::vector<double> x, y, d;
  std::vector<node *> nodes = _mesh->nodes();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    x.push_back(nodes[i]->x());
    y.push_back(nodes[i]->y());
    d.push_back(nodes[i]->d());
  }
  plt::scatter(x, y, 1, d);

  for (size_t i = 0; i < _num_elems; i++)
  {
    if (!(*_bad)[i])
      continue;
    std::vector<size_t> connected_elems = _elem_to_elems_map[i];
    for (size_t j = 0; j < connected_elems.size(); j++)
    {
      if ((*_bad)[i]->cluster() != (*_all)[connected_elems[j]]->cluster())
      {
        std::vector<double> x, y;
        if ((*_bad)[i]->common_edge((*_all)[connected_elems[j]], x, y))
          plt::plot(x, y, "k-");
      }
    }
  }

  plt::save(_mesh->filename() + ".damage_" + std::to_string(_step) + ".png");
}

void
problem::plot_boundary_elems()
{
  plt::clf();
  std::vector<double> x, y;
  std::vector<T3 *> elems = _mesh->elems();
  for (unsigned int i = 0; i < elems.size(); i++)
  {
    if (!_mesh->is_boundary_elem(elems[i]))
      continue;
    x.push_back(elems[i]->xc());
    y.push_back(elems[i]->yc());
  }
  plt::plot(x, y, ".");
  plt::show();
}

void
problem::PCA()
{
  std::ofstream out;
  out.open(_mesh->filename() + ".PCA.csv");
  for (size_t i = 0; i < _clusters.size(); i++)
    if (!_clusters[i]->empty() && !_mesh->is_boundary_cluster(_clusters[i]))
    {
      _clusters[i]->PCA();
      out << _clusters[i]->xp() << "," << _clusters[i]->yp() << std::endl;
    }
  out.close();
  std::cout << "[100%] Stage 5: PCA complete!\n";
}

void
problem::reinit_clusters()
{
  for (size_t i = 0; i < _clusters.size(); i++)
    if (!_clusters[i]->empty())
    {
      _clusters[i]->compute_area();
      _clusters[i]->compute_centroid();
    }
    else
      _clusters[i]->clear();
}

void
problem::dessociate_bad()
{
  // remove bad elements from all clusters

  for (size_t i = 0; i < _clusters.size(); i++)
    for (size_t j = 0; j < _num_elems; j++)
    {
      if (!(*_clusters[i])[j])
        continue;
      if (!(*_clusters[i])[j]->good())
      {
        (*_clusters[i])[j]->set_cluster(0);
        _clusters[i]->move_elem_to(j, _bad);
        break;
      }
    }
  reinit_clusters();

  std::cout << "[  0%] Stage 1: Dessociate preserved bad elements.\n";

  for (size_t i = 0; i < _clusters.size(); i++)
    std::cout << "[  0%] Stage 1: Cluster #" << std::setw(3) << i << " has " << _clusters[i]->size()
              << " elements.\n";
}

void
problem::associate_bad()
{
  // volume preserving clustring -- classify bad elements into its closest cluster

  std::cout << "[100%] Stage 4: Group bad elements into existing clusters. [  0%]\n";

  reinit_clusters();

  for (size_t i = 0; i < _clusters.size(); i++)
    std::cout << "[100%] Stage 4: Cluster #" << std::setw(3) << i << " has " << _clusters[i]->size()
              << " elements.\n";

  double threshold, progress;
  double total = (double)_bad->size();
  size_t to_dequeue = _bad->first();
  while (to_dequeue < _num_elems)
  {
    std::vector<size_t> connected_elems = _elem_to_elems_map[to_dequeue];
    double dist = std::numeric_limits<double>::max();
    size_t cluster_id = 0;
    for (size_t i = 0; i < connected_elems.size(); i++)
      if ((*_all)[connected_elems[i]]->cluster() != 0)
      {
        double dist_new = (*_all)[connected_elems[i]]->distance((*_bad)[to_dequeue]);
        if (dist_new < dist)
        {
          dist = dist_new;
          cluster_id = (*_all)[connected_elems[i]]->cluster();
        }
      }
    if (cluster_id != 0)
    {
      (*_bad)[to_dequeue]->set_cluster(cluster_id);
      _bad->move_elem_to(to_dequeue, _clusters[cluster_id]);
    }
    to_dequeue = _bad->next(to_dequeue);

    progress = (1.0 - (double)_bad->size() / total) * 100.0;
    if (progress > threshold)
    {
      std::cout << "[100%] Stage 4: Group bad elements into existing clusters. [" << std::setw(3)
                << (int)progress << "%]\n";
      threshold += 10.0;
    }
  }

  reinit_clusters();

  std::cout << "[100%] Stage 4: Group bad elements into existing clusters. [100%]\n";
}

void
problem::decluster()
{
  for (auto i = _clusters_pending_update.begin(); i != _clusters_pending_update.end(); i++)
  {
    for (size_t j = 0; j < _num_elems; j++)
    {
      if (!(*_clusters[*i])[j])
        continue;
      if (!(*_clusters[*i])[j]->good())
        throw 1;
      _clusters[*i]->move_elem_to(j, _raw);
    }
    std::cout << "[  0%] Stage 2: Declustered cluster #" << std::setw(3) << *i << std::endl;
  }
  reinit_clusters();
  std::cout << "[  0%] Stage 2: Raw contains " << _raw->size() << " elements.\n";
}

void
problem::reclassify()
{
  double progress = 0.0;
  while (!_raw->empty())
  {
    // find a good element to start
    for (size_t i = 0; i < _num_elems; i++)
      if ((*_raw)[i])
      {
        if ((*_raw)[i]->good())
        {
          _raw->move_elem_to(i, _good);
          break;
        }
        else
          _raw->move_elem_to(i, _bad);
      }

    // create new cluster
    if (_good->empty())
      break;
    cluster * new_cluster = new cluster(_num_elems);
    int cluster_id = _clusters.size();
    _clusters.push_back(new_cluster);

    // dequeue the first element in good unclassified:
    // 1. move all of its connected good elements from raw to good unclassified
    // 2. move all of its connected bad elements from raw to bad
    // 3. label it and move it to good classified

    size_t to_dequeue = _good->first();
    while (to_dequeue < _num_elems)
    {
      std::vector<size_t> connected_elems = _elem_to_elems_map[to_dequeue];
      for (size_t i = 0; i < connected_elems.size(); i++)
        if ((*_raw)[connected_elems[i]])
        {
          if ((*_raw)[connected_elems[i]]->good())
            _raw->move_elem_to(connected_elems[i], _good);
          else
            _raw->move_elem_to(connected_elems[i], _bad);
        }
      (*_good)[to_dequeue]->set_cluster(cluster_id);
      _good->move_elem_to(to_dequeue, _clusters[cluster_id]);
      to_dequeue = _good->first();
    }
    reinit_clusters();
    progress = (1.0 - (double)_raw->size() / (double)_num_elems) * 100.0;
    std::cout << "[" << std::setw(3) << (int)progress << "%] Stage 3: "
              << "Cluster #" << std::setw(4) << cluster_id << " has "
              << _clusters[cluster_id]->size()
              << " elements, area = " << _clusters[cluster_id]->area() << ", centroid = ("
              << _clusters[cluster_id]->xc() << ", " << _clusters[cluster_id]->yc() << ").\n";
  }
}

void
problem::statistics()
{
  // calculate cluster statistics

  // areas
  std::vector<double> areas;
  double total;
  for (size_t i = 0; i < _clusters.size(); i++)
  {
    std::cout << "[100%] Stage 5: Cluster #" << std::setw(3) << i << " has " << _clusters[i]->size()
              << " elements.\n";
    if (_clusters[i]->size() > 0 && !_mesh->is_boundary_cluster(_clusters[i]))
    {
      areas.push_back(_clusters[i]->area());
      total += _clusters[i]->area();
    }
  }

  _num_fragments = areas.size();
  std::cout << "[100%] Stage 5: Number of clusters: " << _num_fragments << std::endl;

  if (_num_fragments >= 1)
  {
    _mean = total / _num_fragments;
    std::cout << "[100%] Stage 5: Cluster area mean: " << _mean << std::endl;

    _sigma = 0.0;
    for (size_t i = 0; i < _num_fragments; i++)
      _sigma += (areas[i] - _mean) * (areas[i] - _mean);
    _sigma /= areas.size();
    _sigma = std::sqrt(_sigma);

    std::cout << "[100%] Stage 5: Cluster area std: " << _sigma << std::endl;
  }
  else
  {
    _mean = 0.0;
    _sigma = 0.0;
  }
}

void
problem::classify()
{
  dessociate_bad();

  decluster();

  reclassify();

  associate_bad();

  statistics();

  //
  // // time to plot
  // plt::clf();
  // std::vector<std::string> colors = {"b", "g", "r", "c", "m", "y", "k"};
  // std::map<std::string, std::string> keywords;
  // for (unsigned int i = 0; i < good_classified.size(); i++)
  // {
  //   std::vector<double> x, y;
  //   x.push_back(good_classified[i]->n1()->x());
  //   x.push_back(good_classified[i]->n2()->x());
  //   x.push_back(good_classified[i]->n3()->x());
  //   y.push_back(good_classified[i]->n1()->y());
  //   y.push_back(good_classified[i]->n2()->y());
  //   y.push_back(good_classified[i]->n3()->y());
  //   keywords["facecolor"] = colors[good_classified[i]->cluster() % colors.size()];
  //   keywords["edgecolor"] = colors[good_classified[i]->cluster() % colors.size()];
  //   keywords["linewidth"] = "1";
  //   plt::fill(x, y, keywords);
  //   std::cout << "[" << std::setw(3) << (int)((double)i / (double)good_classified.size() *
  //   100)
  //             << "%] "
  //             << "Element #" << std::setw(6) << good_classified[i]->id() << " plotted.\n";
  // }
  // for (unsigned int i = 0; i < bad.size(); i++)
  // {
  //   std::vector<double> x, y;
  //   x.push_back(bad[i]->n1()->x());
  //   x.push_back(bad[i]->n2()->x());
  //   x.push_back(bad[i]->n3()->x());
  //   y.push_back(bad[i]->n1()->y());
  //   y.push_back(bad[i]->n2()->y());
  //   y.push_back(bad[i]->n3()->y());
  //   keywords["facecolor"] = colors[bad[i]->cluster() % colors.size()];
  //   keywords["edgecolor"] = colors[bad[i]->cluster() % colors.size()];
  //   keywords["linewidth"] = "1";
  //   plt::fill(x, y, keywords);
  //   std::cout << "[" << std::setw(3) << (int)((double)i / (double)bad.size() * 100) << "%] "
  //             << "Element #" << std::setw(6) << bad[i]->id() << " plotted.\n";
  // }
  //
  // // plot boundaries
  // std::map<std::string, std::string> keywords_boundary;
  // keywords_boundary["linewidth"] = "1";
  // keywords_boundary["color"] = "w";
  // keywords_boundary["linestyle"] = "-";
  // for (unsigned int i = 0; i < bad.size(); i++)
  // {
  //   unsigned int common_edges_count = 0;
  //   for (unsigned int j = 0; j < bad.size(); j++)
  //   {
  //     std::vector<double> x;
  //     std::vector<double> y;
  //     bool connected = bad[i]->common_edge(bad[j], x, y);
  //     if (connected && bad[i]->cluster() != bad[j]->cluster())
  //     {
  //       plt::plot(x, y, keywords_boundary);
  //       common_edges_count++;
  //     }
  //     if (common_edges_count == 3)
  //       break;
  //   }
  //   std::cout << "[" << std::setw(3) << (int)((double)i / (double)bad.size() * 100) << "%] "
  //             << "Element #" << std::setw(6) << bad[i]->id() << " processed boundary.\n";
  // }
  //
  // plt::xlim(0, 100);
  // plt::ylim(0, 100);
  // plt::axis("equal");
  // plt::save("./clustered");
}
