/*********************************************************************************
                                    Aristocrat
								  Move Generator
								   movegen.cpp
							Last Updated March 30, 2009

NOTES:
	- Move generator is fully awesome pseudo-legal. So far it generates all moves
	  for the position with no messups, including en passants, promotions (one move
	  for each possible promotion type), castling, and all that other jazz.
    - Generator is a bit slow. Latest test shows the generator working at about
      253k movelists generated per second, which is less than Sir Chess. Of course,
      Sir Chess didn't have promotions or castling, but still.
    - New note: each of those movelists had 25 moves, meaning Aristocrat
      can generate 6.7 million moves per second. Shit son, all of a sudden it
      don't seem so bad!
 **********************************************************************************/

 #include "functions.h"
 #include <cstdlib>
 #include "position.h"
 #include <vector>
 #include "move.h"
 #include "definitions.h"
 #include <iostream>

 vector<Move> moveGen(Position gameBoard) {
	vector<int> piecelist;
	if (gameBoard.toMove == WHITE) piecelist = gameBoard.whitePiecelist;                  //Load the correct piecelist
	else piecelist = gameBoard.blackPiecelist;

	vector<Move> movelist;

	for (unsigned int i = 0; i < piecelist.size(); i++) {
		switch(abs(gameBoard.board[piecelist[i]])) {
		    case NOBOARD:
                break;

            case PAWN: {
				if (gameBoard.board[piecelist[i]-(10 * gameBoard.toMove)] == EMPTY) {
					if ( piecelist[i] >= (56 - (25 * gameBoard.toMove)) && piecelist[i] < (64 - (25 * gameBoard.toMove))) {
					    Move newMove1(piecelist[i], piecelist[i] - 10 * gameBoard.toMove, PAWN, 0, 0, 0, 0, 2); movelist.push_back(newMove1);
					    Move newMove2(piecelist[i], piecelist[i] - 10 * gameBoard.toMove, PAWN, 0, 0, 0, 0, 3); movelist.push_back(newMove2);
					    Move newMove3(piecelist[i], piecelist[i] - 10 * gameBoard.toMove, PAWN, 0, 0, 0, 0, 4); movelist.push_back(newMove3);
					    Move newMove4(piecelist[i], piecelist[i] - 10 * gameBoard.toMove, PAWN, 0, 0, 0, 0, 5); movelist.push_back(newMove4);
					}
					else {
					    Move newMove(piecelist[i], piecelist[i]-(10 * gameBoard.toMove), PAWN, 0);
                        movelist.push_back(newMove);
                        if (gameBoard.board[piecelist[i]-(20 * gameBoard.toMove)] == EMPTY && (56 + (25 * gameBoard.toMove)) <= piecelist[i] && piecelist[i] < (64 + 25 * (gameBoard.toMove))) {
                            Move newMove(piecelist[i], piecelist[i]-(20 * gameBoard.toMove), PAWN, 0, 0, 0, 1, 0);
                            movelist.push_back(newMove);
                        }
					}
				}
                if (gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)] * gameBoard.toMove < 0 && gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)] != NOBOARD) {
                    if ( piecelist[i] >= (56 - (25 * gameBoard.toMove)) && piecelist[i] < (64 - (25 * gameBoard.toMove))) {
                        Move newMove1(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)]), 0, 0, 0, 2); movelist.push_back(newMove1);
					    Move newMove2(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)]), 0, 0, 0, 3); movelist.push_back(newMove2);
					    Move newMove3(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)]), 0, 0, 0, 4); movelist.push_back(newMove3);
					    Move newMove4(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)]), 0, 0, 0, 5); movelist.push_back(newMove4);
					}
                    else {
                        Move newMove(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(11 * gameBoard.toMove)]));
                        movelist.push_back(newMove);
                    }
                }
                if (gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)] * gameBoard.toMove < 0 && gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)] != NOBOARD) {
                    if ( piecelist[i] >= (56 - (25 * gameBoard.toMove)) && piecelist[i] < (64 - (25 * gameBoard.toMove))) {
                        Move newMove1(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)]), 0, 0, 0, 2); movelist.push_back(newMove1);
					    Move newMove2(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)]), 0, 0, 0, 3); movelist.push_back(newMove2);
					    Move newMove3(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)]), 0, 0, 0, 4); movelist.push_back(newMove3);
					    Move newMove4(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)]), 0, 0, 0, 5); movelist.push_back(newMove4);
					}
                    else {
                        Move newMove(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, abs(gameBoard.board[piecelist[i]-(9 * gameBoard.toMove)]));
                        movelist.push_back(newMove);
                    }
                }
                if (piecelist[i]-(9 * gameBoard.toMove) == gameBoard.enPassant) {
                    Move newMove(piecelist[i], piecelist[i]-(9 * gameBoard.toMove), PAWN, PAWN, 1, 0, 0, 0);
                    movelist.push_back(newMove);
                }
                else if (piecelist[i]-(11 * gameBoard.toMove) == gameBoard.enPassant) {
                    Move newMove(piecelist[i], piecelist[i]-(11 * gameBoard.toMove), PAWN, PAWN, 1, 0, 0, 0);
                    movelist.push_back(newMove);
                }
                break;
			}
			case KNIGHT: {
			    for (int j = 0; j < 8; j++) {
			        if (gameBoard.board[piecelist[i] + KNIGHTMOVES[j]] == 0) {
			            Move newMove(piecelist[i], piecelist[i] + KNIGHTMOVES[j], KNIGHT, 0);
			            movelist.push_back(newMove);
			        }
			        else if (gameBoard.board[piecelist[i] + KNIGHTMOVES[j]] * gameBoard.toMove < 0 && gameBoard.board[piecelist[i] + KNIGHTMOVES[j]] != NOBOARD) {
			            Move newMove(piecelist[i], piecelist[i] + KNIGHTMOVES[j], KNIGHT, abs(gameBoard.board[piecelist[i] + KNIGHTMOVES[j]]));
			            movelist.push_back(newMove);
			        }
			    }
			    break;
			}
			case BISHOP: {
			    for (int j=0; j < 4; j++) {
			        int ray = piecelist[i];
			        int done = 0;
			        while (done == 0) {
			            if (gameBoard.board[ray + SLIDEMOVES[j]] == 0) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], BISHOP, 0);
			                movelist.push_back(newMove);
			                ray = ray + SLIDEMOVES[j];
			            }
			            else if (gameBoard.board[ray + SLIDEMOVES[j]] * gameBoard.toMove < 0 && gameBoard.board[ray + SLIDEMOVES[j]] != NOBOARD) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], BISHOP, abs(gameBoard.board[ray + SLIDEMOVES[j]]));
			                movelist.push_back(newMove);
			                done = 1;
			            }
			            else done = 1;
			        }
			    }
			    break;
			}
            case ROOK: {
			    for (int j=4; j < 8; j++) {
			        int ray = piecelist[i];
			        int done = 0;
			        while (done == 0) {
			            if (gameBoard.board[ray + SLIDEMOVES[j]] == 0) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], ROOK, 0);
			                movelist.push_back(newMove);
			                ray = ray + SLIDEMOVES[j];
			            }
			            else if (gameBoard.board[ray + SLIDEMOVES[j]] * gameBoard.toMove < 0 && gameBoard.board[ray + SLIDEMOVES[j]] != NOBOARD) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], ROOK, abs(gameBoard.board[ray + SLIDEMOVES[j]]));
			                movelist.push_back(newMove);
			                done = 1;
			            }
			            else done = 1;
			        }
			    }
			    break;
			}
			case QUEEN: {
			    for (int j=0; j < 8; j++) {
			        int ray = piecelist[i];
			        int done = 0;
			        while (done == 0) {
			            if (gameBoard.board[ray + SLIDEMOVES[j]] == 0) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], QUEEN, 0);
			                movelist.push_back(newMove);
			                ray = ray + SLIDEMOVES[j];
			            }
			            else if (gameBoard.board[ray + SLIDEMOVES[j]] * gameBoard.toMove < 0 && gameBoard.board[ray + SLIDEMOVES[j]] != NOBOARD) {
			                Move newMove(piecelist[i], ray + SLIDEMOVES[j], QUEEN, abs(gameBoard.board[ray + SLIDEMOVES[j]]));
			                movelist.push_back(newMove);
			                done = 1;
			            }
			            else done = 1;
			        }
			    }
			    break;
			}
			case KING:{
			    for (int j = 0; j < 8; j++) {
			        if (gameBoard.board[piecelist[i] + KINGMOVES[j]] == 0) {
			            Move newMove(piecelist[i], piecelist[i] + KINGMOVES[j], KING, 0);
			            movelist.push_back(newMove);
			            if (j == 3) {
			                if(gameBoard.board[piecelist[i] + 2] == 0 && ((gameBoard.toMove == WHITE && gameBoard.castleWK == 1) || (gameBoard.toMove == BLACK && gameBoard.castleBK == 1))) {
			                    Move newMove(piecelist[i], piecelist[i] + 2, KING, 0, 0, 1, 0, 0);
			                    movelist.push_back(newMove);
			                }
			            }
			            if (j == 7) {
                            if(gameBoard.board[piecelist[i] - 2] == 0 && ((gameBoard.toMove == WHITE && gameBoard.castleWQ == 1) || (gameBoard.toMove == BLACK && gameBoard.castleBQ == 1))) {
			                    Move newMove(piecelist[i], piecelist[i] - 2, KING, 0, 0, 2, 0, 0);
			                    movelist.push_back(newMove);
			                }
			            }
			        }
			        else if (gameBoard.board[piecelist[i] + KINGMOVES[j]] * gameBoard.toMove < 0 && gameBoard.board[piecelist[i] + KINGMOVES[j]] != NOBOARD) {
			            Move newMove(piecelist[i], piecelist[i] + KINGMOVES[j], KING, abs(gameBoard.board[piecelist[i] + KINGMOVES[j]]));
			            movelist.push_back(newMove);
			        }
			    }
			    break;
			}
		}
	}
	return movelist;
}