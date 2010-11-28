/*********************************************************************************
                                    Aristocrat
							   Position Class Header
									position.h
                            Last Updated March 30, 2009

NOTES
    - Position class now also holds a counter for the number of half-moves played.
      Use this later when making up fun interface
**********************************************************************************/

#pragma once

#include "move.h"
#include <string>
#include <vector>

using namespace std;

class Position {

  public:
    int board[120];                      //12x10 board, 8x8 padded for knight moves

    int enPassant;                      //Holds valid enpassant square, or 0
    int castleWK;
    int castleWQ;                        //Castle values hold 1 if allowed, zero if not
    int castleBK;
    int castleBQ;
    int toMove;                           //1 for white, -1 for black
    int halfMoveNumber;                   //Number of half moves played
    int whiteKing;                        //Stores location of white and black king
    int blackKing;

    vector<int> whitePiecelist;           //Piecelists hold the position of each piece,
    vector<int> blackPiecelist;           //and a piece is simply deleted from the vector when captured

    Position();

    void output();

    bool movePiece(Move);                 //Moves a piece. Updates board and piecelists
	bool inCheck();
};
