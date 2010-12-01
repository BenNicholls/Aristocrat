/*********************************************************************************
                                    Aristocrat
								  Position Class
                                   position.cpp

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
    toMove = WHITE;
    enPassant = 0;
    castleWK = true;
    castleWQ = true;
    castleBK = true;
    castleBQ = true;
    fiftyMove = 0;
	totalMoves = 0;
	halfMoves = 0;
	
	//Clears the board, sets up NOBOARD zones.
	for (int i = 0; i < 120; i++) {
		if (i/10 < 2 || i/10 > 9 || i%10 == 0 || i%10 == 9) board[i] = NOBOARD;
		else board[i] = EMPTY;
	}
	
//	Nice visual way to set up the board. Not too shabby!
    int boardsetup[64] = { -ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK, //8
                           -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN,          //7
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //6
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //5
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,          //4
                           EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,         //3
                           PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, 			    //2
                           ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };       //1
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
	else enPassant = 0;

	fiftyMove = atoi(FenPieces[11].c_str());
	totalMoves = atoi(FenPieces[12].c_str());
	halfMoves = 0;
}

//Outputs a fancy schmancy board, all ASCII and everything.
void Position::output() {
    cout << "  ------------------------------- " << endl << "8|";
    for (int i=21; i < 99; i++) {
        if (board[i] != NOBOARD) {
            if (board[i] < 0) cout << " " << BLACKPIECEOUTPUT[abs(board[i])] << " |";
            else cout << " " << WHITEPIECEOUTPUT[board[i]] << " |";
            if (i%10 == 8 && i != 98) cout << endl << " |---+---+---+---+---+---+---+---|" << endl << (7 - (i - 20)/10) << "|";
        }
    }
    cout << endl << "  -------------------------------" << endl << "   a   b   c   d   e   f   g   h" << endl;
    if (toMove == WHITE) cout << "White to move." << endl;
    else cout << "Black to move." << endl;
}

void Position::outputDetails() {
	output();
	cout << endl;
	cout << "White King is on " << toAlgebraic(whiteKing) << endl;
	cout << "Black King is on " << toAlgebraic(blackKing) << endl;
	cout << "Castling Rights: ";
	if (castleWK) cout << "K";
	if (castleWQ) cout << "Q";
	if (castleBK) cout << "k";
	if (castleBQ) cout << "q";
	cout << endl;
	cout << "Fiftymove counter: " << fiftyMove << ". Total moves: " << totalMoves << endl;
	cout << "White pieces are located on: ";
	for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
		if (whitePiecelist[i] != NOBOARD) cout << toAlgebraic(whitePiecelist[i]) << " ";
	}
	cout << endl;
	cout << "Black pieces are located on: ";
	for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
		if (blackPiecelist[i] != NOBOARD) cout << toAlgebraic(blackPiecelist[i]) << " ";
	}
	cout << endl;
	if (enPassant != 0) cout << "En Passant square is: " << toAlgebraic(enPassant) << endl;
	else cout << "There is no en passant square." << endl;
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
void Position::addPiece(int space, int side) {
	
	if (side == WHITE) {
		for (unsigned int i = 0; i < whitePiecelist.size(); i++) {
			if (whitePiecelist[i] == NOBOARD) {
				whitePiecelist[i] = space;
				break;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < blackPiecelist.size(); i++) {
			if (blackPiecelist[i] == NOBOARD) {
				blackPiecelist[i] = space;
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

bool Position::doMove(Move &theMove) {
	
	//save history
	updateHistory();

	//First up: move the piece on the board, update piecelist
	board[theMove.toSpace] = theMove.piece;
	board[theMove.fromSpace] = EMPTY;
	updatePiece(theMove.fromSpace, theMove.toSpace, toMove);

	//If it is a capture, let's delete the dead guy from the proper piecelist
	if (theMove.capture != 0) {
		//For enpassant captures
		if(theMove.enPassant == true) {
			removePiece(enPassant + 10*toMove, -toMove);
			board[enPassant + 10*toMove] = EMPTY;
		}
		else removePiece(theMove.toSpace, -toMove);

		//If a rook is captured, update castle info.
		if(abs(theMove.capture) == ROOK) {
			if (castleWK && theMove.toSpace == 98) castleWK = false;
			else if (castleWQ && theMove.toSpace == 91) castleWQ = false;
			else if (castleBK && theMove.toSpace == 28) castleBK = false;
			else if (castleBQ && theMove.toSpace == 21) castleBQ = false;
		}
	}

	//Speaking of en passant captures...
	if (theMove.jump) enPassant = theMove.fromSpace - 10*toMove;
	else enPassant = 0;

	//For promotions, we must bring a piece to LIFE!
	if (theMove.promotion != 0) board[theMove.toSpace] = theMove.promotion*toMove;
	
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
	if (theMove.piece == ROOK) {
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
	halfMoves++;
	bool check = inCheck();
	toMove = -toMove;

	//update moves made and history
	movesMade.add(theMove);
	return check;
}

void Position::undoMove() {
	Move theMove;
	theMove = movesMade.list[movesMade.totalMoves - 1];
	toMove = -toMove;

	//Move piece back
	board[theMove.fromSpace] = theMove.piece;
	updatePiece(theMove.toSpace, theMove.fromSpace, toMove);

	//If a piece was captured, put it back, you!
	if (theMove.capture != 0) {
		//If it was an en passant capture
		if (theMove.enPassant) {
			board[theMove.toSpace + 10*toMove] = -PAWN*toMove;
			addPiece(theMove.toSpace +10*toMove, -toMove);
			board[theMove.toSpace] = EMPTY;
		}
		else {
			board[theMove.toSpace] = theMove.capture;
			addPiece(theMove.toSpace, -toMove);
		}
	}
	else board[theMove.toSpace] = EMPTY;

	//reupdate king positions
	if (abs(theMove.piece) == KING){
		if (theMove.piece == KING) whiteKing = theMove.fromSpace;
		else blackKing = theMove.fromSpace;
	}

	if (theMove.castle != 0) {
		if (toMove == WHITE) {
			if (theMove.castle == 1) {
				board[96] = EMPTY; board[98] = ROOK;
				updatePiece(96, 98, WHITE);
			}
			else {
				board[94] = EMPTY; board[91] = ROOK;
				updatePiece(94, 91, WHITE);
			}
		}
		else {
			if (theMove.castle == 1) {
				board[26] = EMPTY; board[28] = -ROOK;
				updatePiece(26, 28, BLACK);
			}
			else {
				board[24] = EMPTY; board[21] = -ROOK;
				updatePiece(24, 21, BLACK);
			}
		}
	}

	if (toMove == BLACK) totalMoves--;
	halfMoves--;
	castleWK = WKhistory[halfMoves];
	castleBK = BKhistory[halfMoves];
	castleWQ = WQhistory[halfMoves];
	castleBQ = BQhistory[halfMoves];
	enPassant = enPassantHistory[halfMoves];
	fiftyMove = fiftyMoveHistory[halfMoves];

	movesMade.remove_last();

}

void Position::updateHistory() {
	WKhistory[halfMoves] = castleWK;
	BKhistory[halfMoves] = castleBK;
	WQhistory[halfMoves] = castleWQ;
	BQhistory[halfMoves] = castleBQ;
	enPassantHistory[halfMoves] = enPassant;
	fiftyMoveHistory[halfMoves] = fiftyMove;
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

	//king?
	for (unsigned int j = 0; j < 8; j++) {
		if (board[square + SLIDEMOVES[j]] == -KING*toMove) return true;
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


	
	
