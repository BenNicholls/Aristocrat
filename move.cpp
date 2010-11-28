/*********************************************************************************
                                    Aristocrat
							        Move Class
							         move.cpp
                            Last Updated March 30, 2009

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
Move::Move() {}

//Constructor for regular moves
Move::Move(int f, int t, int p, int c) {
	fromSpace = f;
	toSpace = t;
	piece = p;
	capture = c;
	enPassant = 0;
	castle = 0;
	jump = 0;
	promotion = 0;
}

//Constructor for weird moves (en passant, promotions, all that jazz)
Move::Move(int f, int t, int p, int c, bool en, int ca, bool ju, int pr) {
	fromSpace = f;
	toSpace = t;
	capture = c;
	piece = p;
    enPassant = en;
	castle = ca;
	jump = ju;
    promotion = pr;
}

//Outputs the move in long notation, along with a blurb about promotions and whatnot
void Move::output() {
    if (castle == 1) cout << "0-0" << endl;
    else if (castle == 2) cout << "0-0-0" << endl;
    else {   
		cout << PIECESHORT[abs(piece)] << toAlgebraic(fromSpace);
        if (capture != 0) cout << "x";
        else cout << "-";
        cout << toAlgebraic(toSpace);
        if (promotion != 0) cout << "(" << PIECESHORT[promotion] << ")";
        if (abs(capture) > 1) cout << ", capturing a " << PIECENAMES[abs(capture)] << "!";
        if (abs(capture) == 1) {
            cout << ", capturing a pawn!";
            if (enPassant != 0) cout << "(via En Passant! Ooh la la!)";
        }
        cout << endl;
	}
}

