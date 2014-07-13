/**
 * @file Solution.cpp
 * @brief TSP Solution
 *
 */

#include "Solution.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool comp_pair(pair<double, Solution> fst, pair<double, Solution> snd) { return fst.first < snd.first; }

double Solution::evaluate() const {
    double total = 0.0;
    for ( uint i = 0 ; i < this->sequence.size() - 1; ++i ) {
        int from = this->sequence[i];
        int to   = this->sequence[i+1];
        total += this->instance->costs[from][to];
    }

    total += this->instance->costs[this->sequence.size()-1][0];

    return total;
}

double Solution::fitness(const Solution& best) const {
    // the 10000.0 param is for scale pourpose
    double total = 1.0 / (evaluate()/10000.0);

    // penalizza sol inammissibile (cicli / cappi)
    total -= repeats() / 100.0;
    
    // penalizza soluzioni troppo simili all'ottimo corrente

    int hdist = (best.hamming_distance(*this));

    if (hdist > 0)
        total -= (1.0 / hdist);
    else
        total -= 0.03;

    return total;
}

bool Solution::mutate(vector<Solution>& children) {
    //srand(time(NULL));

    //26 April, 01:23 (UTC+3)
    for (uint i = 0 ; i < children.size(); i++) {
        Solution& mut = children[i];

        double p = (rand() / (double) RAND_MAX);

        if (p < 0.99) {
            Move m;
            m.from = rand() % mut.sequence.size();
            m.to = rand() % mut.sequence.size();
            mut = mut.swap(m);
        }
        else {
            if (mut.is_ammissible()) {
                int idx = rand() % mut.sequence.size();
                int val = rand() % mut.sequence.size();
                mut.sequence[idx] = val;
            }
            else {
                mut.repair();
            }
        }
    }

    return true;
}

bool Solution::improve(vector<Solution>& children) {
    //srand(time(NULL));

    //destroy erase improve
    double p = (rand() / (double) RAND_MAX);

    for (uint i = 0 ; i < children.size(); ++i) {
        if (p < 0.66 && children[i].is_ammissible())
            children[i] = children[i].hill_climbing(0, 0);
    }

    return true;
}

// max_iterations == 0 => go ad libitum
Solution& Solution::hill_climbing(int current_iteration, int max_iterations) {
    vector<Move> moves;

    // XXX generates moves (neighbour) DRAFT
    for (uint i = 0; i < this->sequence.size(); ++i) {
        Move m;
        if (i < this->sequence.size() - 1) {
            m.from = i;
            m.to = i+1;
            moves.push_back(m);
        }
    }

    // Hill climbing
    Solution& local_best = *this;
    double local_best_value = evaluate();

    vector<pair<double, Solution> > neighbourhood;

    // generates the neighbourhood
    for(vector<Move>::iterator mv = moves.begin(); mv != moves.end(); ++mv) {
        Solution tmp = local_best.swap(*mv);
        neighbourhood.push_back(make_pair(tmp.evaluate(), tmp));
    }

    pair<double, Solution> best_pair = *min_element(neighbourhood.begin(), neighbourhood.end(), comp_pair);

    if (best_pair.first < local_best_value) {
        local_best = best_pair.second;
        local_best_value = best_pair.first;

        if (max_iterations == 0 || (max_iterations > 0 && current_iteration < max_iterations))
            return hill_climbing(current_iteration+1, max_iterations);
        else
            return local_best;
    }
    else {
        return local_best;
    }
}

Solution& Solution::swap(const Move& move) 
{
    Solution tmpSol(*this);
    for ( int i = move.from ; i <= move.to ; ++i ) {
        this->sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
    }

    return *this;
}

/** calculates the hamming distance between two Solution
 * @param other_sol the other solution to compare
 */
int Solution::hamming_distance(const Solution& other_sol) const {
    int dist = 0;

    for (uint i = 0; i < this->sequence.size(); i++)
        if (this->sequence[i] != other_sol.sequence[i])
            dist++;

    return dist;
}

double Solution::repeats() const {
    vector<int> comp(this->sequence.size());
    copy(this->sequence.begin(), this->sequence.end(), comp.begin());
    sort(comp.begin(), comp.end());

    int reps = 0;
    int current = comp[0];
    for (uint i = 1; i < comp.size()-1; i++) {
        if (current == comp[i])
            reps++;
        else
            current = comp[i];
    }

    return reps;
}

bool Solution::is_ammissible() const {
    return this->repeats() == 0;
}
