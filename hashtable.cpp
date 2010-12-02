#include <cstdlib>
#include "hashtable.h"

using namespace std;

Hashtable::Hashtable() {
	
	keySeed = 998698407;

	blackKey = genRandomKey();

	for (unsigned int i = 0; i < 64; i++) {
		whitePawnKeys[i] = genRandomKey();
		whiteKnightKeys[i] = genRandomKey();
		whiteBishopKeys[i] = genRandomKey();
		whiteRookKeys[i] = genRandomKey();
		whiteQueenKeys[i] = genRandomKey();
		whiteKingKeys[i] = genRandomKey();
		blackPawnKeys[i] = genRandomKey();
		blackKnightKeys[i] = genRandomKey();
		blackBishopKeys[i] = genRandomKey();
		blackRookKeys[i] = genRandomKey();
		blackQueenKeys[i] = genRandomKey();
		blackKingKeys[i] = genRandomKey();
		if (i < 8) enpassantKeys[8] = genRandomKey();
		if (i < 4) castleKeys[4] = genRandomKey();
	}
}

unsigned long long Hashtable::genRandomKey() {
	unsigned long long random;
	random = (214013*keySeed + 2531011)% 0x1000000000;
	keySeed = random;
	return random;
}

	