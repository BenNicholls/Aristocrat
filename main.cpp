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
    - Alpha-beta Negamax search
	- Fenboard parser (no checks for FEN validity, so be careful!)
	- Hashtable what can log hashes

NOTES FOR LATER:
	- Split up piecelists?
	- Implement piecelist indexing table
	- Move ordering, quiesence search
	- Transposition table
	- DO all of this stuff
	- Also, make Aristocrat faster maybe
	- And better!
	- Maybe add swearing mode to the interface.
**********************************************************************************/

#include <iostream>
#include <time.h>
#include "engine.h"
#include "move.h"
#include "functions.h"
#include "position.h"
#include "definitions.h"
#include "interface.h"


using namespace std;

int main(){

	ChessInterface iface;
	Move theMove;
	iface.Aristocrat.Hashes.LogHash(50,3,theMove, 9, 40, 8);
	int i = 50%HASHTABLESIZE;
	cout << iface.Aristocrat.Hashes.Table[i].used;
	cout << iface.Aristocrat.Hashes.CheckTable(50);
	iface.ifaceRun();

    return 0;
}

