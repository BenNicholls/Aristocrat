/**************************************************************************************************
|											Aristocrat
|										 Interface Header
|										   interface.h
**************************************************************************************************/

#include "engine.h"
#include "position.h"
#include "move.h"


using namespace std;

class ChessInterface {

	public:

		bool interface_running;
		Engine Aristocrat;
		
		ChessInterface();

		void ifaceRun();

		void ifaceDisplay(string);
		void ifaceHelp(string);
		void ifaceSetBoard(string);
		void ifaceDivide(string);
		void ifacePerft(string);
		void ifaceNew();
		void ifacePerftTest(string);
		void ifaceMove(string);
		void ifaceUndo(string);
		void ifaceAbout();
		Move ifaceSearch(string);
		void ifacePlay(string);
		void ifaceSetDepth(string);
		void ifaceGo();
};
