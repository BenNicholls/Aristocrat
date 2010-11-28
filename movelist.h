/*********************************************************************************
                                    Aristocrat
								  Movelist Header
                                    movelist.h
                            Last Updated Nov 28, 2010

NOTES
	- Movelist object holds moves for a position, and contains helper functions for
	  generating moves.
**********************************************************************************/

#pragma once

#include <vector>
#include "move.h"

using namespace std;

class Movelist {

	public:

		vector<Move> list; //What for to hold the moves.

		Movelist();

		int currPiece; //Holds piece that moves are being generated for

		int old_enPassant;
		int old_fiftyMove;
		
		//Regular move. Parameters: fromSpace, toSpace
		void add_move(int, int);
		//Capture Move. Parameters: fromSpace, toSpace, captured piece
		void add_capture(int, int, int);
		//Promotions. Parameters: fromSpace, toSpace, captured piece (0 if no capture)
		void add_promos(int, int, int);
		//Castle moves. Parameters: castle number (1 king, 2 queen), side to move
		void add_castle(int, int);
		//Enpassant moves. Parameters: fromSpace, toSpace
		void add_enpassant(int, int, int);
		//Pawn jumps. Parameters: fromSpace, toSpace
		void add_pawnjump(int, int);

		void output();
};