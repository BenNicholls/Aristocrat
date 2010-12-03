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
#include "hashtable.h"

using namespace std;

class Position {

  public:
	//Position Information
    int board[120];                      //12x10 board, 8x8 padded for knight moves
    int toMove; 
    unsigned int enPassant;                      //Holds valid enpassant square, or 0
    bool castleWK;
    bool castleWQ;                        //Castle values
    bool castleBK;
    bool castleBQ;
    unsigned int fiftyMove;                   //Number of half moves played
    unsigned int whiteKing;                        //Stores location of white and black king
    unsigned int blackKing;
	unsigned int totalMoves;
	unsigned int halfMoves;
	unsigned long long hash;


    vector<int> whitePiecelist;           //Piecelists hold the position of each piece,
    vector<int> blackPiecelist;           //and a piece is simply deleted from the vector when captured
	
	//Zobrist Hash Creation Keys
	unsigned long long whitePawnKeys[64];
	unsigned long long whiteKnightKeys[64];
	unsigned long long whiteBishopKeys[64];
	unsigned long long whiteRookKeys[64];
	unsigned long long whiteQueenKeys[64];
	unsigned long long whiteKingKeys[64];
	unsigned long long blackPawnKeys[64];
	unsigned long long blackKnightKeys[64];
	unsigned long long blackBishopKeys[64];
	unsigned long long blackRookKeys[64];
	unsigned long long blackQueenKeys[64];
	unsigned long long blackKingKeys[64];
	unsigned long long enpassantKeys[8];
	unsigned long long castleKeys[4];
	unsigned long long blackKey;
	unsigned long long keySeed;

	//History Keeping
	bool WKhistory[1024];
	bool WQhistory[1024];
	bool BKhistory[1024];
	bool BQhistory[1024];
	unsigned int enPassantHistory[1024];
	unsigned int fiftyMoveHistory[1024];
	unsigned long long hashHistory[1024];
	Movelist movesMade;

    Position();
	Position(string);
	void fenParse(string);
	
	void generateHash();
	void updatePieceHash(int, int, int);
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
	unsigned long long genRandomKey();
};
