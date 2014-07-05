/**
 * @file instance.cpp
 * @brief 
 */

#include "instance.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

void Instance::build_costs_matrix(double* C) {
    srand (time(NULL));

    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            double cost = (double) (rand() % cost_upper_bound);

            C[i*nodes+j] = cost;
            C[j*nodes+i] = cost;

            if (i == j)
                C[i*nodes+j] = 0.0;
        }
    }

    for (int i = 0; i < nodes*nodes; i++) {
        std::cout << C[i] << ", ";
        if (i % nodes == nodes-1)
            std::cout << std::endl;
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