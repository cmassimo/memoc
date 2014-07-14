#ifndef POPULATION_H
#define POPULATION_H

#include "Solution.h"

#include <vector>
#include <climits>

using namespace std;

/**
* TSP Solution pool representation: list of Solutions
*/
class Population
{
    public:
        int sol_card;
        vector<Solution> individuals;

        /** Constructor 
        * build a standard solution as the sequence <0, 1, 2, 3 ... n-1, 0>
        * @param tsp TSP instance
        * @return ---
        */
        Population(int n) : sol_card(n) {
            individuals.reserve(sol_card);
        }

        /** Copy constructor 
        * build a solution from another
        * @param tspSol TSP solution
        * @return ---
        */
        Population( const Population& pop ) {
            individuals.reserve(pop.individuals.size());
            for (uint i = 0; i < pop.individuals.size(); ++i)
                individuals.push_back(pop.individuals[i]);
        }

        /** print method 
        * @param ---
        * @return ---
        */
        void print ( void ) {
            for(vector<Solution>::iterator it = individuals.begin(); it != individuals.end(); ++it)
                it->print();
        }

        /** assignment method 
        * copy a solution into another one
        * @param right TSP solution to get into
        * @return ---
        */
        Population& operator=(const Population& right) {
            // Handle self-assignment:
            if(this == &right) return *this;

            for ( uint i = 0; i < individuals.size(); i++ )
                individuals[i] = right.individuals[i];

            return *this;
        }

        /* XXX calcolare fitness una volta  sola
         * Operatore di selezione dei parent per la ricombinazione
         *
         */
        vector< vector<Solution> > select_parents(const Solution& best_sol, const Instance& inst) const {
            vector< vector<Solution> > parents;

            double total_fitness = 0.0;
            for (uint i = 0; i < individuals.size(); i++) {
                total_fitness += individuals[i].fitness(best_sol);
            }

            vector< pair<int, double> > index_fitness;

            // accumulo della fitness / index
            for (uint i = 0; i < individuals.size(); i++) {
                double ftns = individuals[i].fitness(best_sol);

                pair<int, double> tmp (i, ftns);
                index_fitness.push_back(tmp);
            }

            // selection
            while((int) parents.size() < sol_card) {
                vector<Solution> couple;

                while (couple.size() < 2) {
                    double p = (rand() / (double) RAND_MAX) * total_fitness;

                    uint i = 0;
                    while(i < index_fitness.size() && p > 0) {
                        p -= index_fitness[i].second;
                        i++;
                    }

                    int selected_idx = index_fitness[i-1].first;
                    Solution sol = individuals[selected_idx];

                    // hamming_distance > 0 => the two solution are not the same
                    if (couple.size() == 0 || couple[0].hamming_distance(sol) > 0)
                        couple.push_back(sol);
                }

                parents.push_back(couple);
            }

            return parents;
        }

        Solution best_solution() const {
            double min = INT_MAX;
            int min_index = 0;

            for (uint i = 0; i < individuals.size(); i++) {
                double tmp_objval = individuals[i].evaluate();
                if (individuals[i].is_ammissible() && tmp_objval < min) {
                    min = tmp_objval;
                    min_index = i;
                }
            }

            return individuals[min_index];

        }
};

#endif /* POPULATION_H */
