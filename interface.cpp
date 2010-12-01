/**************************************************************************************************
|											Aristocrat
|										  Interface Code
|										   interface.cpp
**************************************************************************************************/

#include <iostream>
#include <string>
#include <time.h>
#include "interface.h"
#include "position.h"
#include "functions.h"
#include "definitions.h"

using namespace std;

void interfaceON(Position Game) {

	bool interface_running = true;

	cout << "Welcome to Aristocrat. Aristocrat likes to play chess. He also likes commmands, like 'help' and 'display'. Go for it, buddy!" << endl;
	
	while(interface_running) {
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

		//check if command is a move
		if (command.size() >= 4) {
			if (fromAlgebraic(command.substr(0,2)) != 0 && fromAlgebraic(command.substr(2,2)) != 0) {
				parameter = command;
				command = "move";
			}
		}

		//lowercase the Command string
		for (unsigned int i = 0; i < command.size(); i++) command[i] = tolower(command[i]);

		if (command == "quit") interface_running = false;
		else if (command == "display") ifaceDisplay(parameter, Game);
		else if (command == "help") ifaceHelp(parameter);
		else if (command == "setboard") ifaceSetBoard(parameter, Game);
		else if (command == "divide") ifaceDivide(parameter, Game);
		else if (command == "perft") ifacePerft(parameter, Game);
		else if (command == "new" || command == "restart") ifaceNew(Game);
		else if (command == "testperft") ifacePerftTest(parameter, Game);
		else if (command == "move") ifaceMove(parameter, Game);
		else if (command == "undo") ifaceUndo(parameter, Game);
		else if (command == "") {}
		else {
			cout << "ERROR: ARISTOCRAT AIN'T KNOW WHAT " << command << " MEANS.";
			cout << endl;
		}
	}
}

void ifaceDisplay(string parameter, Position Game) {	
	for (unsigned int i = 0; i < parameter.size(); i++) parameter[i] = tolower(parameter[i]);
	if (parameter == "more") Game.outputDetails();
	else if (parameter == "") Game.output();
	else cout << "Display what?" << endl;
}

void ifaceHelp(string parameter) {
	
	cout << endl;
	cout << "Aristocrat understands many commands, much like a butler or slave. Here is a" << endl << "list of them." << endl << endl;
	cout << "new         | Resets the board to the initial position" << endl;
	cout << "restart     | Same as new" << endl;
	cout << "move        | Moves a piece, specified by parameter (ex. move e2e4)" << endl;
	cout << "e2e4        | Same as move, but faster to type" << endl;
	cout << "e7e8Q       | Move notation for promotions." << endl;
	cout << "undo        | Undoes a move. 'Undo #' undoes multiple moves at once." << endl;
	cout << "display     | Shows the board. Type 'display more' for a detailed view" << endl;
	cout << "setboard #  | Sets up position given by # (fen notation)." << endl;
	cout << "perft #     | Calculates the number of halfmoves to a depth given by #" << endl;
	cout << "divide #    | Shows the perft # value for each legal move on the board" << endl;
	cout << "testPerft # | Runs the #th test from the test suite. # = 'all' runs them all!" << endl;
	cout << "help        | Mystery command. Type it in, I bet something cool happens." << endl;
	cout << "quit        | Exits the program. But why would you want to?" << endl;
}

void ifaceSetBoard(string fen, Position &Game) {
	Position newGame(fen);
	Game = newGame;
}

void ifaceDivide(string parameter, Position Game) {
	int divNum = atoi(parameter.c_str());
	if (divNum > 0) {
		if (divNum > 7) {
			string answer;
			cout << "Dude, this might take a while. You sure? (type 'yes') ->";
			getline(cin, answer);
			if (answer == "yes") {
				divide(Game, divNum);
			}
			else cout << "It's okay." << endl;
		}
		else {
			divide(Game, divNum);
		}
	}
	else cout << "Not a valid number for divide. Try 2, or 3 maybe." << endl;
}

void ifacePerft(string parameter, Position Game) {
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
				double count = perft(Game, perftNum);
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
			double count = perft(Game, perftNum);
			cout << count;
			t2 = clock();
			float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
			cout << ". Computed in " << diff << " seconds (" << count/diff << "nps).";
		}
	}
	else cout << "Not a valid number for perft. Try 2, or 3 maybe." << endl;
}

void ifaceNew(Position &Game) {
	Position newGame;
	Game = newGame;
}

void ifacePerftTest(string parameter, Position Game) {
	if (parameter == "all") {
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
		else cout << "ALL GOOD, MISTER!";
	}
	else {
		int testNum = atoi(parameter.c_str());
		if (testNum < 1 || testNum > 126) cout << "You gotta give me a valid test number, boy. 1-126, capeche? Or type 'all' to do all of them.";
		else bool pass = perftTestSuite(testNum - 1);
	}
}

void ifaceMove(string parameter, Position &Game) {
	int inputtedFromSpace = fromAlgebraic(parameter.substr(0,2));
	int inputtedToSpace = fromAlgebraic(parameter.substr(2,2));

	Movelist tempMoves;
	Game.generateMoves(tempMoves);

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
		bool check = Game.doMove(inputMove);
		if (check) {
			cout << "That move is not legal. Do you know the rules?";
			Game.undoMove();
		}
		else Game.output();
	}
}

void ifaceUndo(string parameter, Position &Game) {
	
	int numUndos;
	if (parameter == "") numUndos = 1;
	else numUndos = atoi(parameter.c_str());

	if (numUndos < 1) cout << "Yeah, I'll get right on that buddy.";
	else {
		for (unsigned int i = 0; i < numUndos; i++) {
			if (Game.halfMoves == 0) break;
			else Game.undoMove();
		}
		Game.output();
	}
}
