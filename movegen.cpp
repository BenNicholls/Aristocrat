#include "position.h"
#include "definitions.h"

void Position::generateMoves(Movelist &Moves) {
	vector<int> piecelist;

	//Load the correct piecelist
	if (toMove == WHITE) piecelist = whitePiecelist;
	else piecelist = blackPiecelist;

	//Loop through pieces
	for (unsigned int i = 0; i < piecelist.size(); i++) {
		int pieceSpace = piecelist[i];
		Moves.currPiece = board[pieceSpace];
		switch(abs(board[pieceSpace])) {
			case NOBOARD:
				break;

			case PAWN:
				//Check if space ahead is empty
				if (board[pieceSpace - 10*toMove] == EMPTY) {
					//Check if piece is about to promote!
					if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
						Moves.add_promos(pieceSpace, pieceSpace - 10*toMove, 0); 
					else {
						Moves.add_move(pieceSpace, pieceSpace - 10*toMove);
						//Test for jump moves!
						if (board[pieceSpace - 20*toMove] == EMPTY && pieceSpace >= (56+(25*toMove)) && pieceSpace < (64+(25*toMove)))
							Moves.add_pawnjump(pieceSpace, pieceSpace - 20*toMove);
					}
				}
				//Check for capture-left
				if (board[pieceSpace - 11*toMove]*toMove < 0 && board[pieceSpace - 11*toMove] != NOBOARD) {
					//Check for capture promotion
					if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
						Moves.add_promos(pieceSpace, pieceSpace - 11*toMove, board[pieceSpace - 11*toMove]);
					else Moves.add_capture(pieceSpace, pieceSpace - 11*toMove, board[pieceSpace - 11*toMove]);
				}
				//Check for enpassant left
				else if (pieceSpace - 11*toMove == enPassant) Moves.add_enpassant(pieceSpace, enPassant, -toMove);

				//Check for capture-right
				if (board[pieceSpace - 9*toMove]*toMove < 0 && board[pieceSpace - 9*toMove] != NOBOARD) {
					//Check for capture promotion
					if (pieceSpace >= (56-(25*toMove)) && pieceSpace < (64-(25*toMove)))
						Moves.add_promos(pieceSpace, pieceSpace - 9*toMove, board[pieceSpace - 9*toMove]);
					else Moves.add_capture(pieceSpace, pieceSpace - 9*toMove, board[pieceSpace - 9*toMove]);
				}
				else if (pieceSpace - 9*toMove == enPassant) Moves.add_enpassant(pieceSpace, enPassant, -toMove);
				break;

			case KNIGHT:			
				for (unsigned int j=0; j < 8; j++) {
					int moveSquare = pieceSpace + KNIGHTMOVES[j];
					//Regular move
					if(board[moveSquare] == EMPTY) Moves.add_move(pieceSpace, moveSquare);
					//Capture
					else if (board[moveSquare]*toMove < 0 && board[moveSquare] != NOBOARD) 
						Moves.add_capture(pieceSpace, moveSquare, board[moveSquare]);
				}
				break;

			case BISHOP:
				//Loop over possible directions
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
				break;

			case ROOK:
				//Loop over possible directions
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
				break;

			case QUEEN:
				//Loop over possible directions
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
				break;

			case KING:
				for (unsigned int j=0; j < 8; j++) {
					int moveSpace = pieceSpace + SLIDEMOVES[j];
					if (board[moveSpace] == EMPTY) {
						Moves.add_move(pieceSpace, moveSpace);
						//Castle checking time
						if (!inCheck()){ 
							if (toMove == WHITE) {
								if (j == 5 && castleWK == true) {
									if (board[97] == EMPTY) {
										if (!isAttacked(97) && !isAttacked(96)) Moves.add_castle(1, WHITE);
									}
								}
								else if (j == 7 && castleWQ == true) {
									if (board[93] == EMPTY && board[92] == EMPTY) {
										if (!isAttacked(93) && !isAttacked(94)) Moves.add_castle(2, WHITE);
									}
								}
							}
							else {
								if (j == 5 && castleBK == true) {
									if (board[27] == EMPTY) {
										if (!isAttacked(27) && !isAttacked(26)) Moves.add_castle(1, BLACK);
									}
								}
								else if (j == 7 && castleBQ == true) {
									if (board[23] == EMPTY && board[22] == EMPTY) {
										if (!isAttacked(23) && !isAttacked(24)) Moves.add_castle(2, BLACK);
									}
								}
							}
						}
					}
					else if (board[moveSpace]*toMove < 0 && board[moveSpace] != NOBOARD)
						Moves.add_capture(pieceSpace, moveSpace, board[moveSpace]);
				}
				break;
		}
	}
}