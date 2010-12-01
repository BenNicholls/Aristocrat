/*********************************************************************************
                                    Aristocrat
								  Version  0.0.7
FEATURES
	- Board Representation: object containing 1x120 array (padded 8x8 board),
	  with functions for piece movement and output. Also thought up way to load
	  board that makes it easier to test
    - Move object, with different constructors and an output function
    - Pseudo-legal move generator
    - Piecelists for white and black
    - Evaluation function counts material, because that's all that's important
    - Broken ass search function that works up to depth 4, but then decides to eat
      balls.
	- Fenboard parser (no checks for FEN validity, so be careful!)
**********************************************************************************/

#include <iostream>
#include <time.h>

#include "move.h"
#include "functions.h"
#include "position.h"
#include "definitions.h"
#include "interface.h"


using namespace std;

int main(){

    Position Game("3k1q2/p3bP2/1p1pQ3/8/5r2/8/PP3PPP/2R1R1K1 b - - 0 32");
	interfaceON(Game);

    return 0;
}

