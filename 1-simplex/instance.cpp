/**
 * @file instance.cpp
 * @brief 
 */

#include "instance.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

using namespace std;

void Instance::build_graph() {
    srand (time(NULL));

    this->costs = new double[this->nodes_card*this->nodes_card];
    
    // casually generated nodes inside some bounds
    for (int i = 0; i < nodes_card; i++) {
        double x = (double) (rand() % x_bound);
        double y = (double) (rand() % y_bound);

        Node* node = new Node(x, y);
        this->nodes.push_back(*node);
    }

    cout << "calculate euclidean distances" << endl;
    // calculate euclidean distances
        
    cout << "nodes: " << this->nodes.size() << endl;

    for (int i = 0; i < nodes_card; i++) {
        for (int j = 0; j < nodes_card; j++) {
            Node fnode = this->nodes[i];
            cout << "fnode.x: " << fnode.x << endl;
            Node snode = this->nodes[j];
            cout << "snode.x: " << snode.x << endl;

            //somma dei quadrati delle diff tra le coord
            double cost = sqrt( pow((snode.x-fnode.x), 2) + pow((snode.y-fnode.y), 2) );

            this->costs[i*nodes_card+j] = cost;
            this->costs[j*nodes_card+i] = cost;
            cout << "cost: " << cost << endl;
        }
    }

}   


// {
//     0, 1, 2, 3, 4, 5,
//     6, 7, 8, 9, 10, 11,
//     12, 13, 14, 15, 16, 17,
//     18, 19, 20, 21, 22, 23,
//     24, 25, 26, 27, 28, 29,
//     30, 31, 32, 33, 34, 35
// }
