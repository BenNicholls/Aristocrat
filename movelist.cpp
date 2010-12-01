#include <iostream>
#include "movelist.h"
#include "move.h"
#include "definitions.h"

Movelist::Movelist() {totalMoves = 0; };

void Movelist::add(Move theMove) {
	list[totalMoves] = theMove;
	totalMoves++;
}

void Movelist::remove_last() {
	totalMoves = totalMoves - 1;
}

void Movelist::add_move(int &from, int &to){
	Move newMove(from, to, currPiece, 0, false, 0, false, 0);
	list[totalMoves] = newMove;
	totalMoves++;
}

void Movelist::add_promos(int &from, int to, const int &cap){
	for (unsigned int i = 2; i < 6; i++){
		Move newMove(from, to, currPiece, cap, false, 0, false, i);
		list[totalMoves] = newMove;
		totalMoves++;
	}
}

void Movelist::add_castle(int type, int toMove){
	int from = 60+35*toMove;
	if (type == 1) {
		int to = from+2;
		Move newMove(from, to, currPiece, 0, false, 1, false, 0);
		list[totalMoves] = newMove;
	}
	else if (type == 2) {
		int to = from-2;
		Move newMove(from, to, currPiece, 0, false, 2, false, 0);
		list[totalMoves] = newMove;
	}
	totalMoves++;
}

void Movelist::add_capture(int &from, int &to, int &cap) {
	Move newMove(from, to, currPiece, cap, false, 0, false, 0);
	list[totalMoves] = newMove;
	totalMoves++;
}

void Movelist::add_enpassant(int &from, int to, int side) {
	Move newMove(from, to, currPiece, PAWN*side, true, 0, false, 0);
	list[totalMoves] = newMove;
	totalMoves++;
}

void Movelist::add_pawnjump(int &from, int to) {
	Move newMove(from, to, currPiece, 0, false, 0, true, 0);
	list[totalMoves] = newMove;
	totalMoves++;
}

void Movelist::output() {
	if (totalMoves == 0) cout << "No moves! Checkmate?";
	else {
		for (unsigned int i = 0; i < totalMoves; i++){
			list[i].outputShort();
		cout << " ";
		}
		cout << endl;
	}
}