#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "stackStruct.h"

#define C_SIZE 8
#define KNIGHT_JUMP_LD 2
#define KNIGHT_JUMP_SD 1
#define CHESSMANS_SIZE 6
#define LOWER_A 97
#define EQUAL_SIGN 61
#define NUMBER_ONE 49
#define UNDO_LIMIT 1
#define UNDO_LIMIT_HEIGHT 6
#define STARTING_CHESSBOARD_LINK "start.cb"
#define KING_ID 1
#define QUEEN_ID 2
#define ROCK_ID 3
#define BISHOP_ID 4
#define KNIGHT_ID 5
#define PAWD_ID 6

extern char captureSign;
extern char chessmans[];




struct game {
	unsigned char** chessBoard;
	bool whiteCastlesChances[2];//magic number?
	bool blackCastlesChances[2];//magic number?
	unsigned char whiteKingPos[2];
	unsigned char blackKingPos[2];
	bool whoToMove;
	struct changesStack changesStack;

};


 

void startAGame(game &startingGame);
void operateGame(struct game game);



