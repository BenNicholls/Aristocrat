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

    Position Game("1r2k2N/p1ppq1b1/bn2pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQ - 0 2");
	//cout << theMove.piece;
	//Game.movesMade.output();
	//double count = perft(Game, 3);

	//cout << count;
	//Movelist moves;
	//divide(Game, 2);

	//Game.generateMoves(moves);
	//moves.output();
	
	vector<int> fails;
	for (unsigned int i = 0; i < 126; i++) {
		bool pass = perftTestSuite(i);
		if (!pass) {
			fails.push_back(i);
		}
	}
	if (fails.size() > 0) {
		cout << "The following tests failed!" << endl;
		for (unsigned int i = 0; i < fails.size(); i++) cout << fails[i] << " ";
	}
	else cout << "YOU ARE THE KING!";

	cin.ignore();
    return 0;
}

