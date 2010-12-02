/**************************************************************************************************
|											Aristocrat
|										  Interface Code
|										   interface.cpp
|NOTES:
|	- Think about adding support for moves in the form "Bd3", like how everyone else writes them.
**************************************************************************************************/

#include <iostream>
#include <string>
#include <time.h>
#include "engine.h"
#include "interface.h"
#include "position.h"
#include "functions.h"
#include "definitions.h"

using namespace std;

ChessInterface::ChessInterface() {
	
	interface_running = true;
	cout << "Welcome to Aristocrat. Aristocrat likes to play chess. He also likes commmands, like 'help' and 'display'. Go for it, buddy!" << endl;
}

void ChessInterface::ifaceRun() {
	
	while(interface_running) {
		if (Aristocrat.myTurn()) ifaceGo();
		else {
			cout << endl << "->";
			string input = "";
			getline(cin, input);
			
			//extract command and, if provided, parameter
			string command = "";
			string parameter = "";		
			for (unsigned int i = 0; i < input.size(); i++) {
				command += input[i];
				if (command[i] == ' ') {
					if (i != input.size() - 1) {
						command.erase(command.size()-1, 1);
						parameter = input.substr(i + 1, input.size() - i - 1);
					}
					break;
				}
			}

			//check if command is a move (like e2e4, or whatever)
			if (command.size() >= 4) {
				if (fromAlgebraic(command.substr(0,2)) != 0 && fromAlgebraic(command.substr(2,2)) != 0) {
					parameter = command;
					command = "move";
				}
			}

			//lowercase the Command string
			for (unsigned int i = 0; i < command.size(); i++) command[i] = tolower(command[i]);

			if (command == "quit") interface_running = false;
			else if (command == "display") ifaceDisplay(parameter);
			else if (command == "help") ifaceHelp(parameter);
			else if (command == "setboard") ifaceSetBoard(parameter);
			else if (command == "divide") ifaceDivide(parameter);
			else if (command == "perft") ifacePerft(parameter);
			else if (command == "new" || command == "restart") ifaceNew();
			else if (command == "testperft") ifacePerftTest(parameter);
			else if (command == "move") ifaceMove(parameter);
			else if (command == "undo") ifaceUndo(parameter);
			else if (command == "about") ifaceAbout();
			else if (command == "play") ifacePlay(parameter);
			else if (command == "setdepth") ifaceSetDepth(parameter);
			else if (command == "go") ifaceGo();
			else if (command == "search") Move nextMove = ifaceSearch(parameter);
			else if (command == "evaluate") cout << Aristocrat.evaluate();
			else if (command == "") {}
			else {
				cout << "ERROR: ARISTOCRAT AIN'T KNOW WHAT " << command << " MEANS.";
				cout << endl;
			}
		}
	}
}

void ChessInterface::ifaceDisplay(string parameter) {	
	for (unsigned int i = 0; i < parameter.size(); i++) parameter[i] = tolower(parameter[i]);
	if (parameter == "more") Aristocrat.Game.outputDetails();
	else if (parameter == "") Aristocrat.Game.output();
	else cout << "Display what?" << endl;
}

void ChessInterface::ifaceHelp(string parameter) {
	
	cout << endl;
	cout << "Aristocrat understands many commands, much like a butler or slave. Here is a" << endl << "list of them." << endl << endl;
	cout << "new         | Resets the board to the initial position" << endl;
	cout << "restart     | Same as new" << endl;
	cout << "move        | Moves a piece, specified by parameter (ex. move e2e4)" << endl;
	cout << "e2e4        | Same as move, but faster to type" << endl;
	cout << "e7e8Q       | Move notation for promotions." << endl;
	cout << "go          | Make Aristocrat move, regardless of whos turn it is." << endl;
	cout << "play        | Tell Aristocrat who to play as. (white, black, both, none)" << endl;
	cout << "search #    | Perform a # ply search. No # uses default depth." << endl;
	cout << "undo        | Undoes a move. 'Undo #' undoes multiple moves at once." << endl;
	cout << "display     | Shows the board. Type 'display more' for a detailed view" << endl;
	cout << "setboard #  | Sets up position given by # (fen notation)." << endl;
	cout << "setdepth #  | Set number of plys Aristocrat should search for." << endl;
	cout << "perft #     | Calculates the number of halfmoves to a depth given by #" << endl;
	cout << "divide #    | Shows the perft # value for each legal move on the board" << endl;
	cout << "testPerft # | Runs the #th test from the test suite. # = 'all' runs them all!" << endl;
	cout << "help        | Mystery command. Type it in, I bet something cool happens." << endl;
	cout << "about       | Standard blurb about the program. Contains words." << endl;
	cout << "quit        | Exits the program. But why would you want to?" << endl;
}

void ChessInterface::ifaceAbout() {
	cout << "Aristocrat is a chess engine written by Benjamin Nicholls. It is more or" << endl
		 << "less an adventure in radical dopeness, with a move generator (which I" << endl
		 <<	"think is extra perfect) and some functions for testing aforementioned" << endl
		 <<	"move generator. Soon to come: search and eval, with the hope that it will" << endl
		 <<	"be able to actually play some hilariously bad chess. To the future!" << endl;
}

void ChessInterface::ifaceSetBoard(string fen) {
	Position newGame(fen);
	Aristocrat.Game = newGame;
}

void ChessInterface::ifaceDivide(string parameter) {
	int divNum = atoi(parameter.c_str());
	if (divNum > 0) {
		if (divNum > 7) {
			string answer;
			cout << "Dude, this might take a while. You sure? (type 'yes') ->";
			getline(cin, answer);
			if (answer == "yes") {
				Aristocrat.divide(divNum);
			}
			else cout << "It's okay." << endl;
		}
		else {
			Aristocrat.divide(divNum);
		}
	}
	else cout << "Not a valid number for divide. Try 2, or 3 maybe." << endl;
}

void ChessInterface::ifacePerft(string parameter) {
	int perftNum = atoi(parameter.c_str());
	if (perftNum > 0) {
		if (perftNum > 7) {
			string answer;
			cout << "Dude, this might take a while. You sure? (type 'yes') ->";
			getline(cin, answer);
			if (answer == "yes") {
				clock_t t1, t2;
				cout << "Perft " << perftNum << ": ";
				cout.precision(10);
				t1 = clock();
				double count = Aristocrat.perft(perftNum);
				cout << count;
				t2 = clock();
				float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
				cout << ". Computed in " << diff << " seconds (" << count/diff << "nps).";
			}
			else cout << "It's okay." << endl;
		}
		else {
			clock_t t1, t2;
			cout << "Perft " << perftNum << ": ";
			cout.precision(10);
			t1 = clock();
			double count = Aristocrat.perft(perftNum);
			cout << count;
			t2 = clock();
			float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
			cout << ". Computed in " << diff << " seconds (" << count/diff << "nps).";
		}
	}
	else cout << "Not a valid number for perft. Try 2, or 3 maybe." << endl;
}

void ChessInterface::ifaceNew() {
	Position newGame;
	Aristocrat.Game = newGame;
}

void ChessInterface::ifacePerftTest(string parameter) {
	if (parameter == "all") {
		vector<int> fails;
		for (unsigned int i = 0; i < 126; i++) {
			bool pass = Aristocrat.perftTestSuite(i);
			if (!pass) {
				fails.push_back(i);
			}
		}
		if (fails.size() > 0) {
			cout << "The following tests failed!" << endl;
			for (unsigned int i = 0; i < fails.size(); i++) cout << fails[i] << " ";
		}
		else cout << "ALL GOOD, MISTER!";
	}
	else {
		int testNum = atoi(parameter.c_str());
		if (testNum < 1 || testNum > 126) cout << "You gotta give me a valid test number, boy. 1-126, capeche? Or type 'all' to do all of them.";
		else bool pass = Aristocrat.perftTestSuite(testNum - 1);
	}
}

void ChessInterface::ifaceMove(string parameter) {
	int inputtedFromSpace = fromAlgebraic(parameter.substr(0,2));
	int inputtedToSpace = fromAlgebraic(parameter.substr(2,2));

	Movelist tempMoves;
	Aristocrat.Game.generateMoves(tempMoves);

	//check if the inputted move is pseudo-legal
	bool pseudoLegal = false;
	Move inputMove;
	for (unsigned int i = 0; i < tempMoves.totalMoves; i++) {
		if (tempMoves.list[i].fromSpace == inputtedFromSpace && tempMoves.list[i].toSpace == inputtedToSpace) {
			if (tempMoves.list[i].promotion != 0) {
				if (parameter.size() == 5) {
					if (toupper(parameter[4]) == PIECENAMES[tempMoves.list[i].promotion][0]) {
						inputMove = tempMoves.list[i];
						pseudoLegal = true;
						break;
					}
				}
				else break;
			}
			else {
				inputMove = tempMoves.list[i];
				pseudoLegal = true;
				break;
			}
		}
	}
	
	//If move was found in the movelist, make the move. If it is illegal, undo!
	if (!pseudoLegal) cout << "Invalid move. See 'help move' for instructions, dummy.";
	else {
		bool check = Aristocrat.Game.doMove(inputMove);
		if (check) {
			cout << "That move is not legal. Do you know the rules?";
			Aristocrat.Game.undoMove();
		}
		else Aristocrat.Game.output();
	}
}

void ChessInterface::ifaceUndo(string parameter) {
	
	int numUndos;
	if (parameter == "") numUndos = 1;
	else numUndos = atoi(parameter.c_str());

	if (numUndos < 1) cout << "Yeah, I'll get right on that buddy.";
	else {
		for (unsigned int i = 0; i < numUndos; i++) {
			if (Aristocrat.Game.halfMoves == 0) break;
			else Aristocrat.Game.undoMove();
		}
		Aristocrat.Game.output();
	}
}

Move ChessInterface::ifaceSearch(string parameter) {
	int depth;
	if (parameter != "") depth = atoi(parameter.c_str());
	else depth = Aristocrat.searchDepth;

	Movelist Variation;
	if (depth > 0) {
		int score = Aristocrat.rootSearch(depth, -INFINITY, INFINITY, Variation);
		if (abs(score) == CHECKMATE) {
			cout << "It's checkmate, boy!" << endl;
			if (Variation.totalMoves > 0) {
				cout << "Depth " << depth << ": " <<(float)score/100 << " | ";
				Variation.output();
				cout << endl;
			}
		}
		else if (abs(score) == STALEMATE) cout << "Stalemate! Nice try!";
		else if (abs(score) == FIFTYMOVEDRAW) cout << "Draw! Fifty move rule, baby!";
		else if (abs(score) == BAD) {}
		else {
			cout << "Depth " << depth << ": " <<(float)score/100 << " | ";
			Variation.output();
			cout << endl;
		}
	}
	else cout << "Need a search depth, boy.";
	return Variation.list[0];
}

void ChessInterface::ifacePlay(string parameter) {
	if (parameter == "white") Aristocrat.playing = "white";
	else if (parameter == "black") Aristocrat.playing = "black";
	else if (parameter == "both") Aristocrat.playing = "both";
	else if (parameter == "none") Aristocrat.playing = "none";
}
void ChessInterface::ifaceSetDepth(string parameter) {
	int newDepth = atoi(parameter.c_str());

	if (newDepth > 0) Aristocrat.searchDepth = newDepth;
	else cout << "Input a positive number to set the depth to.";
}

void ChessInterface::ifaceGo() {
	Move nextMove = ifaceSearch("");
	if (nextMove.fromSpace != 0) {
		bool check = Aristocrat.Game.doMove(nextMove);
		Aristocrat.Game.output();
		cout << "Aristocrat plays ";
		nextMove.output();
	}
	else {
		Aristocrat.playing = "none";
	}
	cout << endl;
}


	
