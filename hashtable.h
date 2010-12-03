#pragma once

using namespace std;

#include "definitions.h"
#include "move.h"

class HashtableEntry {

	public:
		
		unsigned long long hash;
		int score;
		//Move bestMove;
		int moveType;
		int depth;
		double nodes;
		bool used;

		//Initializer
		HashtableEntry();
};

class Hashtable {

	public:
		
		HashtableEntry *Table;
		
		//Initializers
		Hashtable();
		
		//Log Hash
		void LogHash(unsigned long long, int, Move, int, int, double);

		//Retreive Hash Entry
		HashtableEntry GetEntry(unsigned long long);

		//Check if valid hash exists
		bool CheckTable(unsigned long long);

		//Retreival functions
		int getScore(unsigned long long);
		//Move getMove(unsigned long long);
		int getMoveType(unsigned long long);
		int getDepth(unsigned long long);
		double getNodes(unsigned long long);
		

};

