/**
 * @file Instance.h
 * @brief TSP data
 *
 */

#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class Instance
{
public:
  Instance(int n, int a, int b, int si) : nodes_card(n), x_bound(a), y_bound(b), start_index(si) { }
  int nodes_card;
  std::vector< std::vector<double> > costs;

  void build()
  {

  }
};

#endif /*  INSTANCE_H */
