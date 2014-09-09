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
         * Implements order-crossover strategy
         * Genera le due soluzioni figlie con cut-point crossover.
         * accetta soluzioni ammissibili con prob accept_prob e inamissibili
         * con prob complementare.
         */
        vector<Solution> crossover(const Solution& other, const double accept_prob, const int hc_iterations) {
            vector<Solution> children;

            if (*this == other) {
                double p = rand() / (double) RAND_MAX;

                if (this->is_ammissible()) {
                    if (p < accept_prob)
                        children.push_back(*this);
                }
                else {
                    if (p > accept_prob)
                        children.push_back(*this);
                }
            }
            else {
//                uint points_card = (int) ((rand() % 7) + 1);
//                if (points_card < 3)
//                    points_card = 3;

                vector<int> cut_points(2);
                int idx = (int) rand() % this->sequence.size();

                cut_points.push_back(idx);
                while (cut_points[0] == idx)
                    idx = (int) rand() % this->sequence.size();
                cut_points.push_back(idx);

                sort(cut_points.begin(), cut_points.end());

                while (children.size() < 2) {

                    Solution child1 = Solution(*this->instance);
                    Solution child2 = Solution(*this->instance);

                    for (int i = 0; i < cut_points[0]; i++) {
                        child1.sequence.push_back(this->sequence[i]);
                        child2.sequence.push_back(other.sequence[i]);
                    }
                    for (int i = cut_points[0]; i < cut_points[1]; i++) {
                        child1.sequence.push_back(-1);
                        child2.sequence.push_back(-1);
                    }
                    for (uint i = cut_points[1]; i < this->sequence.size(); i++) {
                        child1.sequence.push_back(this->sequence[i]);
                        child2.sequence.push_back(other.sequence[i]);
                    }

                    for (int i = cut_points[0]; i < cut_points[1]; i++) {
                        for (uint j = 0; j < this->sequence.size(); j++) {
                            if (find(child1.sequence.begin(), child1.sequence.end(), other.sequence[j]) == child1.sequence.end()) {
                                child1.sequence[i] = other.sequence[j];
                                break;
                            }
                        }
                        for (uint j = 0; j < this->sequence.size(); j++) {
                            if (find(child2.sequence.begin(), child2.sequence.end(), this->sequence[j]) == child2.sequence.end()) {
                                child2.sequence[i] = this->sequence[j];
                                break;
                            }
                        }
                    }

//                    child1.print();
//                    child2.print();
                    
                    double p = rand() / (double) RAND_MAX;

                    if (child1.is_ammissible()) {
                        if (p < accept_prob)
                            children.push_back(child1);
                    }
                    else {
                        if (p > accept_prob)
                            children.push_back(child1);
                    }

                    if (child2.is_ammissible()) {
                        if (p < accept_prob)
                            children.push_back(child2);
                    }
                    else {
                        if (p > accept_prob)
                            children.push_back(child2);
                    }
                } // children generation
            }

            mutate(children, accept_prob);
            improve(children, accept_prob, hc_iterations);

            return children;
        }

        bool mutate(vector<Solution>& children, const double accept_prob);

        bool improve(vector<Solution>& children, const double accept_prob, const int hc_iterations);

        // max_iterations == 0 => go ad libitum
        Solution& hill_climbing(int current_iteration, int max_iterations =0);

        /** calculates the hamming distance between two Solution
         * @param other_sol the other solution to compare
         */
        int hamming_distance(const Solution& other_sol) const;

        double repeats() const;

        bool is_ammissible() const;

        bool repair();
};

#endif /* SOLUTION_H */
