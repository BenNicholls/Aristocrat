#pragma once

#include <string>
#include "position.h"
#include "hashtable.h"
#include "movelist.h"

using namespace std;

class Engine {

	public:

		//Member objects
		Position Game;
		Hashtable Hashes;
		string playing;    //Who the engine should play. white, black, both, none
		int searchDepth;
		
		//Initializers
		Engine();
		Engine(string);
		
		//Search functions
		int search(int, int, int, int,  Movelist &);
		int rootSearch(int, int, int, Movelist &);

		//Eval functions
		int evaluate();
		
		//Test functions
		double perft(int);
		void divide(int);
		bool perftTestSuite(int);

		//Other functions
		void setupPosition(string);
		bool myTurn();
};

