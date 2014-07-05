/**
 * @file instance.h
 * @brief 
 */

#ifndef INSTANCE_H
#define INSTANCE_H

// // data
// const int n = 6;
// const int a = 30;

// const int C[a*2] = { 
//    0,  2, 9,  9,  2,  7,
//    2,  0, 10,  4,  4,  3,
//    9, 10,  0,  1,  5, 11,
//    9,   4,  1,  0,  4, 10,
//    2,   4,  5,  4,  0, 12,
//    7,   3, 11, 10, 12,  0
// };

// const int start_idx = 0;

class Instance
{
    public:
        int nodes;
        int arcs;
        int cost_upper_bound;
        int start_idx;

        Instance(int n, int a, int cub, int si =0):
          nodes(n), arcs(a), cost_upper_bound(cub), start_idx(si) {}

        void build_costs_matrix(double* C);

};

#endif
