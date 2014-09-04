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
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class Solver
{
    public:
        int phase; // 0 for diversification, 1 for intensification
        int iterations;
        int iterations_without_improvement;
        int max_tot_iterations;
        int hc_int_iterations;
        int hc_div_iterations;
        int hc_iterations;
        double int_accept_prob;
        double div_accept_prob;
        double accept_prob;

        /** Constructor */
        Solver(int max_it, int wimp, double hii, double hdi, double iap, double dap): max_tot_iterations(max_it) {
            phase = 0;
            iterations = 0;
            iterations_without_improvement = 0;

            hc_int_iterations = hii;
            hc_div_iterations = hdi;
            int_accept_prob = iap;
            div_accept_prob = dap;

            accept_prob = div_accept_prob;
            hc_iterations = hc_div_iterations;
        }

        int set_phase(int ph) {
            phase = ph;
            if (phase == 0) {
                hc_iterations = hc_div_iterations;
                accept_prob = div_accept_prob;
            }
            else {
                hc_iterations = hc_div_iterations;
                accept_prob = div_accept_prob;
            }
            
            return phase;
        }

        /**
        * initialize a solution as a random sequence by random swaps
        * @param sol solution to be initialized
        */
        bool init_random_solution(Solution& sol, bool random_val =true) {
            if (random_val)
                for ( int i = 0; i < sol.instance->nodes_card; ++i ) {
                    int it = rand() % sol.instance->nodes_card;
                    sol.sequence.push_back(it);
                }
            else {
                for ( int i = 0; i < sol.instance->nodes_card; ++i )
                    sol.sequence.push_back(i);
                random_shuffle(sol.sequence.begin(), sol.sequence.end());
            }

//            sol.print();
            return true;
        }

        /**
        * initialize a solution as a random sequence by random swaps
        * @param sol solution to be initialized
        * @return true if everything OK, false otherwise
        */
        bool init_starting_population(Population &start_pop, vector<Solution> &start_solutions, const Instance& inst) {
            
            int i = 0; 
            while((int) start_pop.individuals.size() < start_pop.sol_card/2) {
                // lancia hill climbing a partire da random solution
                start_pop.individuals.push_back(start_solutions[i].hill_climbing(0, hc_iterations));
                i++;
            }

            while((int) start_pop.individuals.size() < start_pop.sol_card) {
                start_pop.individuals.push_back(start_solutions[i]);
                i++;
            }

//            cout << "### Initial Population ###" << endl;
//            start_pop.print();

            return true;
        }

        /**
        * Main method: search for a good tour by local search
        * @param TSP TSP data
        * @param initSol initial solution
        * @param bestSol best found solution (output)
        * @return true id everything OK, false otherwise
        */
        Solution solve(const Instance& tsp, Population& init_pop);

    protected:
        Population advance_generation(const Solution& current_opt, const Population& pop, const Instance& inst);

        bool build_new_population(const Solution& current_opt, const Population& current, vector<Solution>& children, const Instance& inst);
};

#endif /* SOLVER_H */

