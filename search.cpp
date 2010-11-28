/*********************************************************************************
                                    Aristocrat
                                 Search Function
								    search.cpp
							Last Updated March 24, 2009

NOTES:
    - WHY DONT YOU WORK, GODDAMN YOU.
 **********************************************************************************/

#include <vector>
#include <algorithm>
#include <iostream>
#include "position.h"
#include "move.h"
#include "functions.h"
#include "movelist.h"
#include "definitions.h"

double nodes = 0;

int search(Position Game, int depth, int alpha, int beta) {
    nodes++;
    if (depth == 0) return eval(Game);
    else {
        Movelist nextMoves;
        Game.generateMoves(nextMoves);
        for (unsigned int i = 0; i < nextMoves.list.size(); i++) {
            Position Next = Game;
            bool check = Next.movePiece(nextMoves.list[i]);
			if (check) return beta;
			else {
				int score = -1*search(Next, depth - 1, -1*beta, -1*alpha);
				if (score >= beta) return beta;
				if (score > alpha) alpha = score;
			}
        }
        return alpha;
    }
}
