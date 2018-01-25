
#include "stdafx.h"
#include "chessBoardCreatorFunctions.h"

void saveToFile(unsigned char x, unsigned char y, unsigned char chessman, std::ofstream & saveStream) {
	saveStream.write((char*)&x, sizeof x);
	saveStream.write((char*)&y, sizeof y);
	saveStream.write((char*)&chessman, sizeof chessman);

}

void chessmansStandard(std::string fileName) {
	std::ofstream saveStream;
	unsigned char whiteKingPosX = 4;
	unsigned char whiteKingPosY = 0;
	unsigned char blackKingPosX = 4;
	unsigned char blackKingPosY = 7;
	bool castles = true;
	saveStream.open(fileName, std::ios_base::trunc | std::ios_base::binary);
	saveStream.write((char*)&whiteKingPosX, sizeof whiteKingPosX);
	saveStream.write((char*)&whiteKingPosY, sizeof whiteKingPosY);
	saveStream.write((char*)&blackKingPosX, sizeof blackKingPosX);
	saveStream.write((char*)&blackKingPosY, sizeof blackKingPosY);
	saveStream.write((char*)&castles, sizeof castles);
	saveStream.write((char*)&castles, sizeof castles);
	saveStream.write((char*)&castles, sizeof castles);
	saveStream.write((char*)&castles, sizeof castles);

	if (saveStream.is_open())

		for (unsigned char i = 0; i < 8; ++i) {
			saveToFile(i, (unsigned char)1, (unsigned char)6, saveStream);
		}
	for (unsigned char i = 0; i < 8; ++i) {
		saveToFile(i, (unsigned char)6, (unsigned char)12, saveStream);

	}
	saveToFile((unsigned char)0, (unsigned char)0, (unsigned char)3, saveStream);
	saveToFile((unsigned char)1, (unsigned char)0, (unsigned char)5, saveStream);
	saveToFile((unsigned char)2, (unsigned char)0, (unsigned char)4, saveStream);
	saveToFile((unsigned char)3, (unsigned char)0, (unsigned char)2, saveStream);
	saveToFile((unsigned char)4, (unsigned char)0, (unsigned char)1, saveStream);
	saveToFile((unsigned char)5, (unsigned char)0, (unsigned char)4, saveStream);
	saveToFile((unsigned char)6, (unsigned char)0, (unsigned char)5, saveStream);
	saveToFile((unsigned char)7, (unsigned char)0, (unsigned char)3, saveStream);
	saveToFile((unsigned char)0, (unsigned char)7, (unsigned char)9, saveStream);
	saveToFile((unsigned char)1, (unsigned char)7, (unsigned char)11, saveStream);
	saveToFile((unsigned char)2, (unsigned char)7, (unsigned char)10, saveStream);
	saveToFile((unsigned char)3, (unsigned char)7, (unsigned char)8, saveStream);
	saveToFile((unsigned char)4, (unsigned char)7, (unsigned char)7, saveStream);
	saveToFile((unsigned char)5, (unsigned char)7, (unsigned char)10, saveStream);
	saveToFile((unsigned char)6, (unsigned char)7, (unsigned char)11, saveStream);
	saveToFile((unsigned char)7, (unsigned char)7, (unsigned char)9, saveStream);
	saveStream.close();
}