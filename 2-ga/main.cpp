/**
 * @file main.cpp
 * @brief 
 */

#include <stdexcept>
#include <ctime>
#include <sys/time.h>

#include "Solver.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[255];

int main (int argc, char const *argv[])
{
	try
	{
		Instance* instance = new Instance(40, 500, 500, 0);
		
		clock_t t1, t2;
		t1 = clock();
		struct timeval  tv1, tv2;
        gettimeofday(&tv1, NULL);
    
        Solver solver(instance->nodes_card*1000);
        vector<Solution> start_sols;

        for (int i = 0; i < 10; i++) {
            Solution sol(*instance);
            solver.init_random_solution(sol);
            start_sols.push_back(sol);
        }

        Population start_pop(10);
		solver.init_starting_population(start_pop, start_sols, *instance);
	
		Solution best_solution = solver.solve(*instance, start_pop);
		
		t2 = clock();
		gettimeofday(&tv2, NULL);
		
        Solution& solution = *start_sols.begin();
		cout << "FROM solution:\t"; 
        solution.print();
		cout << "TO solution:\t"; 
		best_solution.print();
        cout << endl;
		cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
		cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";
		
	}
	catch(exception& e)
	{
		cout << ">>>EXCEPTION: " << e.what() << endl;
	}

	return 0;
}
