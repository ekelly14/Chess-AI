#include "algorithms.h"

//PRE : Moves must be populated
//POST: Will return the optimal gamestate in moves
//DESC: Iterative Deepening MiniMax
gameState IDMM(vector<gameState> moves, string color){
  gameState bestState;  //space for the optimal state
  int bestValue = -999; //Initialize the best heuristic value to negitive infinity

  //For each state in the valid moves vector
  for(int i = 0; i < moves.size(); i++){
    //Iterativly call MiniMax, starting at 0 and going to max depth
    //for(int k = 0; k <= 3; k++){
      //Evaluate Minimax with current max  depth
      int value = MiniMax(moves[i], 3, color, true);
      //If the value is better, make  this state the best state
      if(value > bestValue){
        bestState = moves[i];
      }
    //}
  }
  return bestState;
}

//PRE : none
//POST: Will return the best heuristic value for the state at depth
//DESC: Mini Max algorithm
int MiniMax(gameState state, int depth, string color, bool isMaxPlayer){
  //If this iteration is not about the Max Player (you),
  //Switch the colors so that the predicted calculations are accurate
  if(!isMaxPlayer){
    if(color == "white"){
      color = "black";
    }
    else if(color == "black"){
      color = "white";
    }
  }

  //If the depth is reached, or the board is a win state
  if(depth == 0 || state.is_checkmate(color)){
    //return the state's heuristic value
    state.get_heuristic(color);
    return state.hx;
  }

  //If the current turn is of the max player
  if(isMaxPlayer){
    //Get all valid moves the could come from this state
    vector<gameState> children;
    state.get_valid_moves(children, color, state.gameBoard);
    
    //Get and return the path with the highest value
    int value = -999;
    for(int i = 0; i < children.size(); i++){
      value = max(value, MiniMax(children[i], depth-1, color, false));
    }
    return value;
  //Else, the current turn is of the minimizing player
  } else {
    //Get the valid moves that the other player could make
    vector<gameState> children;
    state.get_valid_moves(children, color, state.gameBoard);

    //Get and return the most likely path the other player could make
    int value = 999;
    for(int i = 0; i < children.size(); i++){
      value = min(value, MiniMax(children[i], depth-1, color, true));
    }
    return value;
  }
}