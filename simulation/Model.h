/**
 * @file model.cpp
 * @brief STSP model and main for simulation
 */

#include <cstdio>
#include <iostream>
#include <vector>
#include "cpxmacro.h"
#include "Instance.h"

#include <ctime>
#include <sys/time.h>

using namespace std;

int status;
char errmsg[BUF_SIZE];

const int NAME_SIZE = 512;
char name[NAME_SIZE];

int get_var_index(int nodes_card, int c, int r) {
	const int n = nodes_card;
	if (c == r) return -1;

	int idx = 0;

	if (c > r)
		idx = c*(n-1) + r;
	else
		idx = c*(n-1) + (r-1);

	return idx;
}
	
class Model {
    public:
        Model() {}

        void setupLP(Instance* inst, CEnv env, Prob lp, int & numVars) {
            const int n = inst->nodes_card;
            const int start_idx = inst->start_index;

            // add x_i_j vars
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {

                    if (i != j) {
                        char xtype = 'I';
                        double obj = 0;
                        double lb = 0;
                        double ub = n;
                        snprintf(name, NAME_SIZE, "x_%i_%i", i, j);
                        char* xname = (char*)(&name[0]);
                        CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname );
                    }

                }
            }

            const int yIdx = CPXgetnumcols(env, lp);

            // add y vars
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        char ytype = 'B';
                        double obj = inst->costs[i][j];
                        double lb = 0;
                        double ub = 1;
                        snprintf(name, NAME_SIZE, "y_%i_%i", i, j);
                        char* yname = (char*)(&name[0]);
                        CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
                    }
                }
            }

            numVars = CPXgetnumcols(env, lp);

            // add x_0_j constraints ( SUM(x_0_j) = |N| )
            std::vector<int> idx(n-1);
            for (int i = 0; i < n-1; i++) {
                idx[i] = start_idx * n + i;
            }
            std::vector<double> coef(n-1, 1.0);
            char sense = 'E';
            int matbeg = 0;
            const double rht = (double) n;
            CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rht, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );

            // add 1-unit-flux-deposit constraint ( SUM(x_i_k) - SUM(x_k_j) = 1 ) foreach k in N \ {N[start_idx]}
            // int row_size = 2 * (n - 1);

            for (int k = 0; k < n; k++) {
                // cout << "k: " << k << endl;
                if (k != start_idx) {

                    std::vector<int> idx;
                    std::vector<double> coef;

                    for (int i = 0; i < n; i++) {
                        if (i == k) {
                            for (int j = 0; j < n; j++) {
                                // x_k_j foreach (k,j) in A
                                // cout << "neg indexes: " << k << "," << j << endl;
                                int var_index = get_var_index(inst->nodes_card, k, j);
                                // cout << "found index: " << var_index << endl;
                                if (var_index >= 0) {
                                    idx.push_back(var_index);
                                    coef.push_back(-1.0);
                                }
                            }
                        }
                        else {
                            // x_i_k foreach (i,k) in A
                            // cout << "pos indexes: " << i << "," << k << endl;
                            int var_index = get_var_index(inst->nodes_card, i, k);
                            // cout << "found index: " << var_index << endl;
                            if (var_index >= 0) {
                                idx.push_back(var_index);
                                coef.push_back(1.0);
                            }
                        }
                    }

                    char sense = 'E';
                    int matbeg = 0;
                    const double rht = 1.0;
                    CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rht, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
                }
            }

            // add 1-arco-uscente-con-flusso constraint SUM(y_i_j) = 1 foreach j in N

            for (int i = 0; i < n; i++) {
                std::vector<int> idx(n-1);
                for (int j = 0; j < n-1; j++) {
                    idx[j] = yIdx + (i*(n-1) + j);
                }
                std::vector<double> coef(n-1, 1.0);
                char sense = 'E';
                int matbeg = 0;
                const double rht = 1.0;
                CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rht, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
            }

            // add 1-arco-entrante-con-flusso constraint SUM(y_i_j) = 1 foreach j in N

            for (int j = 0; j < n; j++) {

                std::vector<int> idx;

                for (int i = 0; i < n; i++) {
                    // cout << "indexes: " << i << "," << j << endl;
                    int var_index = get_var_index(inst->nodes_card, i, j);
                    // cout << "found index: " << yIdx + var_index << endl;

                    if (var_index >= 0) {
                        idx.push_back(yIdx + var_index);
                    }
                    
                }

                std::vector<double> coef(n-1, 1.0);

                char sense = 'E';
                int matbeg = 0;
                const double rht = 1.0;
                CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rht, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
            }

            // x_i_j <= |N|y_i_j foreach i,j in A
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n-1; j++) {
                        std::vector<int> idx(2);
                        idx[0] = i*(n-1)+j;
                        idx[1] = yIdx + (i*(n-1)+j);
                        std::vector<double> coef(2);
                        coef[0] = 1.0;
                        coef[1] = (float) -n;

                        char sense = 'L';
                        int matbeg = 0;
                        const double rht = 0.0;
                        CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &rht, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
                }
            }

            //CHECKED_CPX_CALL( CPXwriteprob, env, lp, "stsp.lp", 0 );
        }

        vector<double> cplex_solve(Instance* instance) {
            vector<double> data;

            try {
                // init
                DECL_ENV( env );
                DECL_PROB( env, lp );

                clock_t t1,t2;
                t1 = clock();
                struct timeval  tv1, tv2;
                gettimeofday(&tv1, NULL);

                // setup LP
                int numVars;
                setupLP(instance, env, lp, numVars);

                // optimize
                CHECKED_CPX_CALL( CPXmipopt, env, lp );

                t2 = clock();
                gettimeofday(&tv2, NULL); 

                double objval;
                CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
                double elapsed_time = (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6));
                data.push_back(elapsed_time);
                data.push_back(objval);

//                cout << "Source: " << instance->start_index << endl;
                cout << "CPLEX objval: " << objval << std::endl;
                cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)\n";
                cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)\n";
                cout << endl;

//                int n = CPXgetnumcols(env, lp);
//                cout << "check num var: " << n << " == " << numVars << endl;
//                if (n != numVars) { throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + "different number of variables"); }
//        		  CHECKED_CPX_CALL( CPXsolwrite, env, lp, "stsp.sol" );

                // free
                CPXfreeprob(env, &lp);
                CPXcloseCPLEX(&env);

            }
            catch(exception& e) {
                cout << ">>> EXCEPTION <<<: " << e.what() << std::endl;
            }
            return data;
        }
};
