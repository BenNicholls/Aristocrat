/*********************************************************************************
                                    Aristocrat
								  Position Class
                                   position.cpp
                            Last Updated Nov 28, 2010

NOTES
  - Castling, promotion, and en passant have been implemented fully, but only
    minimally tested
  - Updating the piecelists in the case of castling and enpassanting and stuff was
    retarded. Not hard or anything, but man, it takes like 60 lines of code. Should
    find a more concise way of doing that.
**********************************************************************************/

#include <iostream>
#include <vector>
#include "position.h"
#include "definitions.h"
#include "move.h"
#include "functions.h"
#include "movelist.h"

//Position setup function. Entering "custom" sets up the position are diagrammed in Position::customSetup()
//otherwise, it sends the input parameter to be parsed as a fen string. 
Position::Position(string setup){
	if (setup == "custom") customSetup();
	else fenParse(setup);
}

//No string included? Initial position set up then.
Position::Position(){
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fenParse(fen);
}

void Position::customSetup() {
    toMove = BLACK;
    enPassant = 0;
    castleWK = 1;
    castleWQ = 1;
    castleBK = 1;
    castleBQ = 1;
    fiftyMove = 0;
	totalMoves = 0;
	
	//Clears the board, sets up NOBOARD zones.
	for (int i = 0; i < 120; i++) {
		if (i/10 < 2 || i/10 > 9 || i%10 == 0 || i%10 == 9) board[i] = NOBOARD;
		else board[i] = EMPTY;
	}
	
//	Nice visual way to set up the board. Not too shabby!
    int boardsetup[64] = { -ROOK, EMPTY, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK, //8
                           -PAWN, PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN,          //7
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //6
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //5
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //4
                           EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,         //3
                           PAWN, -PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, 			    //2
                           ROOK, EMPTY, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };       //1
// 							a 		b 		c 		d 	 e 		f 		g 		h

	//Throws the above position onto the board
    int loadCount = 0;
    for (int i = 20; i < 99; i++) {
		if (board[i] != NOBOARD) {
            board[i] = boardsetup[loadCount];
			if (boardsetup[loadCount] > 0) whitePiecelist.push_back(i); 
			else if (boardsetup[loadCount] < 0) blackPiecelist.push_back(i);
            if (boardsetup[loadCount] == KING) whiteKing = i;
            else if (boardsetup[loadCount] == -KING) blackKing = i;
            loadCount++;
        }
    }
}

//Parses the fen string, sets up the position. WARNING: no checks are currently
//done to validate the fen string. Make sure it is good!
void Position::fenParse(string fen){

	//Clears the board, sets up NOBOARD zones.
	for (int i = 0; i < 120; i++) {
		if (i/10 < 2 || i/10 > 9 || i%10 == 0 || i%10 == 9) board[i] = NOBOARD;
		else board[i] = EMPTY;
	}
	
	//Holds the cut up fen string
	string FenPieces[13];
	
	//This block cuts the fen string into segments
	int count = 0;
	string temp = "";
	for (unsigned int i = 0; i < fen.size(); i++){
		if (fen[i] == ' ' || fen[i] == '/'){
			FenPieces[count] = temp;
			temp = "";
			count++;
		}
		else temp += fen[i];
	}
	FenPieces[12] = temp;

	///This block places the pieces into the board[120] array
	for (unsigned int i = 0; i < 8; i++) {
		int pos = 0;
		for (unsigned int j = 0; j < FenPieces[i].size(); j++) {
			int space = 21+i*10+pos;
			switch(FenPieces[i][j]){
				case 'r': board[space] = -ROOK; blackPiecelist.push_back(space); pos++; break;
				case 'n': board[space] = -KNIGHT; blackPiecelist.push_back(space); pos++; break;
				case 'b': board[space] = -BISHOP; blackPiecelist.push_back(space); pos++; break;
				case 'q': board[space] = -QUEEN; blackPiecelist.push_back(space); pos++; break;
				case 'k': board[space] = -KING; blackPiecelist.push_back(space); blackKing = space; pos++; break;
				case 'p': board[space] = -PAWN; blackPiecelist.push_back(space); pos++; break;
				case 'R': board[space] = ROOK; whitePiecelist.push_back(space); pos++; break;
				case 'N': board[space] = KNIGHT; whitePiecelist.push_back(space); pos++; break;
				case 'B': board[space] = BISHOP; whitePiecelist.push_back(space); pos++; break;
				case 'Q': board[space] = QUEEN; whitePiecelist.push_back(space); pos++; break;
				case 'K': board[space] = KING; whitePiecelist.push_back(space); whiteKing = space; pos++; break;
				case 'P': board[space] = PAWN; whitePiecelist.push_back(space); pos++; break;
				case '1': pos += 1; break;
				case '2': pos += 2; break;
				case '3': pos += 3; break;
				case '4': pos += 4; break;
				case '5': pos += 5; break;
				case '6': pos += 6; break;
				case '7': pos += 7; break;
				case '8': pos += 8; break; 
			}
		}
	}

	//Assesses whos turn it is.
	if (FenPieces[8][0] == 'w') toMove = 1;
	else toMove = -1;

	//Castling
	castleWK = false;
	castleWQ = false;
	castleBK = false;
	castleBQ = false;
	for (unsigned int i = 0; i < FenPieces[9].size(); i++){
		switch(FenPieces[9][i]){
			case 'K': castleWK = true; break;
			case 'Q': castleWQ = true; break;
			case 'k': castleBK = true; break;
			case 'q': castleBQ = true; break;
		}
	}

	//Enpassant space
	if (FenPieces[10][0] != '-') enPassant = fromAlgebraic(FenPieces[10]);

	fiftyMove = atoi(FenPieces[11].c_str());
	totalMoves = atoi(FenPieces[12].c_str());

}

//Outputs a fancy schmancy board, all ASCII and everything.
void Position::output() {
    cout << "  _ _ _ _ _ _ _ _ " << endl << "8|";
    for (int i=21; i < 99; i++) {
        if (board[i] != NOBOARD) {
            if (board[i] < 0) cout << BLACKPIECEOUTPUT[abs(board[i])] << "|";
            else cout << WHITEPIECEOUTPUT[board[i]] << "|";
            if (i%10 == 8 && i != 98) cout << endl << "  - - - - - - - -" << endl << (7 - (i - 20)/10) << "|";
        }
    }
    cout << endl << "  - - - - - - - -" << endl << "  a b c d e f g h" << endl;
    if (toMove == WHITE) cout << "White to move." << endl;
    else cout << "Black to move." << endl;
}

//Moves a piece. It's fucking fantastical. Returns false if move turned out to be illegal.
bool Position::movePiece(Move theMove) {
    board[theMove.toSpace] = board[theMove.fromSpace];
    board[theMove.fromSpace] = 0;

	if (toMove == WHITE) {
		for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
			if (whitePiecelist[i] == theMove.fromSpace) whitePiecelist[i] = theMove.toSpace;
		}
		for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
			if (blackPiecelist[i] == theMove.toSpace) blackPiecelist[i] = 0;
		}
		if (theMove.enPassant == 1) {
		    for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
                if (blackPiecelist[i] == theMove.toSpace + 10) blackPiecelist[i] = 0;
            }
		}
		if (theMove.castle == 1) {
		    castleWK = 0;
		    board[98] = EMPTY;
		    board[96] = ROOK;
		    for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
		        if (whitePiecelist[i] == 98) whitePiecelist[i] = 96;
		    }
		}
		if (theMove.castle == 2) {
		    castleWQ = 0;
		    board[91] = EMPTY;
		    board[94] = ROOK;
		    for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
		        if (whitePiecelist[i] == 91) whitePiecelist[i] = 94;
		    }
        }
        if (theMove.fromSpace == whiteKing) whiteKing = theMove.toSpace;
	}
	else {
		for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
			if (blackPiecelist[i] == theMove.fromSpace) blackPiecelist[i] = theMove.toSpace;
		}
		for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
            if (whitePiecelist[i] == theMove.toSpace) whitePiecelist[i] = 0;
		}
		if (theMove.enPassant == 1) {
		    for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
                if (whitePiecelist[i] == theMove.toSpace - 10) whitePiecelist[i] = 0;
            }
		}
        if (theMove.castle == 1) {
		    castleBK = 0;
		    board[28] = EMPTY;
		    board[26] = -ROOK;
		    for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
		        if (blackPiecelist[i] == 28) blackPiecelist[i] = 26;
		    }
		}
		if (theMove.castle == 2) {
		    castleBQ = 0;
		    board[21] = EMPTY;
		    board[24] = -ROOK;
		    for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
		        if (blackPiecelist[i] == 21) blackPiecelist[i] = 24;
		    }
        }
        if (theMove.fromSpace == blackKing) blackKing = theMove.toSpace;
	}
    if (theMove.enPassant == 1) board[theMove.toSpace + (10 * toMove)] = EMPTY; //Kills the en Passanted pawn, i hope
    if (theMove.jump == 1) enPassant = theMove.toSpace + (10 * toMove); //If there is en passanting to be done, we will know
    else enPassant = 0;
    if (theMove.promotion != 0) board[theMove.toSpace] = theMove.promotion * toMove;
	
	bool check = inCheck();
	toMove = -toMove;
	if (theMove.capture == 0 && theMove.piece != PAWN) fiftyMove++;
	else fiftyMove = 0;

	return check;
}

void Position::removePiece(int space, int side) {
	
	if (side == WHITE) {
		for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
			if (whitePiecelist[i] == space) {
				whitePiecelist[i] = NOBOARD;
				break;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
			if (blackPiecelist[i] == space) {
				blackPiecelist[i] = NOBOARD;
				break;
			}
		}
	}
}

void Position::updatePiece(int fromSpace, int toSpace, int side) {
	
	if (side == WHITE) {
		for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
			if (whitePiecelist[i] == fromSpace) {
				whitePiecelist[i] = toSpace;
				break;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
			if (blackPiecelist[i] == fromSpace) {
				blackPiecelist[i] = toSpace;
				break;
			}
		}
	}
}

bool Position::doMove(Move theMove) {
	
	//First up: move the piece on the board, update piecelist
	board[theMove.toSpace] = theMove.piece;
	board[theMove.fromSpace] = EMPTY;
	updatePiece(theMove.fromSpace, theMove.toSpace, toMove);

	//If it is a capture, let's delete the dead guy from the proper piecelist
	if (theMove.capture != 0) {
		//For enpassant captures
		if(theMove.enPassant == true) {
			removePiece(enPassant, -toMove);
			board[enPassant + 10*toMove] == EMPTY;
		}
		else removePiece(theMove.toSpace, -toMove);
	}

	//For promotions, we must bring a piece to LIFE!
	if (theMove.promotion != 0) board[theMove.toSpace] = -theMove.promotion;
	
	//Castling. Hooray. Move the rooks, update piecelists.
	if (theMove.castle != 0) {
		if (theMove.castle == 1) {
			if (toMove == WHITE) {
				board[98] = EMPTY; board[96] = ROOK;
				updatePiece(98, 96, WHITE);
			}
			else {
				board[28] = EMPTY; board[26] = -ROOK;
				updatePiece(28, 26, BLACK);
			}
		}
		else {
			if (toMove == WHITE) {
				board[91] = EMPTY; board[94] = ROOK;
				updatePiece(91, 94, WHITE);
			}
			else {
				board[21] = EMPTY; board[24] = -ROOK;
				updatePiece(21, 24, BLACK);
			}
		}
	}

	//Update king locations
	if (abs(theMove.piece) == KING) {
		if (toMove == WHITE) {
			whiteKing = theMove.toSpace;
			castleWK = false; castleWQ = false;
		}
		else {
			blackKing = theMove.toSpace;
			castleBK = false; castleBQ = false;
		}
	}

	//Update castling rights for rook moves
	if (theMove.piece = ROOK) {
		if (theMove.fromSpace == 91) castleWQ = false;
		else if (theMove.fromSpace == 98) castleWK = false;
	}
	else if (theMove.piece == -ROOK) {
		if (theMove.fromSpace == 21) castleBQ = false;
		else if (theMove.fromSpace == 28) castleBK = false;
	}

	//Adjust fifty move rule counter
	if (abs(theMove.piece) == PAWN || theMove.capture != 0) fiftyMove = 0;
	else fiftyMove++;

	if (toMove == BLACK) totalMoves++;
	bool check = inCheck();
	toMove = -toMove;

	return check;
}



bool Position::inCheck() {
	int kingSquare;
	if (toMove == WHITE) kingSquare = whiteKing;
	else kingSquare = blackKing;
	return isAttacked(kingSquare);
}

bool Position::isAttacked(int square) {


	//attacked by pawn?
	if (board[square - 11*toMove] == -PAWN*toMove || board[square - 9*toMove] == -PAWN*toMove) return true;

	//knight?
	for (unsigned int j = 0; j < 8; j++) {
		if (board[square + KNIGHTMOVES[j]] == -KNIGHT*toMove) return true;
	}

	//diagonals?
	for (unsigned int i = 0; i < 4; i++) {
		bool ray = true;
		int currentSquare = square;
		while (ray) {
			currentSquare += SLIDEMOVES[i];
			if (board[currentSquare] != NOBOARD) {
				if (board[currentSquare] == -BISHOP*toMove || board[currentSquare] == -QUEEN*toMove) return true;
				else if (board[currentSquare] != EMPTY) ray = false;
			}
			else ray = false;
		}
	}

	//Horizontalists
	for (unsigned int i = 4; i < 8; i++) {
		bool ray = true;
		int currentSquare = square;
		while (ray) {
			currentSquare += SLIDEMOVES[i];
			if (board[currentSquare] != NOBOARD) {
				if (board[currentSquare] == -ROOK*toMove || board[currentSquare] == -QUEEN*toMove) return true;
				else if (board[currentSquare] != EMPTY) ray = false;
			}
			else ray = false;
		}
	}
	return false;
}

//Generates moves for a pre-made movelist.
void Position::generateMoves(Movelist &Moves) {

	vector<int> piecelist;
	Moves.old_fiftyMove = fiftyMove;
	Moves.old_enPassant = enPassant;
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
						if (toMove == WHITE) {
							if (j == 5 && castleWK == true) {
								if (board[97] == EMPTY) {
									if (!isAttacked(97)) Moves.add_castle(1, WHITE);
								}
							}
							else if (j == 7 && castleWQ == true) {
								if (board[93] == EMPTY && board[92] == EMPTY) {
									if (!isAttacked(93) && !isAttacked(92)) Moves.add_castle(2, WHITE);
								}
							}
						}
						else {
							if (j == 5 && castleBK == true) {
								if (board[27] == EMPTY) {
									if (!isAttacked(27)) Moves.add_castle(1, BLACK);
								}
							}
							else if (j == 7 && castleBQ == true) {
								if (board[23] == EMPTY && board[22] == EMPTY) {
									if (!isAttacked(23) && !isAttacked(22)) Moves.add_castle(2, BLACK);
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


	
	
