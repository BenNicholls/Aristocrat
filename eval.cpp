/*********************************************************************************
                                    Aristocrat
                               Evaluation Function
                                     eval.cpp
                            Last Updated March 14, 2009

NOTES:
	- Evaluation function loops through white pieces and black pieces seperately,
	  for now just counting material
 **********************************************************************************/

 #include "functions.h"
 #include "position.h"
 #include "definitions.h"
 #include <iostream>

 int eval(Position gameBoard) {
	int score = 0;
	for (unsigned int i = 0; i < gameBoard.whitePiecelist.size(); i++) {
		if (gameBoard.whitePiecelist[i] != 0) score = score + PIECEVALUE[gameBoard.board[gameBoard.whitePiecelist[i]]];
	}
	for (unsigned int i = 0; i < gameBoard.blackPiecelist.size(); i++) {
	    if (gameBoard.blackPiecelist[i] !=0) score = score - PIECEVALUE[abs(gameBoard.board[gameBoard.blackPiecelist[i]])];
	}
	return score;
}