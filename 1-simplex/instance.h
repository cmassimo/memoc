/**
 * @file instance.h
 * @brief 
 */

#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
using namespace std;

class Node
{
    public:
        double x;
        double y;

        Node(double a, double b):
            x(a), y(b) {}
};

class Instance
{
    public:
        int nodes_card;
        int x_bound;
        int y_bound;
        int start_idx;
        vector<Node> nodes;
        double* costs;

        Instance(int n, int a, int b, int si =0):
          nodes_card(n), x_bound(a), y_bound(b), start_idx(si) {}

        void build_graph();
};

#endif
