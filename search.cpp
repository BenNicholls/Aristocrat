/*********************************************************************************
                                    Aristocrat
                                 Search Function
								    search.cpp
NOTES:
    - HOORAY!
 **********************************************************************************/

#include <algorithm>
#include <iostream>
#include "position.h"
#include "move.h"
#include "movelist.h"
#include "definitions.h"
#include "engine.h"

int Engine::search(int depth, int alpha, int beta, int colour, Movelist &Variation) {
	if (depth == 0) {
		Variation.totalMoves = 0;
		return colour*evaluate();
	}
    else {
		Movelist nowVariation;
        Movelist nextMoves;
        Game.generateMoves(nextMoves);
		
		//In case of mates, both stale and check
		if (nextMoves.totalMoves == 0) {
			Variation.totalMoves = 0;
			if (Game.inCheck()) return -colour*Game.toMove*CHECKMATE;
			else return 0;
		}

		if (Game.fiftyMove == 50) {
			return 0;
		}

		int score = 0;
		bool mate = true;
        for (unsigned int i = 0; i < nextMoves.totalMoves; i++) {
            bool check = Game.doMove(nextMoves.list[i]);
			if (!check) {
				mate = false;
				score = -search(depth - 1, -beta, -alpha, -colour, nowVariation);
				if (score > alpha) {
					alpha = score;
					Variation.totalMoves = 0;
					Variation.add(nextMoves.list[i]);
					for (unsigned int j = 0; j < nowVariation.totalMoves; j++) {
						Variation.add(nowVariation.list[j]);
					}
				}
				if (alpha >= beta) {
					Game.undoMove();
					break;
				}
			}
			Game.undoMove();
        }
		if (mate) {
			if (Game.inCheck()) {
				Variation.totalMoves = 0;
				return -colour*Game.toMove*CHECKMATE;
			}
			else return 0;
		}


        return alpha;
    }
}

int Engine::rootSearch(int depth, int alpha, int beta, Movelist &Variation) {
	if (depth == 0) {
		cout << "Why you do this.";
		return BAD;
	}
    else {
        Movelist nextMoves;
		Movelist nowVariation;
        Game.generateMoves(nextMoves);

		//In case of mates, both stale and check
		if (nextMoves.totalMoves == 0) {
			if (Game.inCheck()) return CHECKMATE;
			else return STALEMATE;
		}
		if (Game.fiftyMove == 50) return FIFTYMOVEDRAW;

		int score = 0;
		bool mate = true;
        for (unsigned int i = 0; i < nextMoves.totalMoves; i++) {
            bool check = Game.doMove(nextMoves.list[i]);			
			if (!check) {
				mate = false;
				score = -search(depth - 1, -beta, -alpha, Game.toMove, nowVariation);
				if (score > alpha) {
					alpha = score;
					Variation.totalMoves = 0;
					Variation.add(nextMoves.list[i]);
					for (unsigned int j = 0; j < nowVariation.totalMoves; j++) {
						Variation.add(nowVariation.list[j]);					
					}					
				}
				if (alpha >= beta) {
					Game.undoMove();
					break;
				}
			}
			Game.undoMove();
        }
		if (mate) {
			if (Game.inCheck()) return CHECKMATE;
			else return 0;
		}
        return Game.toMove*alpha;
    }
}