#include <fstream>
#include <iostream>
#include "engine.h"
#include "definitions.h"

using namespace std;

Engine::Engine() { 
	playing = "none";
	searchDepth = 6;
}

Engine::Engine(string fen) {
	playing = "none";
	searchDepth = 6;
	setupPosition(fen);
}

void Engine::setupPosition(string fen) {
	Position newGame(fen);
	Game = newGame;
}

bool Engine::myTurn() {
	if (playing == "both" || (playing == "white" && Game.toMove == WHITE) || (playing == "black" && Game.toMove == BLACK)) return true;
	else return false;
}

bool Engine::perftTestSuite(int lineNum) {
	if (lineNum < 126){
		Position currentGame;
		currentGame = Game;

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
		setupPosition(chunks[0]);
		Game.output();
		
		cout << "Test " << lineNum + 1 << ": " << chunks[0] << endl;
		cout << "----------------------------------------------------------------------" << endl;
		bool pass = true;

		for (unsigned int i = 0; i < 6; i++) {
			cout << "Perft " << i+1 << ": ";
			cout.precision(10);
			cout << " (expected value: " << expectations[i] << ")... ";
			if (i == 5 && lineNum == 1) break;
			double movecount = perft(i+1);
			cout << movecount;
			if (movecount == expectations[i]) cout << " OK BOSS!" << endl;
			else {
				cout << " Oooh, off by " << expectations[i] - movecount << endl;
				pass = false;
			}
		}
		cout << endl;
		Game = currentGame;

		return pass;
	}
	else return false;
}

double Engine::perft(int depth) {
	if (depth == 0) return 1;
	double movecount = 0;
	Move aMove;
	Movelist theMoves;
	Game.generateMoves(theMoves);
	for (unsigned int i = 0; i < theMoves.totalMoves; i++) {
		bool check = Game.doMove(theMoves.list[i]);
		if (check == false) { 
			if(Hashes.CheckTable(Game.hash) && Hashes.getDepth(Game.hash) == depth) {
				movecount += Hashes.getNodes(Game.hash);
			}
			else {
				double count = perft(depth - 1);
				Hashes.LogHash(Game.hash, 0, aMove, 0, depth, count);
				movecount += count;
			}
		}
		Game.undoMove();
	}
	return movecount;
}

void Engine::divide(int depth) {
	double masterCount = 0;
	int topNodeCount = 0;
	Movelist theMoves;
	Game.generateMoves(theMoves);
	for (unsigned int i = 0; i < theMoves.totalMoves; i++) {
		bool check = Game.doMove(theMoves.list[i]);
		if (check == false) {
			topNodeCount++;
			theMoves.list[i].output();
			double nodeCount = perft(depth - 1);
			masterCount += nodeCount;
			cout << ": " << nodeCount << endl;
		}
		Game.undoMove();
	}
	cout.precision(10);
	cout << "Nodes: " << topNodeCount << endl;
	cout << "Total Nodes: " << masterCount << endl;
}