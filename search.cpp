/*********************************************************************************
                                    Aristocrat
                                 Search Function
								    search.cpp
							Last Updated March 24, 2009

NOTES:
    - WHY DONT YOU WORK, GODDAMN YOU.
 **********************************************************************************/

#include "position.h"
#include "move.h"
#include "functions.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "definitions.h"

double nodes = 0;

int searchW(Position Game, int depth, int alpha, int beta) {
    nodes++;
    if (depth == 0) return eval(Game);
    else {
        vector<Move> movelist;
        movelist = moveGen(Game);
        for (unsigned int i = 0; i < movelist.size(); i++) {
            Position Next = Game;
            Next.movePiece(movelist[i]);
            int score = searchB(Next, depth - 1, alpha, beta);
            if (score > alpha) alpha = score;
            if (score >= beta) return beta;
        }
        return alpha;
    }
}

int searchB(Position Game, int depth, int alpha, int beta) {
    nodes++;
    if (depth == 0) return eval(Game);
    else {
        vector<Move> movelist;
        movelist = moveGen(Game);
        for (unsigned int i = 0; i < movelist.size(); i++) {
            Position Next = Game;
            Next.movePiece(movelist[i]);
            int score = searchW(Next, depth - 1, alpha, beta);
            if (score < beta) beta = score;
            if (score <= alpha) return alpha;
        }
        return beta;
    }
}

int search(Position Game, int depth, int alpha, int beta) {
    nodes++;
    if (depth == 0) return eval(Game);
    else {
        vector<Move> movelist;
        movelist = moveGen(Game);
        for (unsigned int i = 0; i < movelist.size(); i++) {
            Position Next = Game;
            bool check = Next.movePiece(movelist[i]);
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
