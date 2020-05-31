#include "game_logic.h"

void gameState::populate_board(const string fen){
  //extrapolate lines from the fen string
  //Vector of each line of the board (should be 8)
  vector<string> lines;
  stringstream sstr(fen);
  string middle;
  while(getline(sstr, middle, '/')) { 
    lines.push_back(middle); 
  }

  //evaluate the last line to get the castling move and the en passant move
  vector<string> extra_moves;
  stringstream sstr2(fen);
  string mid2;
  while(getline(sstr2, mid2, ' ')){
    extra_moves.push_back(mid2);
  }

  //set the correct move strings
  castling = extra_moves[2];
  en_passant = extra_moves[3];

  //Replace numbers with the appropriate number of blank spaces
  for(int i = 0; i < lines.size(); i++){
    for(int pos = 0; pos < 9; pos++){
      //if there is a number rather than a piece character, get value
      if(int(lines[i][pos]) > 47 && int(lines[i][pos]) < 57){
        int num = lines[i][pos] - '0';
        lines[i].erase(pos, 1);
        //insert that many blank spaces
        for(int k =0; k < num; k++){
          lines[i].insert(pos, "-");
          pos++;
        }
      }
    }
  }


  //For each row of the board (1 thru 8)
  for(int i = 0; i < lines.size(); i++){
    //For each position on the board
    for(int j = 0; j < lines[i].length(); j++){
      if(j < 8){
        gameBoard[i][j] = lines[i][j];
      }
    }
  }
}

//Print the current board datastructure
void gameState::print_board(){
  cout << "  a b c d e f g h\n";
  for(int i = 0; i < 8; i++){
    cout << (8-i) << " ";
    for (int j = 0; j < 8; j++){
      cout << gameBoard[i][j] << " ";
    }
    cout << "\n";
  }
}

void gameState::get_valid_moves(vector<string> & valid_moves, string color, char gameBoard[][8]){
  //If the castling string is not blank, generate moves indicated
  if(castling != "-"){
    get_castling_moves(valid_moves);
  }
  //If the en_passant string is not blank, generate moves indicated
  if(en_passant != "-"){
    get_en_passant_moves(valid_moves, color);
  }
  //For each piece on the board
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      //If the current space is not a blank
      if(gameBoard[i][j] != '-'){
        //Generate all moves for appropriate piece and color
        if(color == "white"){
          switch(gameBoard[i][j]){
            case 'P':
              get_pawn_moves(valid_moves, color, i, j);
              break;
            case 'N':
              get_knight_moves(valid_moves, color, i, j);
              break;
            case 'B':
              get_bishop_moves(valid_moves, color, i, j);
              break;
            case 'R':
              get_rook_moves(valid_moves, color, i, j);
              break;
            case 'Q':
              get_queen_moves(valid_moves, color, i, j);
              break;
            case 'K':
              get_king_moves(valid_moves, color, i, j);
              //For debugging, is the King in Check?
              //cout << "Is King in Check? " << isKingCheck(gameBoard, color, i, j) << endl;
              break;
          }
        } else if(color == "black"){
          switch(gameBoard[i][j]){
            case 'p':
              get_pawn_moves(valid_moves, color, i, j);
              break;
            case 'n':
              get_knight_moves(valid_moves, color, i, j);
              break;
            case 'b':
              get_bishop_moves(valid_moves, color, i, j);
              break;
            case 'r':
              get_rook_moves(valid_moves, color, i, j);
              break;
            case 'q':
              get_queen_moves(valid_moves, color, i, j);
              break;
            case 'k':
              get_king_moves(valid_moves, color, i, j);
              // For debugging, is the King in Check?
              //cout << "Is King in Check? " << isKingCheck(gameBoard, color, i, j) << endl;
              break;
          }
        }
      }
    }
  }
}

void gameState::get_pawn_moves(vector<string> & valid_moves, string color, int x, int y){
  char dupBoard[8][8];     //Board used to simulate the move and check for check
  bool pieceTaken = false; //Indicates whether a peice will be taken for the move
  int kingx;  //King's position x
  int kingy;  //King's position y

  if(color == "white"){ //If white, move up
    //Check if a single space forward is a valid move
    if(gameBoard[x-1][y] == '-' && x-1 < 8 && y < 8 && x-1 >= 0 && y >= 0){
      //Copy the board to simulate the move
      copyBoard(gameBoard, dupBoard, 'P', x, y, x-1, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, it is a valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-1,y));
      }
    }
    //If the pawn is in file 2, check if it is able to move 2 spaces
    if(x == 6 && gameBoard[x-1][y] == '-' && gameBoard[x-2][y] == '-' && x-2 < 8 && y < 8 && x-2 >= 0 && y >= 0){
      //copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, 'P', x, y, x-2, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, it is a valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-2,y));
      }
    }
    //If there is an enemy piece on a diagonal, check the space
    if(isEnemyPiece(gameBoard[x-1][y+1], color) && check_space(gameBoard, color, x-1, y+1, pieceTaken)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, 'P', x, y, x-1, y+1);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, it is a valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-1,y+1));
      }
    }
    //Check other diagonal
    if(isEnemyPiece(gameBoard[x-1][y-1], color) && check_space(gameBoard, color, x-1, y-1, pieceTaken)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, 'P', x, y, x-1, y-1);
      getKingPos(dupBoard, color, kingx, kingy);
      //if king not in check, valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-1,y-1));
      }
    }
  }
  else if(color == "black"){ //If black move down
    //Check if a single space forward is a valid move
    if(gameBoard[x+1][y] == '-' && x+1 < 8 && y < 8 && x+1 >= 0 && y >= 0){
      //copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, 'p', x, y, x+1, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //if  the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+1,y));
      }
    }
    //If the pawn is in file 7, check if it can move 2 spaces
    if(x == 1 && gameBoard[x+1][y] == '-' && gameBoard[x+2][y] == '-' && x+2 < 8 && y < 8 && x+2 >= 0 && y >= 0){
      //copy the board to simulate a move
      copyBoard(gameBoard, dupBoard, 'p', x, y, x+2, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //if the king is not in check, valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+2,y));
      }
    }
    //if an enemy piece is on a diagonal, and move is legal
    if(isEnemyPiece(gameBoard[x+1][y+1], color) && check_space(gameBoard, color, x+1, y+1, pieceTaken)){
      //copy and simulate
      copyBoard(gameBoard, dupBoard, 'p', x, y, x+1, y+1);
      getKingPos(dupBoard, color, kingx, kingy);
      //if king not in check, valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+1,y+1));
      }
    }
    //Check other diagonal
    if(isEnemyPiece(gameBoard[x+1][y-1], color) && check_space(gameBoard, color, x+1, y-1, pieceTaken)){
      //Copy, simulate move
      copyBoard(gameBoard, dupBoard, 'p', x, y, x+1, y-1);
      getKingPos(dupBoard, color, kingx, kingy);
      //if king not in check, valid move
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+1,y-1));
      }
    }
  }
}

void gameState::get_knight_moves(vector<string> & valid_moves, string color, int x, int y){
  bool pieceTaken = false;  //indicates if move will take an enemy peice 
  char dupBoard[8][8];      //space to duplucate the board and simulate a move
  int kingx;
  int kingy;
  //Check all 8 valid positions manually
  
  if(check_space(gameBoard, color, x+2, y+1, pieceTaken)){
    //Copy and simulate move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x+2, y+1);
    getKingPos(dupBoard, color, kingx, kingy);
    //If king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x+2,y+1));
    }
  }
  if(check_space(gameBoard, color, x+2, y-1, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x+2, y-1);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x+2,y-1));
    }
  }
  if(check_space(gameBoard, color, x-2, y+1, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x-2, y+1);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x-2,y+1));
    }
  }
  if(check_space(gameBoard, color, x-2, y-1, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x-2, y-1);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x-2,y-1));
    }
  }
  if(check_space(gameBoard, color, x+1, y+2, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x+1, y+2);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x+1,y+2));
    }
  }
  if(check_space(gameBoard, color, x-1, y+2, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x-1, y+2);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x-1,y+2));
    }
  }
  if(check_space(gameBoard, color, x+1, y-2, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x+1, y-2);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx,  kingy)){
      valid_moves.push_back(move_string(x,y,x+1,y-2));
    }
  }
  if(check_space(gameBoard, color, x-1, y-2, pieceTaken)){
    //copy the board and simulate the move
    copyBoard(gameBoard, dupBoard, (color == "white" ? 'N' : 'n'), x, y, x-1, y-2);
    getKingPos(dupBoard, color, kingx, kingy);
    //if the king not in check, move is valid
    if(!isKingCheck(dupBoard, color, kingx, kingy)){
      valid_moves.push_back(move_string(x,y,x-1,y-2));
    }
  }
}

void gameState::get_bishop_moves(vector<string> & valid_moves, string color, int x, int y){
  //Use for loop to look for valid moves in the four diagonal directions.
  //If theres no valid move or a peice is taken, the piece can go no more in that direction.

  bool peiceReplaced = false;  //indicates if a peice will be replaced by a move
  char dupBoard[8][8];         //Space to copy  the board to simulate a move
  int kingx;
  int kingy;

  //For every space in a direction
  for(int i = 1; i < 9; i++){
    //If the spot is a valid move
    if(check_space(gameBoard, color, x+i, y+i, peiceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'B' : 'b'), x, y, x+i, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is  valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y+i));
      }
    //If the move is not valid, there will be no more valid moves in this direction.
    } else break;
    //When a peice is taken, there will be no more valid moves this direction
    if(peiceReplaced) break;
  }
  peiceReplaced = false;

  //For every space in a direction
  for(int i = 1; i < 9; i++){
    //If the spot is a valid move
    if(check_space(gameBoard, color, x-i, y-i, peiceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'B' : 'b'), x, y, x-i, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is  valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y-i));
      }
    //If the move is not valid, there will be no more valid moves in this direction.
    } else break;
    //When a peice is taken, there will be no more valid moves this direction
    if(peiceReplaced) break;
  }
  peiceReplaced = false;

  //For every space in a direction
  for(int i = 1; i < 9; i++){
    //If the spot is a valid move
    if(check_space(gameBoard, color, x+i, y-i, peiceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'B' : 'b'), x, y, x+i, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is  valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y-i));
      }
    //If the move is not valid, there will be no more valid moves in this direction.
    } else break;
    //When a peice is taken, there will be no more valid moves this direction
    if(peiceReplaced) break;
  }
  peiceReplaced = false;

  //For every space in a direction
  for(int i = 1; i < 9; i++){
    //If the spot is a valid move
    if(check_space(gameBoard, color, x-i, y+i, peiceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'B' : 'b'), x, y, x-i, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is  valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y+i));
      }
    //If the move is not valid, there will be no more valid moves in this direction.
    } else break;
    //When a peice is taken, there will be no more valid moves this direction
    if(peiceReplaced) break;
  }
}

void gameState::get_rook_moves(vector<string> & valid_moves, string color, int x, int y){
  //Use for loop to look for valid moves in the four directions.
  //If theres no valid move or a peice is taken, the piece can go no more in that direction.

  bool pieceReplaced = false;  //Indicates whether a peice has been taken in that direction
  char dupBoard[8][8];         //Space to copy the board to simulate  a move
  int kingx;
  int kingy;

  //For a direction, check moves
  for(int i = 1; i < 9; i++){
    //Check if space is available
    if(check_space(gameBoard, color, x+i, y, pieceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'R' : 'r'), x, y, x+i, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y));
      }
    //If the move isnt valid, there will be no more moves this direction
    } else break;
    //If a piece has been taken, then there will be no more valid moves in this direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //For a direction, check moves
  for(int i = 1; i < 9; i++){
    //Check if space is available
    if(check_space(gameBoard, color, x-i, y, pieceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'R' : 'r'), x, y, x-i, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y));
      }
    //If the move isnt valid, there will be no more moves this direction
    } else break;
    //If a piece has been taken, then there will be no more valid moves in this direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //For a direction, check moves
  for(int i = 1; i < 9; i++){
    //Check if space is available
    if(check_space(gameBoard, color, x, y+i, pieceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'R' : 'r'), x, y, x, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x,y+i));
      }
    //If the move isnt valid, there will be no more moves this direction
    } else break;
    //If a piece has been taken, then there will be no more valid moves in this direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //For a direction, check moves
  for(int i = 1; i < 9; i++){
    //Check if space is available
    if(check_space(gameBoard, color, x, y-i, pieceReplaced)){
      //Copy the board and simulate the move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'R' : 'r'), x, y, x, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x,y-i));
      }
    //If the move isnt valid, there will be no more moves this direction
    } else break;
    //If a piece has been taken, then there will be no more valid moves in this direction
    if(pieceReplaced) break;
  }
}

void gameState::get_queen_moves(vector<string> & valid_moves, string color, int x, int y){
  //Use for loop to look for valid moves in the eight directions.
  //If theres no valid move or a peice is taken, the piece can go no more in that direction.

  bool pieceReplaced = false;  //Indicates whether a piece has been taken
  char dupBoard[8][8];         //space to copy the board to simulate a move
  int kingx;
  int kingy;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x+i, y+i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x+i, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y+i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x-i, y-i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x-i, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y-i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x+i, y-i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x+i, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y-i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x-i, y+i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x-i, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y+i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x+i, y, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x+i, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x+i,y));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x-i, y, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x-i, y);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x-i,y));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x, y+i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x, y+i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x,y+i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
  pieceReplaced = false;

  //for a direction
  for(int i = 1; i < 9; i++){
    //if space is valid for a move
    if(check_space(gameBoard, color, x, y-i, pieceReplaced)){
      //Copy the  board and simulate a move
      copyBoard(gameBoard, dupBoard, (color == "white" ? 'Q' : 'q'), x, y, x, y-i);
      getKingPos(dupBoard, color, kingx, kingy);
      //If the king is not in check, move is valid
      if(!isKingCheck(dupBoard, color, kingx, kingy)){
        valid_moves.push_back(move_string(x,y,x,y-i));
      }
    //If the move isnt valid, there will not be another valid move in that direction
    } else break;
    //If a piece is taken, there will be no more valid moves that direction
    if(pieceReplaced) break;
  }
}

void gameState::get_king_moves(vector<string> & valid_moves, string color, int x, int y){
  //Check the moves around the king. If the move prevents check, it is valid
  
  bool pieceTaken = false; //Indicates whether peice has been taken
  char dupBoard[8][8];     //space to duplicate board to check for check
  //for the area around the current king
  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      if(i != 0 || j != 0){
        //If the space is a valid space
        if(check_space(gameBoard, color, x+i,y+j, pieceTaken)){
          //Copy the board and simulate the move
          copyBoard(gameBoard, dupBoard, (color == "white" ? 'K' : 'k'), x, y, x+i, y+j);
          if(!isKingCheck(dupBoard, color, x+i, y+j)){
            valid_moves.push_back(move_string(x,y,x+i,y+j));
          }
        }
      }
    }
  }
}

void gameState::get_castling_moves(vector<string> & valid_moves){
  char dupBoard[8][8];
  //Castling moves the king two spots towards the target rook,
  // then moves the rook to the other side of the king.
  if(castling.find("K")){
    //White to castle kingside
    if(gameBoard[0][0] == 'R' &&
       gameBoard[0][1] == '-' &&
       gameBoard[0][2] == '-' &&
       gameBoard[0][3] == '-' &&
       gameBoard[0][4] == 'K'){
         //copy board, move king
         copyBoard(gameBoard, dupBoard, 'K', 0, 4, 0, 1);
         //move rook
         dupBoard[0][2] = 'R';
         dupBoard[0][0] = '-';
         //if not in check, valid
         if(!isKingCheck(dupBoard, "white", 0, 1)){
           valid_moves.push_back("e1c1");
         }
    }
  }
  if(castling.find("Q")){
    //White can castle queenside
    if(gameBoard[0][7] == 'R' &&
       gameBoard[0][6] == '-' &&
       gameBoard[0][5] == '-' &&
       gameBoard[0][4] == 'K'){
         //copy board, move king
         copyBoard(gameBoard, dupBoard, 'K', 0, 4, 0, 5);
         //move rook
         dupBoard[0][7] ='-';
         dupBoard[0][6] = 'R';
         //if not in check, valid
         if(!isKingCheck(dupBoard, "white", 0, 5)){
           valid_moves.push_back("e1g1");
         }
    }
    
  }
  if(castling.find("k")){
    //Black can castle kingside
    if(gameBoard[7][0] == 'r' &&
       gameBoard[7][1] == '-' &&
       gameBoard[7][2] == '-' &&
       gameBoard[7][3] == '-' &&
       gameBoard[7][4] == 'k'){
         //copy board, move king
         copyBoard(gameBoard, dupBoard, 'k', 7, 4, 7, 1);
         //move rook
         dupBoard[7][2] = 'r';
         dupBoard[7][0] = '-';
         //if not in check, valid
         if(!isKingCheck(dupBoard, "black", 7, 1)){
           valid_moves.push_back("e8c8");
         }
    }
  }
  if(castling.find("q")){
    //Black can castle queenside
    if(gameBoard[7][7] == 'r' &&
       gameBoard[7][6] == '-' &&
       gameBoard[7][5] == '-' &&
       gameBoard[7][4] == 'k'){
         //copy board, move king
         copyBoard(gameBoard, dupBoard, 'k', 7, 4, 7, 5);
         //move rook
         dupBoard[7][7] ='-';
         dupBoard[7][6] = 'r';
         //if not in check, valid
         if(!isKingCheck(dupBoard, "black", 7, 5)){
           //Add move to list
           valid_moves.push_back("e8g8");
         }
    }
  }
}

void gameState::get_en_passant_moves(vector<string> & valid_moves, string color){
  if(en_passant != "-"){
    //get char position x,y from en_passant string
    int x = char(en_passant[0]-'0')-'0';
    int y = en_passant[0]-'0';
    //if white
    if(color == "white"){
      //check that diagonal is valid
      if(x+1 >= 0 && x+1 < 8){
        //if there is a pawn, then it can perform move
        if(gameBoard[x+1][y+1] == 'p'){
          valid_moves.push_back(move_string(x+1, y+1, x, y));
        }
      }
      //check if other diagonal is valid
      if(x-1 >= 0 && x-1 < 8){
        //if there is a pawn, then it can perform move
        if(gameBoard[x-1][y+1] == 'p'){
          valid_moves.push_back(move_string(x-1, y+1, x, y));
        }
      }
    //If black
    } else if(color == "black"){
      //check that diagonal is valid
      if(x+1 >= 0 && x+1 < 8){
        //if there is a pawn, then it can perform move
        if(gameBoard[x+1][y-1] == 'P'){
          valid_moves.push_back(move_string(x+1, y-1, x, y));
        }
      }
      //check that diagonal is valid
      if(x-1 >= 0 && x-1 < 8){
        //if there is a pawn, then it can perform move
        if(gameBoard[x-1][y-1] == 'P'){
          valid_moves.push_back(move_string(x-1, y-1, x, y));
        }
      }
    }
  }
}

bool check_space(char gameBoard[][8], string color, int x, int y, bool & pieceReplaced){
  //If the space is on the game board
  if((x < 8 && y < 8) && (x >= 0 && y >= 0)){
    //if the space is blank
    if(gameBoard[x][y] == '-'){
      return true;
    }
    //if the space is an enemy piece
    if(isEnemyPiece(gameBoard[x][y], color)){
      pieceReplaced = true;
      return true;
    }
  }
  return false;
}

string move_string(int startx, int starty, int x, int y){
  //Translate two numbers to UCI move format
  string m_string = "";
  m_string.push_back(char(starty+'0')+49);
  m_string.append(to_string(8-startx));
  m_string.push_back(char(y+'0')+49);
  m_string.append(to_string(8-x));
  return m_string;
}

bool isEnemyPiece(char piece, string color){
  if(color == "white"){
    if(piece == 'p' ||
       piece == 'r' ||
       piece == 'n' ||
       piece == 'b' ||
       piece == 'q' ||
       piece == 'k'){
         return true;
       }
       else return false;
  }
  else if(color  == "black"){
    if(piece == 'P' ||
       piece == 'R' ||
       piece == 'N' ||
       piece == 'B' ||
       piece == 'Q' ||
       piece == 'K'){
         return true;
       }
       else return false;
  }
  return false;
}

bool isKingCheck(char gameBoard[][8], string color, int x, int y){
  //Manually check if there is a knight in 8 spaces that could put king in check
  if(gameBoard[x+2][y+1] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x+2][y-1] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x-2][y+1] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x-2][y-1] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x+1][y+2] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x-1][y+2] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x+1][y-2] == (color == "black" ? 'N' : 'n') ||
     gameBoard[x-1][y-2] == (color == "black" ? 'N' : 'n')){
       return true;
  }
  //check all directions radially for a piece to put king in check.
  //Diagonal axises check for queen, king, and bishop
  //Vertical/horizontal axises check for queen, king, rook 
  for(int i = 1; i < 9; i++){
    if(x+i > 7 || x+i < 0) break;
    if(gameBoard[x+i][y] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x+i][y] == (color == "black" ? 'R' : 'r')){
      return true;
    } else if(gameBoard[x+i][y] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(y+i > 7 || y+i < 0) break;
    if(gameBoard[x][y+i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x][y+i] == (color == "black" ? 'R' : 'r')){
      return true;
    } else if(gameBoard[x][y+i] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(x-i > 7 || x-i < 0) break;
    if(gameBoard[x-i][y] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x-i][y] == (color == "black" ? 'R' : 'r')){
      return true;
    } else if(gameBoard[x-i][y] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(y-i > 7 || y-i < 0) break;
    if(gameBoard[x][y-i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x][y-i] == (color == "black" ? 'R' : 'r')){
      return true;
    } else if(gameBoard[x][y-i] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(y+i > 7 || y+i < 0) break;
    if(x+i > 7 || x+i < 0) break;
    if(gameBoard[x+i][y+i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x+i][y+i] == (color == "black" ? 'B' : 'b')){
      return true;
    } else if(gameBoard[x+i][y+i] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(x+i > 7 || x+i < 0) break;
    if(y-i > 7 || y-i < 0) break;
    if(gameBoard[x+i][y-i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x+i][y-i] == (color == "black" ? 'B' : 'b')){
      return true;
    } else if(gameBoard[x+i][y-i] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(x-i > 7 || x-i < 0) break;
    if(y+i > 7 || y+i < 0) break;
    if(gameBoard[x-i][y+i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x-i][y+i] == (color == "black" ? 'B' : 'b')){
      return true;
    } else if(gameBoard[x-i][y+i] != '-'){
      break;
    }
  }
  for(int i = 1; i < 9; i++){
    if(x-i > 7 || x-i < 0) break;
    if(y-i > 7 || y-i < 0) break;
    if(gameBoard[x-i][y-i] == (color == "black" ? 'Q' : 'q') || 
       gameBoard[x-i][y-i] == (color == "black" ? 'B' : 'b')){
      return true;
    } else if(gameBoard[x-i][y-i] != '-'){
      break;
    }
  }

  //Check if a king is putting you in check
  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      if(gameBoard[x+i][y+j] == (color == "black" ? 'K' : 'k')){
        return true;
      }
    }
  }

  //Check if pawns are putting the king in check
  if(color == "white"){
    if((x-1 < 8 && x-1 >= 0) && ((y+1 < 8 && y+1 >= 0) || (y-1 < 8 && y-1 >= 0))){
      if(gameBoard[x-1][y+1] == 'p' || gameBoard[x-1][y-1] == 'p'){
        return true;
      }
    }
  }else if(color == "black"){
    if((x+1 < 8 && x+1 >= 0) && ((y+1 < 8 && y+1 >= 0) || (y-1 < 8 && y-1 >= 0))){
      if(gameBoard[x+1][y+1] == 'P' || gameBoard[x+1][y-1] == 'P'){
        return true;
      }
    }
  }
  return false;
}

void copyBoard(char gameBoard[][8], char copyBoard[][8], char character, int startx, int starty, int x, int y){
  //copy gameBoard into copyBoard 1-1
  for(int a = 0; a < 8; a++){
    for(int b = 0; b < 8; b++){
      copyBoard[a][b] = gameBoard[a][b];
    }
  }
  //If valid, move the given peice
  if(startx >= 0 && startx < 8 && starty >= 0 && starty < 8){
    copyBoard[startx][starty] = '-';
    copyBoard[x][y] = character;
  }
}

void getKingPos(char gameBoard[][8], string color, int & kingx, int & kingy){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(gameBoard[i][j] == 'K' && color == "white"){
        kingx = i;
        kingy = j;
        break;
      }
      else if(gameBoard[i][j] == 'k' && color == "black"){
        kingx = i;
        kingy = j;
        break;
      }
    }
  }
  return;
}