/**************************************************************************************************
|											Aristocrat
|										 Interface Header
|										   interface.h
**************************************************************************************************/

#include "position.h"
#include "move.h"

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
void ifaceAbout();
void ifaceSimpleSearch(string, Position);
Move ifaceSearch(string, Position, int);
void ifacePlay(string, string &);
void ifaceSetDepth(string, int &);
void ifaceGo(Position &, int, string &);
