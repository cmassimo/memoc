/**
 * @file Solver.cpp
 * @brief TSP solver (GA plus HC)
 *
 */

#include "Solver.h"
#include <iostream>
#include <algorithm>

using namespace std;

Solution Solver::solve(const Instance& tsp, Population& init_pop)
{
    //set_phase(1);
    bool stop = false;
    Population& training_population = init_pop;
    Solution best_sol = init_pop.best_solution();
    int max_no_imp = tsp.nodes_card*2;

    while (!stop && iterations < max_tot_iterations) {
        Population next_gen = advance_generation(best_sol, training_population, tsp);
//        cout << "### "<< iterations << "-th population ###" << endl;
//        next_gen.print();
        Solution local_best_sol = next_gen.best_solution();

//        cout << "best sol:\t\t";
//        best_sol.print();
//        cout << "current best sol:\t";
//        local_best_sol.print();

        if (local_best_sol.evaluate() < best_sol.evaluate()) {
            best_sol = local_best_sol;
            set_phase(1);
            iterations_without_improvement = 0;
        }
        else {
            iterations_without_improvement++;
        }
        iterations++;

        training_population = next_gen;


        if (iterations_without_improvement > max_no_imp)
            stop = true;

        if (iterations_without_improvement > max_no_imp-1)
            set_phase(0);
    }

//    cout << "### "<< iterations << "-th population ###" << endl;
    return best_sol;
}

// Operatore di selezione dei nuovi componenti della popolazione successiva
Population Solver::advance_generation(const Solution& current_opt, const Population& pop, const Instance& tsp) {
    srand(time(NULL));

    vector<Solution> next_generation;

    build_new_population(current_opt, pop, next_generation, tsp);

    //pick the next population from next_generation
    Population new_population(0);
    
    double total_fitness = 0.0;
    for (uint i = 0; i < next_generation.size(); i++) {
        total_fitness += next_generation[i].fitness(current_opt);
    }

    vector< pair<int, double> > index_fitness;

    // accumulo della fitness / index
    for (uint i = 0; i < next_generation.size(); i++) {
        double ftns = next_generation[i].fitness(current_opt);

        pair<int, double> tmp (i, ftns);
        index_fitness.push_back(tmp);
    }

    // selection
    while(new_population.individuals.size() < pop.individuals.size()) {

        double p = (rand() / (double) RAND_MAX) * total_fitness;

        uint i = 0;
        while(i < index_fitness.size() && p > 0) {
            p -= index_fitness[i].second;
            i++;
        }
        
        int selected_idx = index_fitness[i-1].first;
        Solution sol = next_generation[selected_idx];

        vector<Solution>::iterator dup = new_population.individuals.begin();
        while (dup != new_population.individuals.end()) {
            if (dup->hamming_distance(sol) == 0)
                break;
            else
                dup++;
        }

        // sol is not already present in the new popultion:
        if (dup == new_population.individuals.end())
            new_population.individuals.push_back(sol);

    }
//    cout << "---------------------- population advanced ----------------------" << endl;
    
    return new_population;
}

bool Solver::build_new_population(const Solution& current_opt, const Population& current, vector<Solution>& children, const Instance& inst) {
    vector< vector<Solution> > parents = current.select_parents(current_opt, inst);
//    cout << "---------------------- parents selected ----------------------" << endl;

    for (uint i = 0; i < parents.size(); i++) {
        vector<Solution> couple = parents[i];

        vector<Solution> twins = couple[0].crossover(couple[1], this->accept_prob, this->hc_iterations);
        for(uint j = 0; j < twins.size(); j++)
            children.push_back(twins[j]);
    }

    return true;
}

