// external
// #include "csv.h"

// FragCount includes
#include "mesh.h"

node *
find_node(std::vector<node *> & nodes, unsigned int nid)
{
  for (auto itr = nodes.begin(); itr != nodes.end(); itr++)
    if ((*itr)->id() == nid)
      return *itr;

  return NULL;
}

std::vector<T3 *>
find_connected_elems(std::vector<T3 *> & elems, T3 * e)
{
  std::vector<T3 *> connected_elems;

  for (auto itr = elems.begin(); itr != elems.end(); itr++)
    if ((*itr)->is_connected_to(e))
      connected_elems.push_back(*itr);

  return connected_elems;
}

std::vector<T3 *>::iterator
move_elem_from_to(std::vector<T3 *>::iterator e, std::vector<T3 *> & from, std::vector<T3 *> & to)
{
  to.push_back(*e);
  auto ee = from.erase(e);
  if (ee == from.end() && ee != from.begin())
    ee--;
  return ee;
}

int
main(int argc, char const * argv[])
{
  mesh m(argv[1]);

  m.plot_mesh();

  //
  // // read damage
  //
  // io::CSVReader<4> csv_nodes(argv[1]);
  // csv_nodes.read_header(io::ignore_extra_column, "Point ID", "Points:0", "Points:1", "d");
  // std::vector<node *> nodes;
  // unsigned int _nid;
  // double _x;
  // double _y;
  // double _d;
  // while (csv_nodes.read_row(_nid, _x, _y, _d))
  // {
  //   node * _n = new node(_nid, _x, _y, _d);
  //   nodes.push_back(_n);
  // }
  //
  // // for (unsigned int i = 0; i < d.size(); i++)
  // //   std::cout << d[i] << std::endl;
  //
  // // read connectivity
  //
  // io::CSVReader<4> csv_conn(argv[2]);
  // csv_conn.read_header(
  //     io::ignore_extra_column, "Cell ID", "Point Index 0", "Point Index 1", "Point Index 2");
  // std::vector<T3 *> elems;
  // unsigned int _eid, _n1, _n2, _n3;
  // double A = 0.0;
  // while (csv_conn.read_row(_eid, _n1, _n2, _n3))
  // {
  //   T3 * _e =
  //       new T3(_eid, find_node(nodes, _n1), find_node(nodes, _n2), find_node(nodes, _n3),
  //       0.5);
  //   elems.push_back(_e);
  //   std::cout << "Element #" << std::setw(6) << _e->id() << ", intact = " << _e->good()
  //             << ", area = " << _e->area() << std::endl;
  //   A += _e->area();
  // }
  // std::cout << "total area = " << A << std::endl;
  //
  // // for (unsigned int i = 0; i < connectivity.size(); i++)
  // //   std::cout << connectivity[i][0] << ", " << connectivity[i][1] << ", " << connectivity[i][2]
  // //             << std::endl;
  //
  // // start classification
  // // create four queues
  // // queue1: good unclassified elements
  // // queue2: good classified elements
  // // queue3: bad elements
  // // queue4: raw elements
  //
  // // initialize queues
  // std::vector<T3 *> good_unclassified;
  // std::vector<T3 *> good_classified;
  // std::vector<T3 *> bad;
  // std::vector<T3 *> raw = elems;
  //
  // // initialize cluster count
  // int cluster = -1;
  //
  // while (!raw.empty())
  // {
  //   cluster++;
  //   // find a good element to start
  //   auto itr = raw.begin();
  //   for (; itr != raw.end(); itr++)
  //     if ((*itr)->good())
  //     {
  //       itr = move_elem_from_to(itr, raw, good_unclassified);
  //       break;
  //     }
  //     else
  //     {
  //       std::cout << "[" << std::setw(3)
  //                 << (int)(((double)good_classified.size() + (double)bad.size()) /
  //                          (double)elems.size() * 100)
  //                 << "%] "
  //                 << "Element #" << std::setw(6) << (*itr)->id() << " classified as bad.\n";
  //       itr = move_elem_from_to(itr, raw, bad);
  //       if (itr == raw.end())
  //         break;
  //     }
  //
  //   // dequeue the first element in good unclassified:
  //   // 1. move all of its connected good elements from raw to good unclassified
  //   // 2. move all of its connected bad elements from raw to bad
  //   // 3. label it and move it to good classified
  //
  //   while (!good_unclassified.empty())
  //   {
  //     for (auto itr = raw.begin(); itr != raw.end(); itr++)
  //       if ((*itr)->is_connected_to(*good_unclassified.begin()))
  //       {
  //         if ((*itr)->good())
  //           itr = move_elem_from_to(itr, raw, good_unclassified);
  //         else
  //         {
  //           std::cout << "[" << std::setw(3)
  //                     << (int)(((double)good_classified.size() + (double)bad.size()) /
  //                              (double)elems.size() * 100)
  //                     << "%] "
  //                     << "Element #" << std::setw(6) << (*itr)->id() << " classified as
  //                     bad.\n";
  //           itr = move_elem_from_to(itr, raw, bad);
  //         }
  //       }
  //     (*good_unclassified.begin())->set_cluster(cluster);
  //     std::cout << "[" << std::setw(3)
  //               << (int)(((double)good_classified.size() + (double)bad.size()) /
  //                        (double)elems.size() * 100)
  //               << "%] "
  //               << "Element #" << std::setw(6) << (*good_unclassified.begin())->id()
  //               << " classified as cluster #" << cluster << std::endl;
  //     move_elem_from_to(good_unclassified.begin(), good_unclassified, good_classified);
  //   }
  // }
  //
  // // volume preserving clustring -- classify bad elements into its closest cluster
  // for (unsigned int i = 0; i < bad.size(); i++)
  // {
  //   double dist = bad[i]->distance(good_classified[0]);
  //   bad[i]->set_cluster(good_classified[0]->cluster());
  //   for (unsigned int j = 1; j < good_classified.size(); j++)
  //   {
  //     double dist_new = bad[i]->distance(good_classified[j]);
  //     if (dist_new < dist)
  //     {
  //       dist = dist_new;
  //       bad[i]->set_cluster(good_classified[j]->cluster());
  //     }
  //   }
  //   std::cout << "[" << std::setw(3) << (int)((double)i / (double)bad.size() * 100) << "%] "
  //             << "Bad element #" << std::setw(6) << bad[i]->id() << " classified as cluster
  //             #"
  //             << bad[i]->cluster() << std::endl;
  // }
  //
  // // calculate cluster statistics
  // std::vector<double> cluster_area(cluster);
  // for (unsigned int i = 0; i < elems.size(); i++)
  // {
  //   cluster_area[elems[i]->cluster()] += elems[i]->area();
  //   std::cout << "[" << std::setw(3) << (int)((double)i / (double)elems.size() * 100) << "%]
  //   "
  //             << "Counting area, element #" << std::setw(6) << elems[i]->id() << std::endl;
  // }
  // double max = *std::max_element(cluster_area.begin(), cluster_area.end());
  // std::vector<double> bins;
  // for (double bin = 0.0; bin < max; bin += 10)
  //   bins.push_back(bin);
  // std::vector<double> bin_counts(bins.size());
  // for (unsigned int i = 0; i < cluster_area.size(); i++)
  // {
  //   for (unsigned int j = 0; j < bins.size() - 1; j++)
  //     if (cluster_area[i] >= bins[j] && cluster_area[i] < bins[j + 1])
  //     {
  //       bin_counts[j] += 1;
  //       break;
  //     }
  //   if (cluster_area[i] >= bins[bins.size() - 1])
  //     bins[bins.size() - 1] += 1;
  // }
  // std::map<std::string, std::string> keywords_bar;
  // keywords_bar["align"] = "edge";
  // plt::bar(bins, bin_counts, "k", "-", 5, keywords_bar);
  // plt::save("./distribution");
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

  return 0;
}
