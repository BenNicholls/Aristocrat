/*********************************************************************************
                                    Aristocrat
                               Evaluation Function
                                     eval.cpp

NOTES:
	- Evaluation function loops through white pieces and black pieces seperately,
	  for now just counting material
 **********************************************************************************/

 #include "functions.h"
 #include "position.h"
 #include "definitions.h"
 #include <iostream>

 int eval(Position &Game) {
	int score = 0;
	for (unsigned int i = 0; i < Game.whitePiecelist.size(); i++) {
		if (Game.whitePiecelist[i] != NOBOARD) {
			score += PIECEVALUE[Game.board[Game.whitePiecelist[i]]];
			switch (Game.board[Game.whitePiecelist[i]]) {
				case PAWN:
					score += pawnSpaceTable[Game.whitePiecelist[i]];
					break;
				case KNIGHT:
					score += knightSpaceTable[Game.whitePiecelist[i]];
					break;
				case BISHOP:
					score += bishopSpaceTable[Game.whitePiecelist[i]];
					break; 
				case ROOK:
					score += rookSpaceTable[Game.whitePiecelist[i]];
					break; 
				case QUEEN:
					score += queenSpaceTable[Game.whitePiecelist[i]];
					break; 
				case KING:
					score += kingSpaceTable[Game.whitePiecelist[i]];
					break; 
			}
		}
	}

	for (unsigned int i = 0; i < Game.blackPiecelist.size(); i++) {
		if (Game.blackPiecelist[i] != NOBOARD) {
			score -= PIECEVALUE[abs(Game.board[Game.blackPiecelist[i]])];
			switch (abs(Game.board[Game.blackPiecelist[i]])) {
				case PAWN:
					score -= pawnSpaceTable[119 - Game.blackPiecelist[i]];
					break;
				case KNIGHT:
					score -= knightSpaceTable[119 - Game.blackPiecelist[i]];
					break;
				case BISHOP:
					score -= bishopSpaceTable[119 - Game.blackPiecelist[i]];
					break; 
				case ROOK:
					score -= rookSpaceTable[119 - Game.blackPiecelist[i]];
					break; 
				case QUEEN:
					score -= queenSpaceTable[119 - Game.blackPiecelist[i]];
					break; 
				case KING:
					score -= kingSpaceTable[119 - Game.blackPiecelist[i]];
					break; 
			}
		}
	}
	return score;
}