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

    Position Game("4k3/8/8/8/8/8/8/4K2R w K - 0 1");

	//Move theMove(fromAlgebraic("d7"), fromAlgebraic("e6"), -KING, 0, false, 0, false, 0);
	//Game.doMove(theMove);
	//Move theMove2(fromAlgebraic("e2"), fromAlgebraic("e3"), KING, 0, false, 0, false, 0);
	//Game.doMove(theMove2);
	//Move theMove3(fromAlgebraic("e2"), fromAlgebraic("e3"), KING, 0, false, 0, false, 0);
	//Game.doMove(theMove);
    Game.outputDetails();
	divide(Game, 3);
	cin.ignore();
    return 0;
}

