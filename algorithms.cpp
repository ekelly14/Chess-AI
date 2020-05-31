#include "algorithms.h"

//PRE : Moves must be populated
//POST: Will return the optimal gamestate in moves
//DESC: Iterative Deepening MiniMax; time_remaining is in ns
gameState IDMM(vector<gameState> moves, string color, double time_remaining){
  gameState bestState;  //space for the optimal state
  vector<gameState> sameValueStates;
  int bestValue = -999; //Initialize the best heuristic value to negitive infinity
  const double ten_min = 600000000000;  //Ten minutes in nanoseconds
  const double five_min = 300000000000; //Five minutes in nanoseconds

  double max_turn_time;  //A variable denoting the maximum allotted time for a turn to be made
  if(time_remaining >= ten_min){
    //If there are more than ten minutes remaining, allow moves to take up to 1/20 of the time remaining
    max_turn_time = time_remaining/20;
  } else if(time_remaining < ten_min && time_remaining >= five_min){
    //If there are between 5 and 10 minutes remaining, allow moves to take up to 1/10th of the time remaining
    max_turn_time = time_remaining/10;
  } else if(time_remaining < five_min){
    //If there are less than 5 minutes remaining, allow moves to take up to one 1/5th of the time remaining
    max_turn_time = time_remaining/5;
  }

  //For each state in the valid moves vector
  for(int i = 0; i < moves.size(); i++){
    //Iterativly call MiniMax, starting at 0 and going to max depth
    for(int k = 0; k <= 5; k++){
      //Get the time to compare moves against
      chrono::time_point<chrono::high_resolution_clock> start_time = chrono::high_resolution_clock::now();
      //Evaluate Minimax with current max  depth
      int value = MiniMax(moves[i], k, color, true, -999, 999, max_turn_time, start_time);
      //If the value is better, make  this state the best state
      if(value > bestValue){
        // bestState = moves[i];
        sameValueStates.clear();
        sameValueStates.push_back(moves[i]);
      } else if(value == bestValue){
        sameValueStates.push_back(moves[i]);
      }
    }
  }
  // return bestState;
  return sameValueStates[rand() % sameValueStates.size()];
}

//PRE : none
//POST: Will return the best heuristic value for the state at depth
//DESC: Mini Max algorithm; time mesured in ns
int MiniMax(gameState state, int depth, string color, bool isMaxPlayer, int alpha, int beta, double time_remaining, std::chrono::time_point<std::chrono::high_resolution_clock> start_time){
  if(time_remaining == 0){
    return 0;
  }
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
    vector<gameState> sameValueStates;

    //Get and return the path with the highest value
    int value = -999;
    for(int i = 0; i < children.size(); i++){
      //Mark the current time
      chrono::time_point<chrono::high_resolution_clock> mark_time = chrono::high_resolution_clock::now();
      //Evaluate the current amound of time elapsed for the move
      double time_elapsed = chrono::duration_cast<chrono::nanoseconds>(mark_time - start_time).count();
      //Evaluate the minimax value of the children of this gamestate
      value = max(value, MiniMax(children[i], depth-1, color, false, alpha, beta, time_elapsed, start_time));
      //If out of time, return
      if(time_remaining == 0){
        return 0;
      }

      //Set alpha to the current value if it is larger than the current alpha
      alpha = max(alpha, value);
      if(alpha >= beta){
        // Beta should be cut off
        break;
      }
    }
    return value;
  //Else, the current turn is of the minimizing player
  } else {
    //Get the valid moves that the other player could make
    vector<gameState> children;
    state.get_valid_moves(children, color, state.gameBoard);
    vector<gameState> sameValueStates;

    //Get and return the most likely path the other player could make
    int value = 999;
    for(int i = 0; i < children.size(); i++){
      //Mark the current time
      chrono::time_point<chrono::high_resolution_clock> mark_time = chrono::high_resolution_clock::now();
      //Evaluate the current amount of time elapsed for the move
      double time_elapsed = chrono::duration_cast<chrono::nanoseconds>(mark_time - start_time).count();
      //Evaluate the minimax value of the children of this gamestate
      value = min(value, MiniMax(children[i], depth-1, color, true, alpha, beta, time_elapsed, start_time));
      //If out of time, return
      if(time_remaining == 0){
        return 0;
      }

      beta = min(beta, value);
      if(alpha >= beta){
        // alpha should be cut off
        break;
      }
    }
    return value;
  }
}