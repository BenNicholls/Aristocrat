/*********************************************************************************
                                    Aristocrat
								  Version  0.0.7
						    Last Updated March 30, 2009

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

TO IMPLEMENT
	- Fenboard parser (although the way I have it set up now should be more than
	  fine for testing purposes

PROBLEMS
    - Oh, couldn't get stupid fenboard thing working, but that's just because I
      was lazy and tried to copy/paste a string splitting function off the internet
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

	//cout << "Welcome to Aristocrat, version 0.0.0" << endl;
    Position Game;
//    Move aMove(84, 64, PAWN, 0, 0, 0, 1, 0);
//    Game.movePiece(aMove);
    Game.output();
	//for (unsigned int i = 0; i < 6; i++) {
	//	 int score = Game.toMove*search(Game, i, -INFINITY, INFINITY);
	//	 cout << i << ":" << score << endl;
	//}

	vector<Move> movelist1 = moveGen(Game);
	Game.toMove = BLACK;
	vector<Move> movelist2 = moveGen(Game);

	movelistOutput(movelist1);
	cout << endl;
	movelistOutput(movelist2);

	cin.ignore();
    return 0;
}

