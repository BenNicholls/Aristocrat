/*********************************************************************************
                                    Aristocrat
                               Constant Definitions
                                  definitions.h
                           Last Updated March 30, 2009

NOTES
    - Do I like NOBOARD being 99? I wish I could make it imaginary or something.
    - Pfft. Nice infinity, buddy. Your mom get you that at the thrift store?

**********************************************************************************/

#pragma once

#include <string>

using namespace std;

const int WHITE = 1;
const int BLACK = -1;

const int NOBOARD = 99;
const int EMPTY = 0;
const int PAWN = 1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int ROOK = 4;
const int QUEEN = 5;
const int KING = 6;

const int PIECEVALUE[7] = { 0, 100, 300, 300, 500, 900, 10000 };

const int INFINITY = 2147483647;

const string PIECENAMES[7] = { "Empty", "Pawn", "Knight", "Bishop", "Rook", "Queen", "King" };

const string PIECESHORT[7] = { "", "", "N", "B", "R", "Q", "K" };

const string WHITEPIECEOUTPUT[7] = { " ", "P", "N", "B", "R", "Q", "K" };
const string BLACKPIECEOUTPUT[7] = { " ", "p", "n", "b", "r", "q", "k" };

const string FILENAMES[10] = {"", "a", "b", "c", "d", "e", "f", "g", "h", ""};
const string RANKNAMES[12] = {"", "", "8", "7", "6", "5", "4", "3", "2", "1", "", ""};

const int SLIDEMOVES[8] = { -11, -9, 11, 9, -10, 1, 10, -1 };
const int KNIGHTMOVES[8] = { -21, -19, -8, 12, 21, 19, 8, -12 };
const int KINGMOVES[8] = { -11, -10, -9, 1, 11, 10, 9, -1 };
