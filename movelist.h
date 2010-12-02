/*********************************************************************************
                                    Aristocrat
								  Movelist Header
                                    movelist.h

NOTES
	- Movelist object holds moves for a position, and contains helper functions for
	  generating moves.
**********************************************************************************/

#pragma once

#include "move.h"

using namespace std;

class Movelist {

	public:

		Move list[512]; //What for to hold the moves.	
		unsigned int totalMoves;

		Movelist();

		int currPiece; //Holds piece that moves are being generated for
		
		//Regular move. Parameters: fromSpace, toSpace
		void add_move(int &, int &);
		//Capture Move. Parameters: fromSpace, toSpace, captured piece
		void add_capture(int &, int &, int &);
		//Promotions. Parameters: fromSpace, toSpace, captured piece (0 if no capture)
		void add_promos(int &, int, const int &);
		//Castle moves. Parameters: castle number (1 king, 2 queen), side to move
		void add_castle(int, int);
		//Enpassant moves. Parameters: fromSpace, toSpace, side being captured
		void add_enpassant(int &, int, int);
		//Pawn jumps. Parameters: fromSpace, toSpace
		void add_pawnjump(int &, int);

		void add(Move);
		void remove_last();
		void output();
};