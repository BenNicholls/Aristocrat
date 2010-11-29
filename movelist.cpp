#include <iostream>
#include "movelist.h"
#include "move.h"
#include "definitions.h"

Movelist::Movelist() { };

void Movelist::add_move(int from, int to){
	Move newMove(from, to, currPiece, 0, false, 0, false, 0);
	list.push_back(newMove);
}

void Movelist::add_promos(int from, int to, int cap){
	for (unsigned int i = 2; i < 6; i++){
		Move newMove(from, to, currPiece, cap, false, 0, false, i);
		list.push_back(newMove);
	}
}

void Movelist::add_castle(int type, int toMove){
	int from = 60+35*toMove;
	if (type == 1) {
		Move newMove(from, from + 2, currPiece, 0, false, 1, false, 0);
		list.push_back(newMove);
	}
	if (type == 2) {
		Move newMove(from, from - 2, currPiece, 0, false, 2, false, 0);
		list.push_back(newMove);
	}
}

void Movelist::add_capture(int from, int to, int cap) {
	Move newMove(from, to, currPiece, cap, false, 0, false, 0);
	list.push_back(newMove);
}

void Movelist::add_enpassant(int from, int to, int side) {
	Move newMove(from, to, currPiece, PAWN*side, true, 0, false, 0);
	list.push_back(newMove);
}

void Movelist::add_pawnjump(int from, int to) {
	Move newMove(from, to, currPiece, 0, 0, 0, true, 0);
	list.push_back(newMove);
}

void Movelist::output() {
	if (list.size() == 0) cout << "No moves! Checkmate?";
	else for (unsigned int i = 0; i < list.size(); i++) list[i].output();
}