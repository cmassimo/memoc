/**
* @file Solution.h
* @brief TSP solution
*
*/

#ifndef SOLUTION_H
#define SOLUTION_H

#include "Instance.h"

#include <vector>

using namespace std;

/**
* TSP Solution representation: ordered sequence of nodes
*/
class Solution
{
    public:
        vector<int> sequence;

        /** Constructor 
        * build a standard solution as the sequence <0, 1, 2, 3 ... n-1, 0>
        * @param tsp TSP instance
        * @return ---
        */
        Solution( const Instance& tsp ) {
            sequence.reserve(tsp.nodes_card + 1);
            for ( int i = 0; i < tsp.nodes_card ; ++i ) {
                sequence.push_back(i);
            }

            sequence.push_back(0);
        }

        /** Copy constructor 
        * build a solution from another
        * @param tspSol TSP solution
        * @return ---
        */
        Solution(const Solution& tspSol) {
            sequence.reserve(tspSol.sequence.size());

            for ( uint i = 0; i < tspSol.sequence.size(); ++i ) {
                sequence.push_back(tspSol.sequence[i]);
            }
        }

        /** print method 
        * @param ---
        * @return ---
        */
        void print ( void ) {
            int max_print_size = (sequence.size() > 20 ? 20 : sequence.size());

            for ( int i = 0; i < max_print_size; i++ )
              cout << sequence[i] << " ";

            if (sequence.size() > 20)
                cout << "..." << endl;
        }

        /** assignment method 
        * copy a solution into another one
        * @param right TSP solution to get into
        * @return ---
        */
        Solution& operator=(const Solution& right) {
            // Handle self-assignment:
            if(this == &right) return *this;

            for ( uint i = 0; i < sequence.size(); i++ )
                sequence[i] = right.sequence[i];

            return *this;
        }

        /** crossover operator (Ricombinazione)
         * creates a new Solution from two parents
         *
         *
         */
        Solution& operator*(const Solution& other) {
            // TODO
            return *this;
        }
};

#endif /* SOLUTION_H */
