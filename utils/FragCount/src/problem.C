#include "problem.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <limits>

problem::problem(const char * filename)
{
  _mesh = new mesh(filename);
  _num_elems = _mesh->elems().size();

  // initialize queues
  _all = new cluster(_mesh->elems());
  _raw = new cluster(_num_elems);
  _good = new cluster(_num_elems);
  _bad = new cluster(_num_elems);

  // initially, put all elems in cluster 0
  // all elements' cluster id are set to 0 already
  cluster * new_cluster = new cluster(_mesh->elems());
  _clusters.push_back(new_cluster);

  plt::backend("TkAgg");
}

void
problem::plot_damage()
{
  plt::clf();
  std::vector<double> x, y, d;
  std::vector<node *> nodes = _mesh->nodes();
  for (unsigned int i = 0; i < nodes.size(); i++)
  {
    x.push_back(nodes[i]->x());
    y.push_back(nodes[i]->y());
    d.push_back(nodes[i]->d());
    // std::cout << "[" << std::setw(3) << (int)((double)i / (double)_elems.size() * 100) << "%] "
    //           << "Element #" << std::setw(6) << _elems[i]->id() << " plotted.\n";
  }
  plt::scatter(x, y, 1, d);
  plt::show();
}

void
problem::dessociate_bad()
{
  // remove bad elements from all clusters

  for (size_t i = 0; i < _num_elems; i++)
  {
    if (!(*_bad)[i])
      continue;
    for (size_t j = 0; j < _clusters.size(); j++)
      (*_clusters[j])[i] = NULL;
  }
  std::cout << "[  0%] Stage 1: Dessociate preserved bad elements.\n";
}

void
problem::associate_bad()
{
  // volume preserving clustring -- classify bad elements into its closest cluster

  // compute centroid of each cluster
  for (size_t i = 0; i < _clusters.size(); i++)
    _clusters[i]->compute_centroid();

  for (size_t i = 0; i < _num_elems; i++)
  {
    if (!(*_bad)[i])
      continue;

    double dist = std::numeric_limits<double>::max();
    size_t cluster_id = 0;
    for (size_t j = 0; j < _clusters.size(); j++)
    {
      if (_clusters[j]->empty())
        continue;
      double dist_new = _clusters[j]->distance_to_elem((*_bad)[i]);
      if (dist_new < dist)
      {
        dist = dist_new;
        cluster_id = j;
      }
    }

    (*_clusters[cluster_id])[i] = (*_bad)[i];
    (*_bad)[i]->set_cluster(cluster_id);
  }
  std::cout << "[100%] Stage 4: Grouped bad elements into existing clusters.\n";
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
      _clusters[*i]->move_elem_to(j, _raw);
    }
    std::cout << "[  0%] Stage 2: Declustered cluster #" << std::setw(3) << *i << std::endl;
  }
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
    size_t cluster_id = _clusters.size();
    cluster * new_cluster = new cluster(_num_elems);
    _clusters.push_back(new_cluster);

    // dequeue the first element in good unclassified:
    // 1. move all of its connected good elements from raw to good unclassified
    // 2. move all of its connected bad elements from raw to bad
    // 3. label it and move it to good classified

    size_t to_dequeue = _good->first();
    while (to_dequeue < _num_elems)
    {
      for (size_t i = 0; i < _num_elems; i++)
        if ((*_raw)[i])
        {
          if ((*_raw)[i]->is_connected_to((*_good)[to_dequeue]))
          {
            if ((*_raw)[i]->good())
              _raw->move_elem_to(i, _good);
            else
              _raw->move_elem_to(i, _bad);
          }
        }
      (*_good)[to_dequeue]->set_cluster(cluster_id);
      _good->move_elem_to(to_dequeue, _clusters[cluster_id]);
      to_dequeue = _good->first();
    }
    _clusters[cluster_id]->compute_centroid();
    progress = (1.0 - (double)_raw->size() / (double)_num_elems) * 100.0;
    std::cout << "[" << std::setw(3) << (int)progress << "%] Stage 3: "
              << "Cluster #" << std::setw(3) << cluster_id << " has "
              << _clusters[cluster_id]->size()
              << " elements, area = " << _clusters[cluster_id]->area() << ", centroid = ("
              << _clusters[cluster_id]->xc() << ", " << _clusters[cluster_id]->yc() << ").\n";
  }
}

void
problem::classify()
{
  dessociate_bad();

  decluster();

  reclassify();

  associate_bad();

  // calculate cluster statistics

  // areas
  std::vector<double> areas;
  double total;
  for (size_t i = 0; i < _clusters.size(); i++)
    if (_clusters[i]->size() > 0)
    {
      areas.push_back(_clusters[i]->area());
      total += _clusters[i]->area();
    }

  // remove the largest cluster
  auto max = std::min_element(areas.begin(), areas.end());
  total -= *max;
  areas.erase(max);

  std::cout << "[100%] Stage 5: Number of clusters: " << areas.size() << std::endl;

  if (areas.size() >= 1)
  {
    double mean = total / areas.size();
    std::cout << "[100%] Stage 5: Cluster area mean: " << mean << std::endl;

    double sigma;
    for (size_t i = 0; i < areas.size(); i++)
      sigma += (areas[i] - mean) * (areas[i] - mean);
    sigma /= areas.size();
    sigma = std::sqrt(sigma);

    std::cout << "[100%] Stage 5: Cluster area std: " << sigma << std::endl;
  }

  std::cout << "================================================================\n";

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
