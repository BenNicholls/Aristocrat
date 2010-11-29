/*********************************************************************************
                                    Aristocrat
                                  Functions Code
                                   functions.cpp
                            Last Updated March 14, 2009
NOTES
	- Nothing to see here folks.
**********************************************************************************/
#include <vector>
#include <string>
#include <iostream>
#include "position.h"
#include "functions.h"
#include "definitions.h"
#include "movelist.h"

using namespace std;

//Perft function. Depth parameter should be 1 for current position, more for extra depth.
double perft(Position Game, int depth) {
	if (depth == 0) return 1;
	double movecount = 0;
	Movelist theMoves;
	Game.generateMoves(theMoves);
	for (unsigned int i = 0; i < theMoves.list.size(); i++) {
		Position newGame;
		newGame = Game;
		bool check = newGame.doMove(theMoves.list[i]);
		if (check == false) movecount += perft(newGame, depth - 1);
	}
	return movecount;
}

void divide(Position Game, int depth) {
	if (depth == 1) cout << "You are stupid";
	else {
		double masterCount = 0;
		int topNodeCount = 0;
		Movelist theMoves;
		Game.generateMoves(theMoves);
		for (unsigned int i = 0; i < theMoves.list.size(); i++) {
			Position newGame;
			newGame = Game;
			bool check = newGame.doMove(theMoves.list[i]);
			if (check == false) {
				topNodeCount++;
				double nodeCount = perft(newGame, depth - 1);
				masterCount += nodeCount;
				theMoves.list[i].output();
				cout << ": " << nodeCount << endl;
			}
		}
		cout << "Nodes: " << topNodeCount << endl;
		cout << "Total Nodes: " << masterCount << endl;
	}
}

//Takes a square in algebraic notation (ex. a4) and outputs the number on the board.
int fromAlgebraic(string space){
	int file; int rank;
	switch(space[0]){
		case 'a': file = 0; break;
		case 'b': file = 1; break;
		case 'c': file = 2; break;
		case 'd': file = 3; break;
		case 'e': file = 4; break;
		case 'f': file = 5; break;
		case 'g': file = 6; break;
		case 'h': file = 7; break;
	}
	switch(space[1]){
		case '1': rank = 7; break;
		case '2': rank = 6; break;
		case '3': rank = 5; break;
		case '4': rank = 4; break;
		case '5': rank = 3; break;
		case '6': rank = 2; break;
		case '7': rank = 1; break;
		case '8': rank = 0; break;
	}
	return 21 + 10*rank + file;
}

//Takes a board space, returns a string with the algebraic form
string toAlgebraic(int space){
	return FILENAMES[space%10] + RANKNAMES[space/10];
}
