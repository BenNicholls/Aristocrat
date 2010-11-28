/*********************************************************************************
                                    Aristocrat
								  Position Class
                                   position.cpp
                            Last Updated March 30, 2009

NOTES
  - Castling, promotion, and en passant have been implemented fully, but only
    minimally tested
  - Updating the piecelists in the case of castling and enpassanting and stuff was
    retarded. Not hard or anything, but man, it takes like 60 lines of code. Should
    find a more concise way of doing that.
**********************************************************************************/

#include <iostream>
#include "position.h"
#include "definitions.h"
#include "move.h"
#include <vector>
#include "functions.h"

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

bool Position::inCheck() {
	int kingSquare;
	if (toMove == WHITE) kingSquare = whiteKing;
	else kingSquare = blackKing;

	//attacked by pawn?
	if (board[kingSquare - 11*toMove] == -PAWN*toMove || board[kingSquare - 9*toMove] == -PAWN*toMove) return true;

	//knight?
	for (unsigned int j = 0; j < 8; j++) {
		if (board[kingSquare + KNIGHTMOVES[j]] == -KNIGHT*toMove) return true;
	}

	//diagonals?
	for (unsigned int i = 0; i < 4; i++) {
		bool ray = true;
		int currentSquare = kingSquare;
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
		int currentSquare = kingSquare;
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

