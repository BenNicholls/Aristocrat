#pragma once

using namespace std;

class Hashtable {

	public:

		unsigned long long whitePawnKeys[64];
		unsigned long long whiteKnightKeys[64];
		unsigned long long whiteBishopKeys[64];
		unsigned long long whiteRookKeys[64];
		unsigned long long whiteQueenKeys[64];
		unsigned long long whiteKingKeys[64];
		unsigned long long blackPawnKeys[64];
		unsigned long long blackKnightKeys[64];
		unsigned long long blackBishopKeys[64];
		unsigned long long blackRookKeys[64];
		unsigned long long blackQueenKeys[64];
		unsigned long long blackKingKeys[64];
		unsigned long long enpassantKeys[8];
		unsigned long long castleKeys[4];
		unsigned long long blackKey;
		
		unsigned long long keySeed;

		//Initializers
		Hashtable();

		//Key Generation
		unsigned long long genRandomKey();
};
