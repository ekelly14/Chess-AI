#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//PRE : gameBoard must be filled in with letters or dashes '-'
//POST: will return a boolean TRUE if the space is a valid move, FALSE if not
//DESC: Checks to see if a space can be moved to. It must be either a blank space or an enemy piece
bool check_space(char gameBoard[][8], string color, int x, int y, bool & pieceReplaced);

//PRE : startx, starty, x, y must be between 0 and 7 (valid board space)
//POST: returns a string for a move in UCI notation
//DESC: returns a string for a move in UCI notation, from start(x,y) to (x,y)
string move_string(int startx, int starty, int x, int y);

//PRE : None
//POST: TRUE if an enemy, FALSE if same team or '-'
//DESC: Returns a bool if the given piece is an enemy of the given color
bool isEnemyPiece(char piece, string color);

//PRE : Gameboard must be filled in with pieces or '-'
//POST: return TRUE if king is in check, FALSE if king is not in check
//DESC: Takes an (x,y) of the king position, returns whether or not the king is in check.
bool isKingCheck(char gameBoard[][8], string color, int x, int y);

//PRE : gameBoard must be a valid chess board, x,y must be valid chess board positions
//POST: copyBoard will be filled in with the peice already moved
//DESC: Copies the gameBoard into a seperate copyBoard array. Will automatically move piece "character" from (startx,starty)
//      to position (x,y)
void copyBoard(char gameBoard[][8], char copyBoard[][8], char character, int startx, int starty, int x, int y);

//PRE : Board must be populated correctly
//POST: King's x,y will be returned to the  kingx,kingy
//DESC: Find the king on the board
void getKingPos(char gameBoard[][8], string color, int & kingx, int & kingy);

class gameState{
  public:
    char gameBoard[8][8]; //Representation of the game board
    bool isFirstMove;     //TRUE if is the first move, FALSE if else
    string castling;      //The string in the castling position of the fen string
    string en_passant;    //The string in the en passant positon of the fen string

    //PRE : FEN string must be in fen notation
    //POST: gameState's board will be populated
    //DESC: populate the gameBoard to the position of the fen string
    void populate_board(const string fen);

    //PRE : None
    //POST: Board is printed to the console
    //DESC: Board is printed to the console
    void print_board();

    //PRE : Board must be populated correctly, color must be "black" or "white"
    //POST: Populate valid_moves with all the valid moves of the color's board pieces
    //DESC: Finds all the color's valid possible moves
    void get_valid_moves(vector<string> & valid_moves, string color, char gameBoard[][8]);

  private:
    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid pawn moves from x,y will be added to valid_moves
    //DESC: Generate pawn moves from position x,y
    void get_pawn_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid knight moves from x,y will be added to valid_moves
    //DESC: Generate knight moves from position x,y
    void get_knight_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid bishop moves from x,y will be added to valid_moves
    //DESC: Generate bishop moves from position x,y
    void get_bishop_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid rook moves from x,y will be added to valid_moves
    //DESC: Generate rook moves from position x,y
    void get_rook_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid queen moves from x,y will be added to valid_moves
    //DESC: Generate queen moves from position x,y
    void get_queen_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, x,y must be on the game board
    //POST: valid king moves from x,y will be added to valid_moves
    //DESC: Generate king moves from position x,y
    void get_king_moves(vector<string> & valid_moves, string color, int x, int y);

    //PRE : gameBoard must be populated correctly, castling string must be set
    //POST: valid castling moves will be added to valid_moves
    //DESC: Generate castling moves from castling string
    void get_castling_moves(vector<string> & valid_moves);

    //PRE : gameBoard must be populated correctly, en_passant string must be set
    //POST: valid en passant moves will be added to valid_moves
    //DESC: Generate en passant moves from en_passant string
    void get_en_passant_moves(vector<string> & valid_moves, string color);
};

#endif