#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "game_logic.h"
#include <chrono>

using Ns = std::chrono::nanoseconds;


//PRE : Moves must be populated
//POST: Will return the optimal gamestate in moves
//DESC: Iterative Deepening MiniMax; time remaining is in ns
gameState IDMM(vector<gameState> moves, string color, double time_remaining);

//PRE : none
//POST: Will return the best heuristic value for the state at depth
//DESC: Mini Max algorithm; time remaining is mesured in ns
int MiniMax(gameState state, int depth, string color, bool isMaxPlayer, int alpha, int beta,  
            double time_remaining, std::chrono::time_point<std::chrono::high_resolution_clock> start_time);

#endif