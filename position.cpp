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
	fenParse(setup);
}

//No string included? Initial position set up then.
Position::Position(){
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fenParse(fen);
}

//Parses the fen string, sets up the position. WARNING: no checks are currently
//done to validate the fen string. Make sure it is good!
void Position::fenParse(string fen){

	pieceListTotals[0][PAWN] = 0;
	pieceListTotals[0][KNIGHT] = 0;
	pieceListTotals[0][BISHOP] = 0;
	pieceListTotals[0][ROOK] = 0;
	pieceListTotals[0][QUEEN] = 0;
	pieceListTotals[1][PAWN] = 0;
	pieceListTotals[1][KNIGHT] = 0;
	pieceListTotals[1][BISHOP] = 0;
	pieceListTotals[1][ROOK] = 0;
	pieceListTotals[1][QUEEN] = 0;

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
				case 'r': board[space] = -ROOK; updatePieceList(0, space, -ROOK); pos++; break;
				case 'n': board[space] = -KNIGHT; updatePieceList(0, space, -KNIGHT); pos++; break;
				case 'b': board[space] = -BISHOP; updatePieceList(0, space, -BISHOP); pos++; break;
				case 'q': board[space] = -QUEEN; updatePieceList(0, space, -QUEEN); pos++; break;
				case 'k': board[space] = -KING; blackKing = space; pos++; break;
				case 'p': board[space] = -PAWN; updatePieceList(0, space, -PAWN); pos++; break;
				case 'R': board[space] = ROOK; updatePieceList(0, space, ROOK); pos++; break;
				case 'N': board[space] = KNIGHT; updatePieceList(0, space, KNIGHT); pos++; break;
				case 'B': board[space] = BISHOP; updatePieceList(0, space, BISHOP); pos++; break;
				case 'Q': board[space] = QUEEN; updatePieceList(0, space, QUEEN); pos++; break;
				case 'K': board[space] = KING; whiteKing = space; pos++; break;
				case 'P': board[space] = PAWN; updatePieceList(0, space, PAWN); pos++; break;
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

	keySeed = 80;

	blackKey = genRandomKey();

	for (unsigned int i = 0; i < 64; i++) {
		whitePawnKeys[i] = genRandomKey();
		whiteKnightKeys[i] = genRandomKey();
		whiteBishopKeys[i] = genRandomKey();
		whiteRookKeys[i] = genRandomKey();
		whiteQueenKeys[i] = genRandomKey();
		whiteKingKeys[i] = genRandomKey();
		blackPawnKeys[i] = genRandomKey();
		blackKnightKeys[i] = genRandomKey();
		blackBishopKeys[i] = genRandomKey();
		blackRookKeys[i] = genRandomKey();
		blackQueenKeys[i] = genRandomKey();
		blackKingKeys[i] = genRandomKey();
		if (i < 8) enpassantKeys[i] = genRandomKey();
		if (i < 4) castleKeys[i] = genRandomKey();
	}

	generateHash();
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
	for (unsigned int i = 1; i < 6; i++) {
		for (unsigned int j = 0; j < pieceListTotals[0][i]; j++) cout << toAlgebraic(pieceLists[0][i][j]) << " ";
	}
	cout << endl;
	cout << "Black pieces are located on: ";
	for (unsigned int i = 1; i < 6; i++) {
		for (unsigned int j = 0; j < pieceListTotals[1][i]; j++) cout << toAlgebraic(pieceLists[1][i][j]) << " ";
	}
	cout << endl;
	if (enPassant != 0) cout << "En Passant square is: " << toAlgebraic(enPassant) << endl;
	else cout << "There is no en passant square." << endl;
	cout << "Hashkey: " << hash << endl;
}

void Position::updatePieceList(int fromSpace, int toSpace, int piece) {
	if (abs(piece) != KING) {
		int fromIndex, side;
		if (fromSpace != 0) fromIndex = pieceIndexTable[fromSpace];
		if (piece > 0) side = 0;
		else side = 1;
		if (fromSpace != 0) {
				pieceListTotals[side][abs(piece)]--;
				pieceLists[side][abs(piece)][fromIndex] = pieceLists[side][abs(piece)][pieceListTotals[side][abs(piece)]];
				pieceIndexTable[pieceLists[side][abs(piece)][fromIndex]] = fromIndex;
		}
		if (toSpace != 0) {
				pieceLists[side][abs(piece)][pieceListTotals[side][abs(piece)]] = toSpace;
				pieceIndexTable[toSpace] = pieceListTotals[side][abs(piece)];
				pieceListTotals[side][abs(piece)]++;
		}
	}
}

void Position::updatePieceHash(int fromSpace, int toSpace, int piece) {
	
	int from64;
	int to64;

	if (fromSpace != 0) from64 = 8*(fromSpace/10 - 2) + fromSpace%10 - 1;
	if (toSpace != 0) to64 = 8*(toSpace/10 - 2) + toSpace%10 - 1;

	switch (piece) {
		case PAWN:
			if (fromSpace != 0) hash = hash ^ whitePawnKeys[from64];
			if (toSpace != 0) hash = hash ^ whitePawnKeys[to64];
			break;
		case KNIGHT:
			if (fromSpace != 0) hash = hash ^ whiteKnightKeys[from64];
			if (toSpace != 0) hash = hash ^ whiteKnightKeys[to64];
			break;
		case BISHOP:
			if (fromSpace != 0) hash = hash ^ whiteBishopKeys[from64];
			if (toSpace != 0) hash = hash ^ whiteBishopKeys[to64];
			break;
		case ROOK:
			if (fromSpace != 0) hash = hash ^ whiteRookKeys[from64];
			if (toSpace != 0) hash = hash ^ whiteRookKeys[to64];
			break;
		case QUEEN:
			if (fromSpace != 0) hash = hash ^ whiteQueenKeys[from64];
			if (toSpace != 0) hash = hash ^ whiteQueenKeys[to64];
			break;
		case KING:
			if (fromSpace != 0) hash = hash ^ whiteKingKeys[from64];
			if (toSpace != 0) hash = hash ^ whiteKingKeys[to64];
			break;
		case -PAWN:
			if (fromSpace != 0) hash = hash ^ blackPawnKeys[from64];
			if (toSpace != 0) hash = hash ^ blackPawnKeys[to64];
			break;
		case -KNIGHT:
			if (fromSpace != 0) hash = hash ^ blackKnightKeys[from64];
			if (toSpace != 0) hash = hash ^ blackKnightKeys[to64];
			break;
		case -BISHOP:
			if (fromSpace != 0) hash = hash ^ blackBishopKeys[from64];
			if (toSpace != 0) hash = hash ^ blackBishopKeys[to64];
			break;
		case -ROOK:
			if (fromSpace != 0) hash = hash ^ blackRookKeys[from64];
			if (toSpace != 0) hash = hash ^ blackRookKeys[to64];
			break;
		case -QUEEN:
			if (fromSpace != 0) hash = hash ^ blackQueenKeys[from64];
			if (toSpace != 0) hash = hash ^ blackQueenKeys[to64];
			break;
		case -KING:
			if (fromSpace != 0) hash = hash ^ blackKingKeys[from64];
			if (toSpace != 0) hash = hash ^ blackKingKeys[to64];
			break;
	}
}

bool Position::doMove(Move &theMove) {
	
	//save history
	updateHistory();

	//First up: move the piece on the board, update piecelist
	board[theMove.toSpace] = theMove.piece;
	board[theMove.fromSpace] = EMPTY;
	updatePieceList(theMove.fromSpace, theMove.toSpace, theMove.piece);
	updatePieceHash(theMove.fromSpace, theMove.toSpace, theMove.piece);

	//If it is a capture, let's delete the dead guy from the proper piecelist
	if (theMove.capture != 0) {
		//For enpassant captures
		if(theMove.enPassant == true) {
			updatePieceList(enPassant + 10*toMove, 0, -PAWN*toMove);
			updatePieceHash(enPassant + 10*toMove, 0, theMove.capture);
			board[enPassant + 10*toMove] = EMPTY;
		}
		else {
			updatePieceList(theMove.toSpace, 0, theMove.capture);
			updatePieceHash(theMove.toSpace, 0, theMove.capture);
		}

		//If a rook is captured, update castle info.
		if(abs(theMove.capture) == ROOK) {
			if (castleWK && theMove.toSpace == 98) {
				castleWK = false;
				hash = hash ^ castleKeys[0];
			}
			else if (castleWQ && theMove.toSpace == 91) {
				castleWQ = false;
				hash = hash ^ castleKeys[1];
			}
			else if (castleBK && theMove.toSpace == 28) {
				castleBK = false;
				hash = hash ^ castleKeys[2];
			}
			else if (castleBQ && theMove.toSpace == 21) {
				hash = hash ^ castleKeys[3];
				castleBQ = false;
			}
		}
	}

	//Speaking of en passant captures...
	if (enPassant != 0) hash = hash ^ enpassantKeys[enPassant%10 - 1];
	if (theMove.jump) {
		enPassant = theMove.fromSpace - 10*toMove;
		hash = hash ^ enpassantKeys[enPassant%10 - 1];
	}
	else enPassant = 0;

	//For promotions, we must bring a piece to LIFE!
	if (theMove.promotion != 0) {
		board[theMove.toSpace] = theMove.promotion*toMove;
		updatePieceList(theMove.toSpace, 0, theMove.piece);
		updatePieceList(0, theMove.toSpace, theMove.promotion*toMove);
		updatePieceHash(0, theMove.toSpace, theMove.promotion*toMove);
		updatePieceHash(theMove.toSpace, 0, theMove.piece);
	}
	
	//Castling. Hooray. Move the rooks, update piecelists.
	if (theMove.castle != 0) {
		if (theMove.castle == 1) {
			if (toMove == WHITE) {
				board[98] = EMPTY; board[96] = ROOK;
				updatePieceList(98, 96, ROOK);
				updatePieceHash(98, 96, ROOK);
			}
			else {
				board[28] = EMPTY; board[26] = -ROOK;
				updatePieceList(28, 26, -ROOK);
				updatePieceHash(28, 26, -ROOK);
			}
		}
		else {
			if (toMove == WHITE) {
				board[91] = EMPTY; board[94] = ROOK;
				updatePieceList(91, 94, ROOK);
				updatePieceHash(91, 94, ROOK);
			}
			else {
				board[21] = EMPTY; board[24] = -ROOK;
				updatePieceList(21, 24, -ROOK);
				updatePieceHash(21, 24, -ROOK);
			}
		}
	}

	//Update king locations
	if (abs(theMove.piece) == KING) {
		if (toMove == WHITE) {
			whiteKing = theMove.toSpace;
			if (castleWK) {
				castleWK = false; 
				hash = hash ^ castleKeys[0];
			}
			if (castleWQ) {
				castleWQ = false;
				hash = hash ^ castleKeys[1];
			}
		}
		else {
			blackKing = theMove.toSpace;
			if (castleBK) {
				castleBK = false;
				hash = hash ^ castleKeys[2];
			}
			if (castleBQ) {
				castleBQ = false;
				hash = hash ^ castleKeys[3];
			}
		}
	}

	//Update castling rights for rook moves
	if (theMove.piece == ROOK) {
		if (theMove.fromSpace == 91 && castleWQ) {
			castleWQ = false;
			hash = hash ^ castleKeys[1];
		}
		else if (theMove.fromSpace == 98 && castleWK){
			castleWK = false;
			hash = hash ^ castleKeys[0];
		}
	}
	else if (theMove.piece == -ROOK) {
		if (theMove.fromSpace == 21 && castleBQ){
			castleBQ = false;
			hash = hash ^ castleKeys[3];
		}
		else if (theMove.fromSpace == 28 && castleBK){
			castleBK = false;
			hash = hash ^ castleKeys[2];
		}
	}

	//Adjust fifty move rule counter
	if (abs(theMove.piece) == PAWN || theMove.capture != 0) fiftyMove = 0;
	else fiftyMove++;

	if (toMove == BLACK) totalMoves++;
	halfMoves++;
	bool check = inCheck();
	toMove = -toMove;
	hash = hash ^ blackKey;

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
	updatePieceList(theMove.toSpace, theMove.fromSpace, theMove.piece);

	//If a piece was captured, put it back, you!
	if (theMove.capture != 0) {
		//If it was an en passant capture
		if (theMove.enPassant) {
			board[theMove.toSpace + 10*toMove] = -PAWN*toMove;
			updatePieceList(0, theMove.toSpace +10*toMove, -PAWN*toMove);
			board[theMove.toSpace] = EMPTY;
		}
		else {
			board[theMove.toSpace] = theMove.capture;
			updatePieceList(0, theMove.toSpace, theMove.capture);
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
				updatePieceList(96, 98, ROOK);
			}
			else {
				board[94] = EMPTY; board[91] = ROOK;
				updatePieceList(94, 91, ROOK);
			}
		}
		else {
			if (theMove.castle == 1) {
				board[26] = EMPTY; board[28] = -ROOK;
				updatePieceList(26, 28, -ROOK);
			}
			else {
				board[24] = EMPTY; board[21] = -ROOK;
				updatePieceList(24, 21, -ROOK);
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
	hash = hashHistory[halfMoves];
	movesMade.remove_last();
}

void Position::updateHistory() {
	WKhistory[halfMoves] = castleWK;
	BKhistory[halfMoves] = castleBK;
	WQhistory[halfMoves] = castleWQ;
	BQhistory[halfMoves] = castleBQ;
	enPassantHistory[halfMoves] = enPassant;
	fiftyMoveHistory[halfMoves] = fiftyMove;
	hashHistory[halfMoves] = hash;
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

void Position::generateHash() {
	hash = 0;
	for (unsigned int i = 0; i < pieceListTotals[0][PAWN]; i++) hash = hash ^ whitePawnKeys[pieceLists[0][PAWN][i]];
	for (unsigned int i = 0; i < pieceListTotals[0][KNIGHT]; i++) hash = hash ^ whiteKnightKeys[pieceLists[0][KNIGHT][i]];
	for (unsigned int i = 0; i < pieceListTotals[0][BISHOP]; i++) hash = hash ^ whiteBishopKeys[pieceLists[0][BISHOP][i]];
	for (unsigned int i = 0; i < pieceListTotals[0][ROOK]; i++) hash = hash ^ whiteRookKeys[pieceLists[0][ROOK][i]];
	for (unsigned int i = 0; i < pieceListTotals[0][QUEEN]; i++) hash = hash ^ whiteQueenKeys[pieceLists[0][QUEEN][i]];
	for (unsigned int i = 0; i < pieceListTotals[1][PAWN]; i++) hash = hash ^ blackPawnKeys[pieceLists[1][PAWN][i]];
	for (unsigned int i = 0; i < pieceListTotals[1][KNIGHT]; i++) hash = hash ^ blackKnightKeys[pieceLists[1][KNIGHT][i]];
	for (unsigned int i = 0; i < pieceListTotals[1][BISHOP]; i++) hash = hash ^ blackBishopKeys[pieceLists[1][BISHOP][i]];
	for (unsigned int i = 0; i < pieceListTotals[1][ROOK]; i++) hash = hash ^ blackRookKeys[pieceLists[1][ROOK][i]];
	for (unsigned int i = 0; i < pieceListTotals[1][QUEEN]; i++) hash = hash ^ blackQueenKeys[pieceLists[1][QUEEN][i]];
	
	hash = hash ^ whiteKingKeys[whiteKing];
	hash = hash ^ blackKingKeys[blackKing];
	
	if (castleWK) hash = hash ^ castleKeys[0];
	if (castleWQ) hash = hash ^ castleKeys[1];
	if (castleBK) hash = hash ^ castleKeys[2];
	if (castleBQ) hash = hash ^ castleKeys[3];

	if (enPassant != 0) hash = hash ^ enpassantKeys[enPassant%10-1];

	if (toMove == BLACK) hash = hash ^ blackKey;
}

unsigned long long Position::genRandomKey() {
	unsigned long long random;
	unsigned long long a = 6364136223846793005;
	unsigned long long c = 1442695040888963407;
	unsigned long long m = 0x1000000000000000;
	random = (a*keySeed + c)% m;
	keySeed = random;
	return random;
}