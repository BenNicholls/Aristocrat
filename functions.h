/*********************************************************************************
                                    Aristocrat
                                 Functions Header
                                    functions.h
                            Last Updated March 14, 2009

NOTES
  - Well. Isn't this fantastic. Function declarations: this is where the goods is.
**********************************************************************************/

#pragma once

#include "position.h"
#include <vector>

vector<Move> moveGen(Position);

void movelistOutput(vector<Move>);

double perft(Position, int);

int eval(Position);

int searchW(Position, int, int, int);

int searchB(Position, int, int, int);

int search(Position, int, int, int);
