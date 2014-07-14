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
    
        Solver solver(instance->nodes_card*5);
        vector<Solution> start_sols;

        for (int i = 0; i < 10; i++) {
            Solution sol(*instance);
            solver.init_random_solution(sol);
            start_sols.push_back(sol);
        }

        Population start_pop(10);
		solver.init_starting_population(start_pop, start_sols, *instance);
        Solution best_initial_solution = start_pop.best_solution();
	
		Solution best_solution = solver.solve(*instance, start_pop);
		
		t2 = clock();
		gettimeofday(&tv2, NULL);
		
        cout << "GENETIC ALGORITM" << endl;
		cout << "FROM init solution:\t"; 
        best_initial_solution.print();
		cout << "TO final solution:\t"; 
		best_solution.print();
		cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
		cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";

        double elapsed_time = (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6));
        double initial_obj_val = best_initial_solution.evaluate();
        double final_obj_val = best_solution.evaluate();
        double generation = solver.iterations;
        ga_data.push_back(elapsed_time);
        ga_data.push_back(initial_obj_val);
        ga_data.push_back(final_obj_val);
        ga_data.push_back(generation);

        /* END GENETIC */
		
        data.push_back(instance->nodes_card);
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
    int rounds = 1;

    vector< vector<double> > rows;
    for (int round = 0; round < rounds; round++) {
        cout << "Round: " << round << endl;
        for (int i = sim_start; i < sim_end+1; i += step) {
            cout << endl << i << " nodes" << endl << endl;
            vector<double> row = simulation(i, 500, 500);
            row.push_back(round);
            rows.push_back(row);
        }
    }

    // save data to CSV file
    ofstream ofs;
    ofs.open("data.csv", ofstream::out | ofstream::app);

    ofs << "nodes,ctime,cobj,gtime,gobj_ini,gobj_fin,gen,round\n";

    for(vector< vector<double> >::iterator r = rows.begin(); r != rows.end(); r++) {
        for(vector<double>::iterator d = r->begin(); d != r->end(); d++) {
            ofs << *d;
            if (d != r->end()-1)
                ofs << ",";
        }

        ofs << "\n";
    }

    ofs.close();

    return 0;
};

