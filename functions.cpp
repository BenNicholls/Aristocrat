/*********************************************************************************
                                    Aristocrat
                                  Functions Code
                                   functions.cpp
                            Last Updated March 14, 2009

NOTES
	- Nothing to see here folks.

**********************************************************************************/

#include "position.h"
#include <iostream>
#include "functions.h"
#include <vector>

using namespace std;

//Nice little function to print out the contents of any vector of moves. Good for testing!
void movelistOutput(vector<Move> movelist) {
    for (unsigned int i = 0; i < movelist.size(); i++) movelist[i].output();
}

double perft(Position Game, int depth) {
		double movecount = 0;
		vector<Move> movelist;
		movelist = moveGen(Game);
		if (depth == 0) {
			if (Game.inCheck()) return 0;
			else return 1;
		}
		else {
			for (unsigned int i = 0; i < movelist.size(); i++) {
				Position newGame;
				newGame = Game;
				movecount += perft(newGame, depth - 1);
			}
		}
		return movecount;
}
