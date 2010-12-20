/*********************************************************************************
                                    Aristocrat
                               Evaluation Function
                                     eval.cpp

NOTES:
	- Evaluation function loops through white pieces and black pieces seperately,
	  for now just counting material
**********************************************************************************/

#include "position.h"
#include "definitions.h"
#include "engine.h"
#include <iostream>

int Engine::evaluate() {
	int score = 0;

	for (unsigned int i = 0; i < Game.pieceListTotals[0][PAWN]; i++) {
		score += PIECEVALUE[PAWN];
		score += pawnSpaceTable[119 - Game.pieceLists[0][PAWN][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[0][KNIGHT]; i++) {
		score += PIECEVALUE[KNIGHT];
		score += pawnSpaceTable[119 - Game.pieceLists[0][KNIGHT][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[0][BISHOP]; i++) {
		score += PIECEVALUE[BISHOP];
		score += pawnSpaceTable[119 - Game.pieceLists[0][BISHOP][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[0][ROOK]; i++) {
		score += PIECEVALUE[ROOK];
		score += pawnSpaceTable[119 - Game.pieceLists[0][ROOK][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[0][QUEEN]; i++) {
		score += PIECEVALUE[QUEEN];
		score += pawnSpaceTable[119 - Game.pieceLists[0][QUEEN][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[1][PAWN]; i++) {
		score -= PIECEVALUE[PAWN];
		score -= pawnSpaceTable[Game.pieceLists[1][PAWN][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[1][KNIGHT]; i++) {
		score -= PIECEVALUE[KNIGHT];
		score -= pawnSpaceTable[Game.pieceLists[1][KNIGHT][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[1][BISHOP]; i++) {
		score -= PIECEVALUE[BISHOP];
		score -= pawnSpaceTable[Game.pieceLists[1][BISHOP][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[1][ROOK]; i++) {
		score -= PIECEVALUE[ROOK];
		score -= pawnSpaceTable[Game.pieceLists[1][ROOK][i]];
	}
	for (unsigned int i = 0; i < Game.pieceListTotals[1][QUEEN]; i++) {
		score -= PIECEVALUE[QUEEN];
		score -= pawnSpaceTable[Game.pieceLists[1][QUEEN][i]];
	}

	return score;
}