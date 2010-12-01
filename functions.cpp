/*********************************************************************************
                                    Aristocrat
                                  Functions Code
                                   functions.cpp
NOTES
	- Nothing to see here folks.
**********************************************************************************/
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "position.h"
#include "functions.h"
#include "definitions.h"
#include "movelist.h"

using namespace std;

//Perft function. Depth parameter should be 1 for current position, more for extra depth.
double perft(Position &Game, int depth) {
	if (depth == 0) return 1;
	double movecount = 0;
	Movelist theMoves;
	Game.generateMoves(theMoves);
	for (unsigned int i = 0; i < theMoves.totalMoves; i++) {
		bool check = Game.doMove(theMoves.list[i]);
		if (check == false) { 
			movecount += perft(Game, depth - 1);
		}
		Game.undoMove();
	}
	return movecount;
}

void divide(Position &Game, int depth) {
	double masterCount = 0;
	int topNodeCount = 0;
	Movelist theMoves;
	Game.generateMoves(theMoves);
	for (unsigned int i = 0; i < theMoves.totalMoves; i++) {
		bool check = Game.doMove(theMoves.list[i]);
		if (check == false) {
			topNodeCount++;
			theMoves.list[i].output();
			double nodeCount = perft(Game, depth - 1);
			masterCount += nodeCount;
			cout << ": " << nodeCount << endl;
		}
		Game.undoMove();
	}
	cout.precision(10);
	cout << "Nodes: " << topNodeCount << endl;
	cout << "Total Nodes: " << masterCount << endl;
}

//Takes a square in algebraic notation (ex. a4) and outputs the number on the board.
int fromAlgebraic(string space){
	int file = 9; int rank = 9;
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
	if (file == 9 || rank == 9) return 0;
	else return 21 + 10*rank + file;
}

//Takes a board space, returns a string with the algebraic form
string toAlgebraic(int space){
	return FILENAMES[space%10] + RANKNAMES[space/10];
}

bool perftTestSuite(int lineNum) {
	if (lineNum < 126){
		vector<string> lines;
		string currentline;
		ifstream myfile("perftsuite.epd");
		if (myfile.is_open()) {
			while (myfile.good()) {
				getline(myfile, currentline);
				lines.push_back(currentline);
			}
			myfile.close();
		}
		else { 
			cout << "FILE ERROR";
			return false;
		}

		//split into chunks
		int count = 0;
		string parseString = lines[lineNum];
		string chunks[7];
		string currentChunk = "";
		for (unsigned int i = 0; i < parseString.size(); i++) {
			if (parseString[i] == ';') {
				currentChunk.erase(currentChunk.size()-1, 1);
				chunks[count] = currentChunk;
				currentChunk = "";
				count++;
			}
			else currentChunk += parseString[i];
		}
		chunks[6] = currentChunk;

		//Log expected values
		double expectations[6];
		for (unsigned int i = 0; i < 6; i++) {
			chunks[i+1].erase(0,3);
			expectations[i] = atof(chunks[i+1].c_str());
		}

		//Setup Game
		Position testGame(chunks[0]);
		testGame.output();
		
		cout << "Test " << lineNum + 1 << ": " << chunks[0] << endl;
		cout << "----------------------------------------------------------------------" << endl;
		bool pass = true;

		for (unsigned int i = 0; i < 6; i++) {
			cout << "Perft " << i+1 << ": ";
			cout.precision(10);
			cout << " (expected value: " << expectations[i] << ")... ";
			if (i == 5 && lineNum == 1) break;
			double movecount = perft(testGame, i+1);
			cout << movecount;
			if (movecount == expectations[i]) cout << " OK BOSS!" << endl;
			else {
				cout << " Oooh, off by " << expectations[i] - movecount << endl;
				pass = false;
			}
		}
		cout << endl;

		return pass;
	}
	else return false;
}

int genRandInt(int low, int high){
    return (rand() % (high - low + 1)) + low;
}