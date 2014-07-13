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
#include <math.h>

using namespace std;

typedef struct node {
    double x;
    double y;
} Node;

/**
 * Class that describes a TSP instance (a cost matrix, nodes are identified by integer 0 ... n-1)
 */
class Instance
{
    public:
        int nodes_card;
        double x_bound;
        double y_bound;
        int start_index;
        vector< vector<double> > costs;
        vector<Node> nodes;

        Instance(int n, int a, int b, int si) : nodes_card(n), x_bound(a), y_bound(b), start_index(si) {
            srand(time(NULL));

            // build nodes and costs matrix
            
            for (int i = 0; i < nodes_card; i++) {
                Node node;
                node.x = fRand(0.0, x_bound);
                node.y = fRand(0.0, y_bound);

                this->nodes.push_back(node);
            }

            costs.reserve(nodes_card);

            // calculate euclidean distances

            for (int i = 0; i < nodes_card; i++) {
                vector<double> row;
                for (int j = 0; j < nodes_card; j++) {
                    Node fnode = this->nodes[i];
                    //cout << "node " << i << ": (" << fnode.x << "," << fnode.y << ")" << endl;
                    Node snode = this->nodes[j];
                    //cout << "node " << j << ": (" << snode.x << "," << snode.y << ")" << endl;

                    //somma dei quadrati delle diff tra le coord
                    double cost = sqrt( pow((snode.x-fnode.x), 2) + pow((snode.y-fnode.y), 2) );

                    row.push_back(cost);
                    //cout << "cost: " << cost << endl;
                }
                this->costs.push_back(row);
            }

        }

        double fRand(double fMin, double fMax) {
            double f = (double)rand() / RAND_MAX;
            return fMin + f * (fMax - fMin);
        }
};

#endif /*  INSTANCE_H */

