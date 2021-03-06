/*********************************************************************************
                                    Aristocrat
							        Move Class
							         move.cpp

NOTES
	- Move output now correctly supports blurbs about castling, promotion, captures,
	  en passant, and puts a sweet x when there is a capture instead of a hyphen
**********************************************************************************/

#include "move.h"
#include <iostream>
#include "definitions.h"
#include <cstdlib>
#include "functions.h"

using namespace std;

//Dummy constructor to fool stupid arrays and functions and whatever
Move::Move() {
	fromSpace = 0;
	toSpace = 0;
	capture = 0;
	piece = 0;
	enPassant = false;
	castle = 0;
	jump = false;
	promotion = 0;
}

Move::Move(int &f, int &t, int &p, int c, bool en, int ca, bool ju, int pr) {
	fromSpace = f;
	toSpace = t;
	capture = c;
	piece = p;
    enPassant = en;
	castle = ca;
	jump = ju;
    promotion = pr;
}

void Move::outputShort() {
	if (castle == 1) cout << "0-0";
    else if (castle == 2) cout << "0-0-0";
    else {   
		cout << PIECESHORT[abs(piece)] << toAlgebraic(fromSpace);
        if (capture != 0) cout << "x";
        cout << toAlgebraic(toSpace);
        if (promotion != 0) cout << "(" << PIECESHORT[abs(promotion)] << ")";
	}
}

//Outputs the move in long notation, along with a blurb about promotions and whatnot
void Move::output() {
    if (castle == 1) cout << "0-0";
    else if (castle == 2) cout << "0-0-0";
    else {   
		cout << PIECESHORT[abs(piece)] << toAlgebraic(fromSpace);
        if (capture != 0) cout << "x";
        else cout << "-";
        cout << toAlgebraic(toSpace);
        if (promotion != 0) cout << "(" << PIECESHORT[abs(promotion)] << ")";
        if (abs(capture) > 1) cout << ", capturing a " << PIECENAMES[abs(capture)] << "!";
        if (abs(capture) == 1) {
            cout << ", capturing a pawn!";
            if (enPassant != 0) cout << "(via En Passant! Ooh la la!)";
        }
	}
}



