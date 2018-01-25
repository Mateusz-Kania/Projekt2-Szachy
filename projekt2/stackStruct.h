#pragma once
#include "stdafx.h"

struct changesSet {
	bool castle;
	bool passant;
	unsigned char change1[3];
	unsigned char change2[3];
	unsigned char change3[3];
	unsigned char change4[3];
	unsigned char kingsPrevPosition[2];
	bool whiteCastlesPrevChances[2];
	bool blackCastlesPrevChances[2];
};

struct changesStackElement {
	struct changesSet changesSet;
	struct changesStackElement *nextElement;
};
struct changesStack {
	unsigned int size;
	bool maxSizeSet;
	unsigned int maxSize;
	struct changesStackElement *nextElement;
};