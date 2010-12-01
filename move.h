/*********************************************************************************
                                    Aristocrat
							     Move Class Header
							          move.h
**********************************************************************************/

#pragma once

class Move {

	public:
		unsigned int fromSpace;                        //Space where it is coming from
		unsigned int toSpace;                          //Space where it is going
		int piece;                            //Records which piece is moving
		int capture;                          //Records which piece was hit
		bool enPassant;                        //1 if enpassant capture, 0 otherwise
		int castle;                           //0 for no, 1 for kingside, 2 for queen
		int promotion;                        //holds value of piece being created , or 0
		bool jump;                             //1 if pawn jumped 2 spaces, 0 if not

		Move();
		Move(int, int, int, int, bool, int, bool, int);

		void output();                             //Outputs a description of the moves
};
