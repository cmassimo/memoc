/**
 * @file Solver.cpp
 * @brief TSP solver (GA plus HC)
 *
 */

#include "Solver.h"
#include <iostream>
#include <algorithm>

using namespace std;

bool comp_pair(pair<double, Solution> fst, pair<double, Solution> snd) { return fst.first < snd.first; }

bool Solver::solve(const Instance& tsp, const Population& init_pop, Solution& bestSol)
{
    // TODO
    try
    {
        bool stop = false;
        int  iter = 0;

    }
    catch(exception& e)
    {
        cout << ">>>EXCEPTION: " << e.what() << endl;
        return false;
    }

    return true;
}

//Solution& Solver::swap(Solution& tspSol, const Move& move) 
//{
//  Solution tmpSol(tspSol);
//  for ( int i = move.from ; i <= move.to ; ++i ) {
//    tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
//  }
//  return tspSol;
//}

// Operatore di selezione dei nuovi componenti della popolazione successiva
bool advance_generation(Population& pop, Instance& inst) {
    // TODO
}

Solution& Solver::hill_climbing_tsp(const Instance& inst, Solution& start, double starting_value) {
    vector<Move> moves;

    // XXX generates moves (neighbour) DRAFT
    for (uint i = 0; i < start.sequence.size(); ++i) {
        Move m;
        if (i < start.sequence.size() - 1) {
            m.from = i;
            m.to = i+1;
            moves.push_back(m);
        }
    }

    // Hill climbing
    Solution& local_best = start;
    double local_best_value = evaluate(start, inst);

    vector<pair<double, Solution> > neighbourhood;

    // generates the neighbourhood
    for(vector<Move>::iterator it = moves.begin(); it != moves.end(); ++it) {
        Solution tmp = swap(start, *it);
        neighbourhood.push_back(make_pair(evaluate(tmp, inst), tmp));
    }

    pair<double, Solution> best_pair = *max_element(neighbourhood.begin(), neighbourhood.end(), comp_pair);

    if (best_pair.first > local_best_value) {
        local_best = best_pair.second;
        local_best_value = best_pair.first;
        return hill_climbing_tsp(inst, local_best, local_best_value);
    }
    else {
        return local_best;
    }
}

bool Solver::build_new_population(Population& current, vector<Solution>& children, const Instance& inst) {
    //TODO
}
