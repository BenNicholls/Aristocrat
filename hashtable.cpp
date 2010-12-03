#include <cstdlib>
#include "hashtable.h"
#include "definitions.h"

using namespace std;

HashtableEntry::HashtableEntry() {
	used = false;
}

Hashtable::Hashtable() { 
	Table = new HashtableEntry[HASHTABLESIZE];
}

void Hashtable::LogHash(unsigned long long newHash, int newScore, Move newMove, int newMovetype, int newDepth, double newNodes) {
	
	//Find index
	int i = newHash % HASHTABLESIZE;
	
	//If this is a new position...
	if (!Table[i].used) {
		Table[i].hash = newHash;
		Table[i].score = newScore;
		//Table[i].bestMove = newMove;
		Table[i].depth = newDepth;
		Table[i].nodes = newNodes;
		Table[i].used = true;
	}
	else {
		//Same position. Okay. Replace if depth is higher.
		if (newDepth > Table[i].depth) {
			Table[i].hash = newHash;
			Table[i].score = newScore;
			//Table[i].bestMove = newMove;
			Table[i].depth = newDepth;
			Table[i].nodes = newNodes;
		}
	}
}

HashtableEntry Hashtable::GetEntry(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	return Table[i];
}

bool Hashtable::CheckTable(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	if (Table[i].used){
		if (Table[i].hash == hash) return true;
		else return false;
	}
	else return false;
}

int Hashtable::getScore(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	return Table[i].score;
}

//Move Hashtable::getMove(unsigned long long hash) {
//	int i = hash % HASHTABLESIZE;
//	return Table[i].bestMove;
//}

int Hashtable::getMoveType(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	return Table[i].moveType;
}

int Hashtable::getDepth(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	return Table[i].depth;
}

double Hashtable::getNodes(unsigned long long hash) {
	int i = hash % HASHTABLESIZE;
	return Table[i].nodes;
}
	