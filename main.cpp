/*********************************************************************************
                                    Aristocrat
								  Version  0.0.7
						  Last Updated November 28, 2010

FEATURES
	- Board Representation: object containing 1x120 array (padded 8x8 board),
	  with functions for piece movement and output. Also thought up way to load
	  board that makes it easier to test
    - Move object, with different constructors and an output function
    - Pseudo-legal move generator, so far it is perfect but who knows, right?
    - Piecelists for white and black
    - Evaluation function counts material, because that's all that's important
    - Broken ass search function that works up to depth 4, but then decides to eat
      balls.
	- Fenboard parser (no checks for FEN validity, so be careful!)

PROBLEMS
    - Somehow, even when copy/pasting the old search code, Aristocrat manages to
      fuck up EVEN MORE than Sir Chess did. What the hell.

OTHER NOTES
    - Well, Aristocrat is slower at move generation than even Sir Chess,
      which is a really sad fact because Sir Chess sucked twice on sunday. Sure
      Aristocrat is fully loaded with castling and promotions and missiles, but
      still. The good thing is that the actual code for the move generator is only
      1/4 the number of lines as Sir Chess'. I suppose that's a plus.
**********************************************************************************/

#include <iostream>

#include "move.h"
#include "functions.h"
#include "position.h"
#include "definitions.h"

using namespace std;

int main(){

    Position Game("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0");
    Game.output();

	double moves = perft(Game, 1);
	cout << moves;

	cin.ignore();
    return 0;
}

