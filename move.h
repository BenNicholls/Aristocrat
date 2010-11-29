/*********************************************************************************
                                    Aristocrat
							     Move Class Header
							          move.h
                            Last Updated March 10, 2009
**********************************************************************************/

#pragma once

class Move {

	public:
		int fromSpace;                        //Space where it is coming from
		int toSpace;                          //Space where it is going
		int score;                            //How good is move? You decide.
		int piece;                            //Records which piece is moving
		int capture;                          //Records which piece was hit
		bool enPassant;                        //1 if enpassant capture, 0 otherwise
		int castle;                           //0 for no, 1 for kingside, 2 for queen
		int promotion;                        //holds value of piece being created , or 0
		bool jump;                             //1 if pawn jumped 2 spaces, 0 if not

		Move();

		Move(int, int, int, int);             //Regular constructor, for normal moves
		                                      //Crazy constructor for crazies
		Move(int, int, int, int, bool, int, bool, int);

		Move(int, int, int, int, bool, int, bool, int, int, int);

		void output();                             //Outputs a description of the movess
};
