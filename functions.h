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
#include <string>


double perft(Position, int);

int eval(Position);

int search(Position, int, int, int);

int fromAlgebraic(string);
string toAlgebraic(int);