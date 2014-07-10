#ifndef POPULATION_H
#define POPULATION_H

#include "Solution.h"

#include <vector>

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

        // Operatore di selezione dei parent per la ricombinazione
        vector< pair<Solution, Solution> >& select_parents() {
            //TODO
            vector< pair<Solution, Solution> > parents;
            return parents;
        }
};

#endif /* POPULATION_H */
