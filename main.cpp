/*********************************************************************************
                                    Aristocrat
								  Version  0.0.7
						  Last Updated November 28, 2010

FEATURES
	- Board Representation: object containing 1x120 array (padded 8x8 board),
	  with functions for piece movement and output. Also thought up way to load
	  board that makes it easier to test
    - Move object, with different constructors and an output function
    - Pseudo-legal move generator, still in testing
    - Piecelists for white and black
    - Evaluation function counts material, because that's all that's important
    - Broken ass search function that works up to depth 4, but then decides to eat
      balls.
	- Fenboard parser (no checks for FEN validity, so be careful!)
**********************************************************************************/

#include <iostream>

#include "move.h"
#include "functions.h"
#include "position.h"
#include "definitions.h"

using namespace std;

int main(){

    Position Game;

	//Move theMove(fromAlgebraic("a2"), fromAlgebraic("a3"), PAWN, 0, false, 0, false, 0);
	//theMove.output();
	//Game.doMove(theMove);
	//theMove.output();
	//Game.undoMove();
	//theMove.output();

    Game.outputDetails();
	//cout << theMove.piece;
	divide(Game, 4);
	cin.ignore();
    return 0;
}

