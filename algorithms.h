#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "game_logic.h"


//PRE : Moves must be populated
//POST: Will return the optimal gamestate in moves
//DESC: Iterative Deepening MiniMax
gameState IDMM(vector<gameState> moves, string color);

//PRE : none
//POST: Will return the best heuristic value for the state at depth
//DESC: Mini Max algorithm
int MiniMax(gameState state, int depth, string color, bool isMaxPlayer);

#endif