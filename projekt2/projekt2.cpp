// projekt2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "chess.h"

char chessmans[] = "KQRBN";
char captureSign = 120;

int main()
{
	setlocale(LC_ALL, "");
	struct game game;
	startAGame(game);
	operateGame(game);
	return 0;
}
