/**************************************************************************************************
|											Aristocrat
|										 Interface Header
|										   interface.h
**************************************************************************************************/

#include "position.h"

using namespace std;

void interfaceON(Position);
void ifaceDisplay(string, Position);
void ifaceHelp(string);
void ifaceSetBoard(string, Position &);
void ifaceDivide(string, Position);
void ifacePerft(string, Position);
void ifaceNew(Position &);
void ifacePerftTest(string, Position);
void ifaceMove(string, Position &);
void ifaceUndo(string, Position &);
