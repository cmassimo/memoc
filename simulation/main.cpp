/**
 * @file main.cpp
 * @brief 
 */

#include <stdexcept>
#include <ctime>
#include <sys/time.h>

#include "Solver.h"
#include "Model.h"

using namespace std;

vector<double> simulation(int nodes_card, int x, int y, int start =0) {
    vector<double> data;

	try {
		Instance* instance = new Instance(nodes_card, x, y, start);
		
        /* CPLEX */

        Model model;
        vector<double> cplex_data;
        cplex_data = model.cplex_solve(instance);


        /* END CPLEX */

        /* GENETIC */

        vector<double> ga_data;

		clock_t t1, t2;
		t1 = clock();
		struct timeval  tv1, tv2;
        gettimeofday(&tv1, NULL);
    
        double params[] = {
            0.6, 0.9, 4, 0, 22000, 10000, 10,
            0.6, 0.9, 10, 0, 28000, 16000, 20
        };

        int i = 0;
        if (nodes_card >= 40)
            i = 7;

        Solver solver((int) params[i+4], (int) params[i+5], params[i+3], params[i+2], params[i+1], params[i+0]);
        vector<Solution> start_sols;

        int pop_size = params[i+6];

        for (int i = 0; i < pop_size/2; i++) {
            Solution sol(*instance);
            solver.init_random_solution(sol);
            start_sols.push_back(sol);
        }

        for (int i = 0; i < pop_size/2; i++) {
            Solution sol(*instance);
            solver.init_random_solution(sol, false);
            start_sols.push_back(sol);
        }

        Population start_pop(pop_size);
        solver.init_starting_population(start_pop, start_sols, *instance);
        Solution best_initial_solution = start_pop.best_solution();
    
        Solution best_solution = solver.solve(*instance, start_pop);
        
        t2 = clock();
        gettimeofday(&tv2, NULL);
        
        cout << "ITERATION: " << i << " - GENETIC ALGORITM" << endl;
        cout << "FROM init solution:\t"; 
        best_initial_solution.print();
        cout << "TO final solution:\t"; 
        best_solution.print();
        cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
        cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";

        double elapsed_time = (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6));
//        double initial_obj_val = best_initial_solution.evaluate();
        double final_obj_val = best_solution.evaluate();
        double generation = solver.iterations;
        ga_data.push_back(elapsed_time);
        //ga_data.push_back(initial_obj_val);
        ga_data.push_back(final_obj_val);
        ga_data.push_back(generation);

        /* END GENETIC */
		
        //data.push_back(instance->nodes_card);
        data.insert(data.end(), cplex_data.begin(), cplex_data.end());
        data.insert(data.end(), ga_data.begin(), ga_data.end());
	}
	catch(exception& e) {
		cout << ">>>EXCEPTION: " << e.what() << endl;
	}

    return data;
};

int main(int argc, char const *argv[])
{
    int sim_start = 10;
    int sim_end = 70;
    int step = 10;
    int rounds = 10;

    vector< vector<double> > rows;
    for (int round = 0; round < rounds; round++) {
        cout << "Round: " << round << endl;
        for (int i = sim_start; i < sim_end+1; i += step) {
            cout << endl << i << " nodes" << endl << endl;
            vector<double> row = simulation(i, 800, 800);
            //row.push_back(round);
            rows.push_back(row);
        }
    }

    int sim_steps = sim_end / step;
    // save data to CSV file
    ofstream ofs;
    ofs.open("E1.csv", ofstream::out | ofstream::app); 
    cout << "CPLEX_MEAN_TIME,CPLEX_MEAN_OBJ,GEN_MEAN_TIME,GEN_MEAN_OBJ,MEAN_POP,MATCH,EXCESS,STDD" << endl;

    // means
    for (int r = 0; r < sim_steps; r++) {
        vector<double> means;
        for (int i = 0; i < 5; i++) {
            double acc = 0;
            for(int j = 0+r; j < sim_steps*rounds; j += sim_steps) {
                acc += rows[j][i];
            }
            ofs << (acc / rounds);
            means.push_back(acc / rounds);
            ofs << ",";
        }

        // match
        int match = 0;
        for(int j = 0+r; j < sim_steps*rounds; j += sim_steps) {
            if ((int) floor(rows[j][1]) - (int) floor(rows[j][3]) == 0) {
                match++;
            }
        }

        ofs << match * 100.0 / rounds;
        ofs << ",";
        
        // excess
        ofs << ((means[3] - means[1]) / means[1]) * 100;
        ofs << ",";

        //cplex time stdd
        double time_sqerr = 0;
        for(int j = 0+r; j < sim_steps*rounds; j += sim_steps) {
            time_sqerr += pow(rows[j][2] - means[2], 2);
        }
        ofs << sqrt(time_sqerr / rounds);

        ofs << ",";

        //meta obj stdd
        double obj_sqerr = 0;
        for(int j = 0+r; j < sim_steps*rounds; j += sim_steps) {
            obj_sqerr += pow(rows[j][3] - means[3], 2);
        }
        ofs << sqrt(obj_sqerr / rounds);

        ofs << "\n";
    }

    ofs.close();

    return 0;
};

