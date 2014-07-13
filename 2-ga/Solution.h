/**
* @file Solution.h
* @brief TSP solution
*
*/

#ifndef SOLUTION_H
#define SOLUTION_H

#include "Instance.h"

#include <vector>
#include <algorithm>

using namespace std;

/**
 * Class representing substring reversal move
 */
typedef struct move {
    int	from;
    int	to;
} Move;

/**
* TSP Solution representation: ordered sequence of nodes
*/
class Solution
{
    public:
        const Instance* instance;
        vector<int> sequence;

        /** Constructor 
        * build a standard solution as the sequence <0, 1, 2, 3 ... n-1>
        * @param tsp TSP instance
        * @return ---
        */
        Solution( const Instance& tsp ) {
            sequence.reserve(tsp.nodes_card);
            instance = &tsp;
//            for ( int i = 0; i < tsp.nodes_card ; ++i ) {
//                sequence.push_back(i);
//            }

        }

        /** Copy constructor 
        * build a solution from another
        * @param tspSol TSP solution
        * @return ---
        */
        Solution(const Solution& tspSol) {
            instance = tspSol.instance;

            sequence.reserve(tspSol.sequence.size());

            for ( uint i = 0; i < tspSol.sequence.size(); ++i ) {
                sequence.push_back(tspSol.sequence[i]);
            }
        }

        /** print method 
        * @param ---
        * @return ---
        */
        void print() const {
            uint max_print_size = (sequence.size() > 30 ? 30 : sequence.size());
            
            if (is_ammissible())
                cout << "(A) ";
            else
                cout << "(I) ";

            cout << "o: " << evaluate() << " seq: [ ";
            for (uint i = 0; i < max_print_size; i++)
              cout << sequence[i] << " ";

            if (sequence.size() > max_print_size)
                cout << "... ]" << endl;
            else
                cout << " ] " << endl;
        }

        /**
        * evaluate a solution (objective function)
        * @param solution solution to be evaluated
        * @param TSP TSP data
        * @return the value of the sequence
        */
        double evaluate() const;

        double fitness(const Solution& best) const;

        Solution& swap(const Move& move);

        bool operator==(const Solution& other) {
            return this->hamming_distance(other) == 0;
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
         * Creates a new Solution from two parents
         * Genera le due soluzioni figlie con cut-point crossover.
         * accetta soluzioni ammissibili con prob 0.85 e inamissibili
         * con prob complementare.
         */
        vector<Solution> operator*(const Solution& other) {
            vector<Solution> children;

            if (*this == other) {
                double p = rand() / (double) RAND_MAX;

                if (this->is_ammissible()) {
                    if (p < 0.85)
                        children.push_back(*this);
                }
                else {
                    if (p > 0.85)
                        children.push_back(*this);
                }
            }
            else {
                uint points_card = (int) ((rand() % 7) + 1);
                if (points_card < 3)
                    points_card = 3;

                vector<int> cut_points;

                cut_points.push_back(0);

                while (cut_points.size() < points_card-1) {
                    int idx = (int) rand() % this->sequence.size();
                    if (find(cut_points.begin(), cut_points.end(), idx) == cut_points.end())
                        cut_points.push_back(idx);
                }
                cut_points.push_back(this->sequence.size());

                sort(cut_points.begin(), cut_points.end());

                while (children.size() < 2) {

                    Solution child1 = Solution(*this->instance);
                    Solution child2 = Solution(*this->instance);

                    int turn = 0;
                    for (uint i = 0; i < cut_points.size()-1; i++) {

                        if (turn == 0) {
                            for (int j = cut_points[i]; j < cut_points[i+1]; j++) {
                                child1.sequence.push_back(this->sequence[j]);
                                child2.sequence.push_back(other.sequence[j]);
                            }
                            turn = 1;
                        }
                        else {
                            for (int j = cut_points[i+1]-1; j > cut_points[i]-1; j--) {
                                child1.sequence.push_back(this->sequence[j]);
                                child2.sequence.push_back(other.sequence[j]);
                            }
                            turn = 0;
                        }

                    }

//                    child1.print();
//                    child2.print();
                    
                    double p = rand() / (double) RAND_MAX;

                    if (child1.is_ammissible()) {
                        if (p < 0.99)
                            children.push_back(child1);
                    }
                    else {
                        if (p > 0.99)
                            children.push_back(child1);
                    }

                    if (child2.is_ammissible()) {
                        if (p < 0.99)
                            children.push_back(child2);
                    }
                    else {
                        if (p > 0.99)
                            children.push_back(child2);
                    }
                } // children generation
            }

            mutate(children);
            improve(children);

            return children;
        }

        bool mutate(vector<Solution>& children);

        bool improve(vector<Solution>& children);

        // max_iterations == 0 => go ad libitum
        Solution& hill_climbing(int current_iteration, int max_iterations =0);

        /** calculates the hamming distance between two Solution
         * @param other_sol the other solution to compare
         */
        int hamming_distance(const Solution& other_sol) const;

        double repeats() const;

        bool is_ammissible() const;

        bool repair() {
            //TODO?
            return true;
        }
};

#endif /* SOLUTION_H */
