/*********************************************************************************
                                    Aristocrat
							   Position Class Header
									position.h

NOTES
    - Position class now also holds a counter for the number of half-moves played.
**********************************************************************************/

#pragma once

#include "move.h"
#include <string>
#include <vector>
#include "movelist.h"

using namespace std;

class Position {

  public:
    int board[120];                      //12x10 board, 8x8 padded for knight moves

    unsigned int enPassant;                      //Holds valid enpassant square, or 0
    bool castleWK;
    bool castleWQ;                        //Castle values
    bool castleBK;
    bool castleBQ;
    int toMove;                           //1 for white, -1 for black
    unsigned int fiftyMove;                   //Number of half moves played
    unsigned int whiteKing;                        //Stores location of white and black king
    unsigned int blackKing;
	unsigned int totalMoves;
	unsigned int halfMoves;

    vector<int> whitePiecelist;           //Piecelists hold the position of each piece,
    vector<int> blackPiecelist;           //and a piece is simply deleted from the vector when captured
	
	bool WKhistory[5096];
	bool WQhistory[5096];
	bool BKhistory[5096];
	bool BQhistory[5096];
	unsigned int enPassantHistory[5096];
	unsigned int fiftyMoveHistory[5096];

	Movelist movesMade;

    Position();
	Position(string);
	void customSetup();
	void fenParse(string);

    void output();
	void outputDetails();
	void removePiece(int, int);
	void addPiece(int, int);
	void updatePiece(int, int, int);
	void updateHistory();
	bool doMove(Move &);
	void undoMove();
	bool isAttacked(int);
	bool inCheck();
	void generateMoves(Movelist &);
};
