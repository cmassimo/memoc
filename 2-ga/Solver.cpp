/**
 * @file Solver.cpp
 * @brief TSP solver (GA plus HC)
 *
 */

#include "Solver.h"
#include <iostream>

bool Solver::solve(const Instance& tsp, const Solution& initSol, Solution& bestSol)
{
    try
    {
        bool stop = false;
        int  iter = 0;

        TSPSolution currSol(initSol);
        double bestValue, currValue;
        bestValue = currValue = evaluate(currSol,tsp);
        TSPMove move;

        while ( ! stop ) {
            currSol.print();

            cout << " (" << ++iter << ") value " << currValue << " (" << evaluate(currSol,tsp) << ")";

            double bestNeighValue = currValue + findBestNeighbor(tsp,currSol,move);

            cout << " move: " << move.from << " , " << move.to << std::endl;

            if ( bestNeighValue < currValue ) {
                bestValue = currValue = bestNeighValue;
                currSol = swap(currSol,move);
                stop = false;
            } else {
                stop = true;
            }
        }

        bestSol = currSol;
    }
    catch(std::exception& e)
    {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return true;
}

TSPSolution& TSPSolver::swap ( TSPSolution& tspSol , const TSPMove& move ) 
{
  TSPSolution tmpSol(tspSol);
  for ( int i = move.from ; i <= move.to ; ++i ) {
    tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
  }
  return tspSol;
}


double TSPSolver::findBestNeighbor ( const TSP& tsp , const TSPSolution& currSol , TSPMove& move )
{
  double bestDecrement = tsp.infinite;

  /// REPLACE BY RIGHT CODE FROM HERE...
  move.from = move.to = 0;
  /// ...TO HERE
  return bestDecrement;
}
