/*********************************************************************************
                                    Aristocrat
							   Position Class Header
									position.h
                          Last Updated November 28, 2019

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

    int enPassant;                      //Holds valid enpassant square, or 0
    bool castleWK;
    bool castleWQ;                        //Castle values
    bool castleBK;
    bool castleBQ;
    int toMove;                           //1 for white, -1 for black
    int fiftyMove;                   //Number of half moves played
    int whiteKing;                        //Stores location of white and black king
    int blackKing;
	int totalMoves;

    vector<int> whitePiecelist;           //Piecelists hold the position of each piece,
    vector<int> blackPiecelist;           //and a piece is simply deleted from the vector when captured
	
	vector<bool> WKhistory;
	vector<bool> WQhistory;
	vector<bool> BKhistory;
	vector<bool> BQhistory;
	vector<int> enPassantHistory;
	vector<int> fiftyMoveHistory;

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
	bool doMove(Move);
	void undoMove();
	bool isAttacked(int);
	bool inCheck();
	void generateMoves(Movelist &);
};
