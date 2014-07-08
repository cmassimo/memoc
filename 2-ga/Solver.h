/**
 * @file Solver.h
 * @brief TSP solver (GA plus HC)
 *
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "Population.h"

#include <vector>

using namespace std;

/**
 * Class representing substring reversal move
 */
typedef struct move {
    int	from;
    int	to;
} TSPMove;

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class Solver
{

    public:
        /** Constructor */
        Solver() {}

        /**
        * evaluate a solution
        * @param solution solution to be evaluated
        * @param TSP TSP data
        * @return the value of the sequence
        */
        double evaluate ( const Solution& sol , const Instance& tsp ) const {
            double total = 0.0;
            for ( uint i = 0 ; i < sol.sequence.size() - 1 ; ++i ) {
                int from = sol.sequence[i]  ;
                int to   = sol.sequence[i+1];
                total += tsp.cost[from][to];
            }

            return total;
        }

        /**
        * initialize a solution as a random sequence by random swaps
        * @param sol solution to be initialized
        * @return true if everything OK, false otherwise
        */
        bool initRnd ( Solution& sol ) {
            srand(time(NULL));

            for ( uint i = 1 ; i < sol.sequence.size() ; ++i ) {
                int idx1 = rand() % (sol.sequence.size()-2) + 1;
                int idx2 = rand() % (sol.sequence.size()-2) + 1;
                int tmp = sol.sequence[idx1];
                sol.sequence[idx1] = sol.sequence[idx2];
                sol.sequence[idx2] = tmp;
            }

            cout << "### ";
            sol.print();
            cout << " ###" << endl;
            return true;
        }

        /**
        * initialize a solution as a random sequence by random swaps
        * @param sol solution to be initialized
        * @return true if everything OK, false otherwise
        */
        bool init_population(Population &start_pop, vector<Solution> &start_solutions) {
            
            for ( uint i = 1 ; i < pop.n/2 ; ++i ) {
                // lancia hill climbing a partire da initRnd
                pop.individuals[i] = hill_climbing_tsp(start_solutions[i]);
            }

            cout << "### Initial Population ###" << endl;
            pop.print();
            cout << "###" << endl;

            return true;
        }


        /**
        * Main method: search for a good tour by local search
        * @param TSP TSP data
        * @param initSol initial solution
        * @param bestSol best found solution (output)
        * @return true id everything OK, false otherwise
        */
        bool solve(const Instance& tsp, const Solution& initSol, Solution& bestSol);

    protected:
        Solution& swap(Solution& tspSol, const TSPMove& move);

        Solution& hill_climbing_tsp(Solution &start) {
            // TODO
            new Solution(start.instance);
        }

};

#endif /* TSPSOLVER_H */
