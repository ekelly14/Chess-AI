// AI
// This is where you build your AI

#include "ai.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
#include "game_logic.h" 
// <<-- /Creer-Merge: includes -->>

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Chess C++ Player";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
    cout << "I am Player " << player->color << endl;
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
    //<<-- /Creer-Merge: ended -->>
}

/// <summary>
/// This is called every time it is this AI.player's turn to make a move.
/// </summary>
/// <returns>A string in Universal Chess Inferface (UCI) or Standard Algebraic Notation (SAN) formatting for the move you want to make. If the move is invalid or not properly formatted you will lose the game.</returns>
std::string AI::make_move()
{
    // <<-- Creer-Merge: makeMove -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.

    //Create an instance of the game
    gameState startState;

    //Determine if this is the first turn
    if(game->history.size() == 0 ){
      startState.isFirstMove = true;
    } else {
      startState.isFirstMove = false;
    }

    //Populate the gamestate datastructure using the FEN string
    string fen = game->fen;
    startState.populate_board(fen);

    //Print the board to the user before the move is made
    startState.print_board();

    //Generate all valid moves that can be made
    vector<string> moves;
    startState.get_valid_moves(moves, player->color, startState.gameBoard);

    srand(time(NULL));
    //Randomly choose one, then find moves that originate from the same piece
    int randIndex = (rand() % moves.size());
    string moveSubStr =  moves[randIndex].substr(0,2);
    for(int k = 0; k < moves.size(); k++){
      if(moves[k].substr(0,2) == moveSubStr){
        cout << moves[k] << " ";
      }
    }
    cout << endl;
    // <<-- /Creer-Merge: makeMove -->>
    //return std::string{};
    return moves[randIndex];
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional methods here for your AI to call
//<<-- /Creer-Merge: methods -->>

} // chess

} // cpp_client