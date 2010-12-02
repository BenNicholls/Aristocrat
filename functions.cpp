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



int genRandInt(int low, int high){
    return (rand() % (high - low + 1)) + low;
}