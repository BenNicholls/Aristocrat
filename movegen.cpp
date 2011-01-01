#include "position.h"
#include "definitions.h"

void Position::generatePawnMoves(Movelist &Moves, int side) {	
	for (unsigned int i = 0; i < pieceListTotals[side][PAWN]; i++) {
		int pieceSpace = pieceLists[side][PAWN][i];
		Moves.currPiece = PAWN*toMove;
			//Check if space ahead is empty
			if (board[pieceSpace - 10*toMove] == EMPTY) {
				//Check if piece is about to promote!
				if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
					Moves.add_promos(pieceSpace, pieceSpace - 10*toMove, EMPTY, toMove); 
				else {
					int to = pieceSpace - 10*toMove;
					Moves.add_move(pieceSpace, to);
					//Test for jump moves!
					if (board[pieceSpace - 20*toMove] == EMPTY && pieceSpace >= (56+(25*toMove)) && pieceSpace < (64+(25*toMove)))
						Moves.add_pawnjump(pieceSpace, pieceSpace - 20*toMove);
				}
			}
			//Check for capture-left
			if (board[pieceSpace - 11*toMove]*toMove < 0 && board[pieceSpace - 11*toMove] != NOBOARD) {
				//Check for capture promotion
				if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
					Moves.add_promos(pieceSpace, pieceSpace - 11*toMove, board[pieceSpace - 11*toMove], toMove);
				else {
					int to = pieceSpace - 11*toMove;
					Moves.add_capture(pieceSpace, to, board[pieceSpace - 11*toMove]);
				}
			}
			//Check for enpassant left
			else if (pieceSpace - 11*toMove == enPassant) Moves.add_enpassant(pieceSpace, enPassant, -toMove);

			//Check for capture-right
			if (board[pieceSpace - 9*toMove]*toMove < 0 && board[pieceSpace - 9*toMove] != NOBOARD) {
				//Check for capture promotion
				if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
					Moves.add_promos(pieceSpace, pieceSpace - 9*toMove, board[pieceSpace - 9*toMove], toMove);
				else {
					int to =  pieceSpace - 9*toMove;
					Moves.add_capture(pieceSpace, to, board[pieceSpace - 9*toMove]);
				}
			}
			else if (pieceSpace - 9*toMove == enPassant) Moves.add_enpassant(pieceSpace, enPassant, -toMove);
	}
}

void Position::generateKnightMoves(Movelist &Moves, int side) {
	for (unsigned int i = 0; i < pieceListTotals[side][KNIGHT]; i++) {
		int pieceSpace = pieceLists[side][KNIGHT][i];
		Moves.currPiece = KNIGHT*toMove;
		for (unsigned int j=0; j < 8; j++) {
			int moveSquare = pieceSpace + KNIGHTMOVES[j];
			//Regular move
			if(board[moveSquare] == EMPTY) Moves.add_move(pieceSpace, moveSquare);
			//Capture
			else if (board[moveSquare]*toMove < 0 && board[moveSquare] != NOBOARD) 
				Moves.add_capture(pieceSpace, moveSquare, board[moveSquare]);
		}
	}
}

void Position::generateBishopMoves(Movelist &Moves, int side) {
	for (unsigned int i = 0; i < pieceListTotals[side][BISHOP]; i++) {
		int pieceSpace = pieceLists[side][BISHOP][i];
		Moves.currPiece = BISHOP*toMove;
		for (unsigned int j=0; j < 4; j++) {
			int raySpace = pieceSpace;
			bool ray = true;
			//extend ray
			while (ray) {
				raySpace += SLIDEMOVES[j];
				if (board[raySpace] == 0) Moves.add_move(pieceSpace, raySpace);
				else if (board[raySpace]*toMove < 0 && board[raySpace] != NOBOARD) {
					Moves.add_capture(pieceSpace, raySpace, board[raySpace]);
					ray = false;
				}
				else ray = false;
			}
		}
	}
}

void Position::generateRookMoves(Movelist &Moves, int side) {
	for (unsigned int i = 0; i < pieceListTotals[side][ROOK]; i++) {
		int pieceSpace = pieceLists[side][ROOK][i];
		Moves.currPiece = ROOK*toMove;
		for (unsigned int j=4; j < 8; j++) {
			int raySpace = pieceSpace;
			bool ray = true;
			//extend ray
			while (ray) {
				raySpace += SLIDEMOVES[j];
				if (board[raySpace] == 0) Moves.add_move(pieceSpace, raySpace);
				else if (board[raySpace]*toMove < 0 && board[raySpace] != NOBOARD) {
					Moves.add_capture(pieceSpace, raySpace, board[raySpace]);
					ray = false;
				}
				else ray = false;
			}
		}
	}
}

void Position::generateQueenMoves(Movelist &Moves, int side) {
	for (unsigned int i = 0; i < pieceListTotals[side][QUEEN]; i++) {
		int pieceSpace = pieceLists[side][QUEEN][i];
		Moves.currPiece = QUEEN*toMove;
		for (unsigned int j=0; j < 8; j++) {
			int raySpace = pieceSpace;
			bool ray = true;
			//extend ray
			while (ray) {
				raySpace += SLIDEMOVES[j];
				if (board[raySpace] == 0) Moves.add_move(pieceSpace, raySpace);
				else if (board[raySpace]*toMove < 0 && board[raySpace] != NOBOARD) {
					Moves.add_capture(pieceSpace, raySpace, board[raySpace]);
					ray = false;
				}
				else ray = false;
			}
		}
	}
}

void Position::generateKingMoves(Movelist &Moves, int kingSpace) {
	Moves.currPiece = KING*toMove;
	for (unsigned int j=0; j < 8; j++) {
		int moveSpace = kingSpace + SLIDEMOVES[j];
		if (board[moveSpace] == EMPTY) {
			Moves.add_move(kingSpace, moveSpace);
			//Castle checking time
			if (!inCheck()){ 
				if (toMove == WHITE) {
					if (j == 5 && castleWK == true) {
						if (board[97] == EMPTY) {
							if (!isAttacked(97) && !isAttacked(96)) Moves.add_castle(1, 1);
						}
					}
					else if (j == 7 && castleWQ == true) {
						if (board[93] == EMPTY && board[92] == EMPTY) {
							if (!isAttacked(93) && !isAttacked(94)) Moves.add_castle(2, 1);
						}
					}
				}
				else {
					if (j == 5 && castleBK == true) {
						if (board[27] == EMPTY) {
							if (!isAttacked(27) && !isAttacked(26)) Moves.add_castle(1, -1);
						}
					}
					else if (j == 7 && castleBQ == true) {
						if (board[23] == EMPTY && board[22] == EMPTY) {
							if (!isAttacked(23) && !isAttacked(24)) Moves.add_castle(2, -1);
						}
					}
				}
			}
		}
		else if (board[moveSpace]*toMove < 0 && board[moveSpace] != NOBOARD)
			Moves.add_capture(kingSpace, moveSpace, board[moveSpace]);
	}
}

void Position::generateMoves(Movelist &Moves) {
	unsigned int side;
	if (toMove == WHITE) {
		side = 0;
		generateKingMoves(Moves, whiteKing);
	}
	else {
		side = 1;
		generateKingMoves(Moves, blackKing);
	}

	generatePawnMoves(Moves, side);
	generateKnightMoves(Moves, side);
	generateBishopMoves(Moves, side);
	generateRookMoves(Moves, side);
	generateQueenMoves(Moves, side);
}