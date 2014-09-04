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

    total += this->instance->costs[this->sequence[this->sequence.size()-1]][this->sequence[0]];

    return total;
}

double Solution::fitness(const Solution& best) const {
    // the 1/10000.0 is here for scale pourpose
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

bool Solution::mutate(vector<Solution>& children, const double accept_prob) {

    //26 April, 01:23 (UTC+3)
    for (uint i = 0 ; i < children.size(); i++) {
        Solution& mut = children[i];

        double p = (rand() / (double) RAND_MAX);

        if (p < accept_prob) {
            // mutazione per inversione
            int from = rand() % mut.sequence.size();
            int to = rand() % mut.sequence.size();
            if (from < to)
                reverse(mut.sequence.begin()+from, mut.sequence.begin()+to);
            else
                reverse(mut.sequence.begin()+to, mut.sequence.begin()+from);
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

bool Solution::improve(vector<Solution>& children, const double accept_prob, const int hc_iterations) {

    //destroy erase improve
    double p = (rand() / (double) RAND_MAX);

    for (uint i = 0 ; i < children.size(); ++i) {
        if (p < accept_prob && children[i].is_ammissible())
            children[i] = children[i].hill_climbing(0, hc_iterations);
    }

    return true;
}

// max_iterations == 0 => go ad libitum
Solution& Solution::hill_climbing(int current_iteration, int max_iterations) {
    // Hill climbing
    Solution& local_best = *this;
    double local_best_value = evaluate();

    vector<pair<double, Solution> > neighbourhood;
    uint max_neighbourhood_size = (this->sequence.size()*3);

    // again casual 2-opt
    while(neighbourhood.size() < max_neighbourhood_size) {
        int from = rand() % local_best.sequence.size();
        int to = rand() % local_best.sequence.size();

        Solution tmp = local_best;

        if (!(abs(from-to) <= 1 || abs(from-to) == 49)) {
            if (from < to)
                reverse(tmp.sequence.begin()+from, tmp.sequence.begin()+to);
            else
                reverse(tmp.sequence.begin()+to, tmp.sequence.begin()+from);

            neighbourhood.push_back(make_pair(tmp.evaluate(), tmp));
        }

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
    for (uint i = 1; i < comp.size(); i++) {
        if (current == comp[i])
            reps++;
        else
            current = comp[i];
    }

    return reps;
}

bool Solution::repair() {
    vector<int> reps;
    vector<int> tpl;
    for (uint i = 0; i < this->sequence.size(); i++)
        tpl.push_back(i);

    for(uint i = 0; i < this->sequence.size()-1; i++) {
        int current = this->sequence[i];
        for(uint j = i+1; j < this->sequence.size(); j++) {
            if (current == this->sequence[j])
                reps.push_back(j);
        }
    }

    for (uint i = 0; i < reps.size(); i++) {
        for (uint j = 0; j < tpl.size(); j++) {
            if (find(this->sequence.begin(), this->sequence.end(), tpl[j]) == this->sequence.end()) {
                this->sequence[reps[i]] = tpl[j];
                break;
            }
        }
    }

    return true;
}

bool Solution::is_ammissible() const {
    return this->repeats() == 0;
}
