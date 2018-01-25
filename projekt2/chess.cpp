#include "stdafx.h"
#include "chess.h"
#include "chessInsideFunctions.h"
#include "stack.h"

void startAGame(game &startingGame) {
	using namespace std;
	//tworzenie szachownicy
	ifstream loadingStream;
	loadingStream.open(STARTING_CHESSBOARD_LINK, ios_base::binary);
	loadingStream.read((char*)&*(startingGame.whiteKingPos), sizeof *(startingGame.whiteKingPos));
	loadingStream.read((char*)&*(startingGame.whiteKingPos + 1), sizeof *(startingGame.whiteKingPos + 1));
	loadingStream.read((char*)&*(startingGame.blackKingPos), sizeof *(startingGame.blackKingPos));
	loadingStream.read((char*)&*(startingGame.blackKingPos + 1), sizeof *(startingGame.blackKingPos + 1));
	loadingStream.read((char*)&*(startingGame.whiteCastlesChances), sizeof *(startingGame.whiteCastlesChances));
	loadingStream.read((char*)&*(startingGame.whiteCastlesChances + 1), sizeof *(startingGame.whiteCastlesChances + 1));
	loadingStream.read((char*)&*(startingGame.blackCastlesChances), sizeof *(startingGame.blackCastlesChances));
	loadingStream.read((char*)&*(startingGame.blackCastlesChances + 1), sizeof *(startingGame.blackCastlesChances + 1));
	if (UNDO_LIMIT == 1) {
		startingGame.changesStack = createChangesStack(UNDO_LIMIT_HEIGHT);
	}
	else {
		startingGame.changesStack = createChangesStack();
	}
	unsigned char x;
	unsigned char y;
	unsigned char chessman;
	startingGame.chessBoard = new unsigned char*[C_SIZE];
	for (int i = 0; i < C_SIZE; ++i) {

		*(startingGame.chessBoard + i) = new unsigned char[C_SIZE];
		for (int j = 0; j < C_SIZE; ++j) {

			*(*(startingGame.chessBoard + i) + j) = 0;
		}
	}
	//wczytywanie figur
	while (!loadingStream.eof()) {
		loadingStream.read((char*)&x, sizeof x);
		loadingStream.read((char*)&y, sizeof y);
		loadingStream.read((char*)&chessman, sizeof chessman);
		*(*(startingGame.chessBoard + y) + x) = chessman;
	}
}

void show(unsigned char** chessBoard) {
	std::cout << "Biali:    Czarni:"<<std::endl<<std::endl;
	for (int i = 0; i < C_SIZE; i++) {
		for (int j = 0; j < C_SIZE; j++) {
			showChessman(*(chessBoard+ C_SIZE -1-i) + j);
		}
		std::cout << "  ";
		for (int j = 0; j < C_SIZE; j++) {
			showChessman(*(chessBoard + i)+ C_SIZE -1-j);
		}
		std::cout << std::endl;
	}
}

void showChessman(unsigned char* chessman) {
	switch (*chessman) {
	case 0: std::cout << " "; break;
	case KING_ID: std::cout << "K"; break;
	case QUEEN_ID: std::cout << "Q"; break;
	case ROCK_ID: std::cout << "R"; break;
	case BISHOP_ID: std::cout << "B"; break;
	case KNIGHT_ID: std::cout << "N"; break;
	case PAWD_ID: std::cout << "P"; break;
	case KING_ID+ CHESSMANS_SIZE: std::cout << "k"; break;
	case QUEEN_ID+ CHESSMANS_SIZE: std::cout << "q"; break;
	case ROCK_ID+ CHESSMANS_SIZE: std::cout << "r"; break;
	case BISHOP_ID+ CHESSMANS_SIZE: std::cout << "b"; break;
	case KNIGHT_ID+ CHESSMANS_SIZE: std::cout << "n"; break;
	case PAWD_ID+ CHESSMANS_SIZE: std::cout << "p"; break;
	}
}

void finishGame(struct game &game) {
	for (int i = 0; i < C_SIZE; ++i) {
		delete[] game.chessBoard[i];

	}
	delete[] game.chessBoard;
	deleteChangesStack(game.changesStack);
}

void sendCommunique(std::string communique) {
	std::cout << communique << std::endl;
}

std::string waitForAnswer() {
	std::string answer;
	std::cin >> answer;
	return answer;
}


bool identifyCM(const char* chessman, unsigned char* target) {
	for(int i=0;i<CHESSMANS_SIZE -1;++i){
		if (*chessman == *(chessmans + i)) {
			*target=i+1;
			return true;;
	}
	}
	*target = 0;
	return false;
}

bool identifyY(const char* y,  unsigned char* target) {
	unsigned char value = *y - NUMBER_ONE;
	if (value <C_SIZE) {//czy oba porownania sa potrzebne??
		*target = value;
		return true;
	}
	return false;
}

bool identifyX(const char* x,  unsigned char* target) {
	unsigned char value = *x - LOWER_A;
	if (value <C_SIZE) {//czy oba porownania sa potrzebne??
		*target = value;
		return true;
	}
	return false;
}

void operateGame(struct game game) {
	std::string move;
	bool running = true;
	while (running) {
		show(game.chessBoard);
		sendCommunique("");
		if (game.whoToMove) {
			sendCommunique("Ruch bia³ych");
		}
		else {
			sendCommunique("Ruch czarnych");
		}
		sendCommunique("");
		move = waitForAnswer();
		sendCommunique("");
		if (move == "undo") {
			if (undo(game)) {
				sendCommunique("Cofniêto zmiany");
			}
			else {
				sendCommunique("Nie mo¿na cofn¹æ");
			}
		}
		else if (move == "zakoncz") {
			finishGame(game);
			running = false;
		}
		else {

			switch (executeMove(game, move)) {
			case 0:
				break;
			case 1:
				sendCommunique("Niepoprawny zapis");
				sendCommunique("");
				break;
			case 3:
				sendCommunique("Taka figura nie istnieje");
				sendCommunique("");
				break;

			case 4:
				sendCommunique("Nie mo¿na promowaæ figury");
				sendCommunique("");
				break;
			case 6:
				sendCommunique("Na tym polu nie znajduje siê przeciwna figura");
				sendCommunique("");
				break;
			case 7:
				sendCommunique("Te pole nie jest puste");
				sendCommunique("");
				break;
			case 8:
				sendCommunique("Wiêcej ni¿ 1 figura mo¿e zostaæ zagrana na to pole. Przoszê sprecyzowaæ ruch.");
				sendCommunique("");
				break;
			case 9:
				sendCommunique("¯adna figura nie mo¿e zostaæ zagrana na to pole");
				sendCommunique("");
				break;
			case 10:
				sendCommunique("Twój król znalaz³by siê w szachu");
				sendCommunique("");
				break;
			case 11:
				sendCommunique("Szach Mat!");
				sendCommunique("");
				break;
			case 12:
				sendCommunique("Nie mo¿na dokonaæ tej roszady, gdy¿ na polu pomiêdzy królem i wie¿¹ znajduje siê figura");
				sendCommunique("");
				break;
			case 13:
				sendCommunique("Nie mo¿na dokonaæ tej roszady, gdy¿ jedno z pól jest szachowane");
				sendCommunique("");
				break;
			case 14:
				sendCommunique("Nie mo¿na dokonaæ tej roszady, gdy¿ wie¿a lub król zosta³a wczeœniej poruszona");
				sendCommunique("");
				break;
			case 15:
				sendCommunique("Nieprawid³owe bicie w przelocie");
				sendCommunique("");
				break;
			case 16:
				sendCommunique("Szach!");
				sendCommunique("");
				break;

			default:
				sendCommunique("Niezdefiniowany b³¹d");
				sendCommunique("");
				break;
			}
		}
	}
	

}
unsigned char executeMove(struct game &game, std::string move) {
	unsigned char changeArray1[3] ;
	unsigned char changeArray2[3] ;
	unsigned char changeArray3[3];
	changesSet undoChanges;
	char mLength = move.length();
	unsigned char place0TextLength;
	unsigned char place0Knowledge;
	bool passant;
	bool hit;
	unsigned char promote;
	const char* moveCStr = move.c_str();
	switch (checkPromote(moveCStr, &promote, &mLength)) {
	case 0:
		break;
	default:
		return 3;

	};
	checkPassant(moveCStr, &passant, &mLength);
	if (identifyX(moveCStr + mLength - 2, changeArray2) && identifyY(moveCStr + mLength - 1, changeArray2 + 1)) {// czy jest to normalne zagranie

		switch (recognizeStandardMove(changeArray1, changeArray2, moveCStr, &mLength, &place0Knowledge, &place0TextLength, &hit, &game.whoToMove)) {
		case 0:
			break;
		default:
			return 1;
		}

		switch (checkHitCorrect(game.chessBoard, changeArray2, &passant,hit, game.whoToMove)) {
		case 0:
			break;
		case 6:
			return 6;
		case 7:
			return 7;
		}
	
		
		switch (findPlace0(game.chessBoard, changeArray1, changeArray2, &place0Knowledge, hit)) {
			case 0://istnieje taka figura
				break;
			case 8:
				return 8;
			case 9:
				return 9;
			
			default://jakis b³¹d
				return 1;
			}

		if (passant) {
			if (game.changesStack.nextElement != nullptr) {
			switch (checkPassantCorrect(game.changesStack.nextElement->changesSet, game.chessBoard, changeArray2)) {
			case 0:
				break;
			case 15:
				return 15;
			}
			undoChanges.passant = true;
			*changeArray3 = *changeArray2;
			*(changeArray3 + 1) = *(changeArray1 + 1);
			*(changeArray3 + 2) = 0;
			createUndoChangeArray(undoChanges.change3, game.chessBoard, changeArray3);
			executeChange(changeArray3, game.chessBoard);
		}
			else {
				return 1;
			}
		}

		switch (operatePromote( changeArray2, &promote, &game.whoToMove)) {
		case 0://istnieje taka figura
			break;
		case 5:
			return 5;
		}


		*(changeArray1 + 2) = 0;
		undoChanges.castle = false;


	}
	else {
		if (promote == 0) {
			if (strcmp(move.c_str(), "O-O") == 0) {
				unsigned char changeArray4[3];
				if (game.whoToMove) {
					switch (recognizeCastle(game.chessBoard, changeArray1, changeArray2, changeArray3, changeArray4, game.whoToMove, true, game.whiteCastlesChances)) {
					case 0: break;
					case 10: return 10;
					case 12: return 12;
					case 13: return 13;
					}
					
				}
				else {
					switch (recognizeCastle(game.chessBoard, changeArray1, changeArray2, changeArray3, changeArray4, game.whoToMove, true, game.blackCastlesChances)) {
					case 0: break;
					case 10: return 10;
					case 12: return 12;
					case 13: return 13;
					}
				}
				undoChanges.castle = true;
				createUndoChangeArray(undoChanges.change3, game.chessBoard, changeArray3);
				createUndoChangeArray(undoChanges.change4, game.chessBoard, changeArray4);
				executeChange(changeArray3, game.chessBoard);
				executeChange(changeArray4, game.chessBoard);
			}
			else if (strcmp(move.c_str(), "O-O-O")==0) {
				unsigned char changeArray4[3];
				if (game.whoToMove) {
					switch (recognizeCastle(game.chessBoard, changeArray1, changeArray2, changeArray3, changeArray4, game.whoToMove, false, game.whiteCastlesChances)) {
					case 0: break;
					case 10: return 10;
					case 12: return 12;
					case 13: return 13;
					}



				}
				else {
					switch (recognizeCastle(game.chessBoard, changeArray1, changeArray2, changeArray3, changeArray4, game.whoToMove, false, game.blackCastlesChances)) {
					case 0: break;
					case 10: return 10;
					case 12: return 12;
					case 13: return 13;
					}
				}
				undoChanges.castle = true;
				createUndoChangeArray(undoChanges.change3, game.chessBoard, changeArray3);
				createUndoChangeArray(undoChanges.change4, game.chessBoard, changeArray4);
				executeChange(changeArray3, game.chessBoard);
				executeChange(changeArray4, game.chessBoard);
			}
			else {//niepoprawny zapis
				return 1;
			}

		}
		else {
			return 1;
		}
	}
	if (game.whoToMove) {
		secureKingsMovementAndCastles(game.whiteKingPos, game.whiteCastlesChances, game.blackCastlesChances, changeArray1, changeArray2, undoChanges.kingsPrevPosition, undoChanges.whiteCastlesPrevChances, undoChanges.blackCastlesPrevChances, true);
	}
	else {
		secureKingsMovementAndCastles(game.blackKingPos, game.whiteCastlesChances, game.blackCastlesChances, changeArray1, changeArray2, undoChanges.kingsPrevPosition, undoChanges.whiteCastlesPrevChances, undoChanges.blackCastlesPrevChances, false);
	}
	createUndoChangeArray(undoChanges.change1,game.chessBoard,changeArray1);
	createUndoChangeArray(undoChanges.change2, game.chessBoard, changeArray2);
	saveChanges(game.changesStack, undoChanges);
	executeChange(changeArray1, game.chessBoard); 
	executeChange(changeArray2, game.chessBoard);
	if (game.whoToMove) {
		game.whoToMove = false;
		if (isChecked(game.chessBoard, game.whiteKingPos, false,true)) {
			undo(game);
			return 10;
		}
		switch (isCheckMate(game.chessBoard, game.blackKingPos, true)) {
		case 1: return 16;
		case 2: return 11;
		case 0: return 0;
		}
		if (isCheckMate(game.chessBoard, game.blackKingPos, true)==2) {
			return 11;
		}
	}
	else {
		game.whoToMove = true;
		if (isChecked(game.chessBoard, game.blackKingPos, true,true)) {
			undo(game);
			return 10;
		}
		switch (isCheckMate(game.chessBoard, game.blackKingPos, true)) {
		case 1: return 16;
		case 2: return 11;
		case 0: return 0;
		}

	}
	return 0;
	}

unsigned char recognizeCastle(unsigned char** chessBoard, unsigned char* changesArray1, unsigned char* changesArray2, unsigned char* changesArray3, unsigned char* changesArray4, bool side,bool kingSide,bool* castlesChances) {

	unsigned char sideDifference;
	unsigned char currentSquare[2];
	if (side) {
		*(currentSquare+1) = 0;
		sideDifference = 0;
	}
	else {
		*(currentSquare + 1) = C_SIZE -1;
		sideDifference = 6;
	}
	if (kingSide) {
		if (!*(castlesChances)) {	
			return 10;//nie mozna dokonac tej roszady ze wzgledu na wczesniejsze poruszenie wiezy lub krola
		}
		if (!(findChessmanOnXYPlace(chessBoard,5, *(currentSquare + 1))==0 && findChessmanOnXYPlace(chessBoard, 6,*(currentSquare + 1))==0)) {//magic numbers?
			return 12;//nie mozna dokonac tej roszady, gdyz na polu pomiedzy wieza i krolem znajduje sie figura
		}

		for (*currentSquare = 4; *currentSquare <= C_SIZE - 1; ++*currentSquare) {//jedno z pól jest szachowane
			if (isChecked(chessBoard, currentSquare, !side, true)) {
				return 13;
			}
		}
		*changesArray1 = 4;
		*(changesArray1 + 1) = *(currentSquare + 1);
		*(changesArray1 + 2) = 0;
		*changesArray2 = 6;
		*(changesArray2 + 1) = *(currentSquare + 1);
		*(changesArray2 + 2) = 1+sideDifference;
		*changesArray3 = 7;
		*(changesArray3 + 1) = *(currentSquare + 1);
		*(changesArray3 + 2) = 0;
		*changesArray4 = 5;
		*(changesArray4 + 1) = *(currentSquare + 1);
		*(changesArray4 + 2) = 3+sideDifference;//magic?

		

	}
	else {
		if (!*(castlesChances+1)) {
			return 14;//nie mozna dokonac tej roszady ze wzgledu na wczesniejsze poruszenie wiezy lub krola
		}
		if (!(findChessmanOnXYPlace(chessBoard,1,*(currentSquare + 1))==0 && findChessmanOnXYPlace(chessBoard,2, *(currentSquare + 1))==0 && findChessmanOnXYPlace(chessBoard,3, *(currentSquare + 1))==0)) {//magic numbers?
			return 12;//nie mozna dokonac tej roszady, gdyz na polu pomiedzy wieza i krolem znajduje sie figura
		}
		
		for (*currentSquare = 0; *currentSquare <= 4; ++*currentSquare) {//jedno z pól jest szachowane
			if (isChecked(chessBoard, currentSquare, !side, true)) {
				return 13;
			}


		}
		//tutaj sprawdzenie szachu jeszcze !!!
		*changesArray1 = 4;
		*(changesArray1 + 1) = *(currentSquare + 1);
		*(changesArray1 + 2) = 0;
		*changesArray2 = 2;
		*(changesArray2 + 1) = *(currentSquare + 1);
		*(changesArray2 + 2) = 1 + sideDifference;
		*changesArray3 = 0;
		*(changesArray3 + 1) = *(currentSquare + 1);
		*(changesArray3 + 2) = 0;
		*changesArray4 = 3;
		*(changesArray4 + 1) = *(currentSquare + 1);
		*(changesArray4 + 2) = 3 + sideDifference;//magic?

	}
	return 0;
}

void createUndoChangeArray(unsigned char* undoChangeArray,unsigned char** chessBoard, unsigned char* changeArray) {
	*(undoChangeArray) = *(changeArray);
	*(undoChangeArray+1) = *(changeArray+1);
	*(undoChangeArray+2) = *(*(chessBoard + *(changeArray+1)) + *changeArray);
}
void createUndoKingsMovement(unsigned char* kingsPrevPosition,unsigned char* kingPos) {
	*(kingsPrevPosition) = *(kingPos);
	*(kingsPrevPosition +1) = *(kingPos+1);
}
void createUndoCastlesChances(bool* prevCastlesChances,bool* castlesChances ) {
	*(prevCastlesChances) = *(castlesChances);
	*(prevCastlesChances+1) = *(castlesChances+1);
}
void secureKingsMovementAndCastles(unsigned char* kingPos, bool* whiteCastlesChances, bool* blackCastlesChances, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* kingsPrevPosition, bool* whiteCastlesPrevChances, bool* blackCastlesPrevChances,bool side) {
		createUndoKingsMovement(kingsPrevPosition, kingPos);
		createUndoCastlesChances(whiteCastlesPrevChances,whiteCastlesChances);
		createUndoCastlesChances(blackCastlesPrevChances, blackCastlesChances);
	if (*(changeArray2 + 2) == 1) {
		*kingPos = *changeArray2;
		*(kingPos+1) = *(changeArray2+1);
		*whiteCastlesChances = false;
		*(whiteCastlesChances+1) = false;
	}
	else if (*(changeArray2 + 2) == 1 + CHESSMANS_SIZE) {
		*kingPos = *changeArray2;
		*(kingPos + 1) = *(changeArray2 + 1);
		*blackCastlesChances = false;
		*(blackCastlesChances + 1) = false;

	}
	else {
		if (side) {
			if (*(changeArray1) == 0 && *(changeArray1 + 1) == 0) {
				*(whiteCastlesChances + 1) = false;
			}			
			else if (*(changeArray1) == C_SIZE -1 && *(changeArray1 + 1) == 0) {
				*(whiteCastlesChances) = false;
			}
			if (*(changeArray2) == 0 && *(changeArray2 + 1) == C_SIZE -1) {
				*(blackCastlesChances + 1) = false;
			}
			else if (*(changeArray2) == C_SIZE - 1 && *(changeArray2 + 1) == C_SIZE -1) {
				*(blackCastlesChances) = false;
			}

		}
		else {
			if (*(changeArray2) == 0 && *(changeArray2 + 1) == 0) {
				*(whiteCastlesChances + 1) = false;
			}
			else if (*(changeArray2) == C_SIZE - 1 && *(changeArray2 + 1) == 0) {
				*(whiteCastlesChances) = false;
			}
			if (*(changeArray1) == 0 && *(changeArray1 + 1) == C_SIZE - 1) {
				*(blackCastlesChances + 1) = false;
			}
			else if (*(changeArray1) == C_SIZE - 1 && *(changeArray1 + 1) == C_SIZE - 1) {
				*(blackCastlesChances) = false;
			}
		}
	}
}

unsigned char recognizeStandardMove(unsigned char* changeArray1, unsigned char* changeArray2, const char* move, char* mLength, unsigned char* place0Knowledge, unsigned char* place0TextLength, bool* hit, bool* side) {

			if (*(move + (*mLength - 3)) == captureSign) {//czy jest to zbicie
				*hit = true;
				if (identifyX(move, changeArray1)) {//1.1.1 zbicie pionkiem
					*place0Knowledge = 1;
					if (*side) {//strona bia³ych
						*(changeArray2 + 2) = CHESSMANS_SIZE;
					}
					else {//strona czarnych
						*(changeArray2 + 2) = CHESSMANS_SIZE * 2;
					}
				}
				else if (identifyCM(move, changeArray2 + 2)) {//1.1.2 zbicie figur¹
					*place0TextLength = *mLength - 4;
					*place0Knowledge = identifyPlace0Knowledge(changeArray1, move + 1, place0TextLength);

					if (!*side) {//strona czarnych
						*(changeArray2 + 2) += CHESSMANS_SIZE;
					}
				}
				else {//niepoprawny zapis B£¥D!!
					return 1;
				}

			}
			else {// czy jest to ruch
				*hit = false;
				if (identifyCM(move, changeArray2 + 2)) {//1.2.1 ruch figur¹
					*place0TextLength = *mLength - 3;
					*place0Knowledge = identifyPlace0Knowledge(changeArray1, move + 1, place0TextLength);
					if (!*side) {//strona czarnych
						*(changeArray2 + 2) += CHESSMANS_SIZE;
					}
				}
				else if (*mLength == 2) {//1.2.2 ruch pionem
					*changeArray1 = *changeArray2;
					*place0Knowledge = 0;
					if (*side) {//strona bia³ych
						*(changeArray2 + 2) = CHESSMANS_SIZE;
					}
					else {//strona czarnych
						*(changeArray2 + 2) = CHESSMANS_SIZE * 2;
					}
				}
				else {//niepoprawny zapis B£¥D!!
					return 1;
				}


			}
			if (*place0Knowledge == 4) {//niepoprawny zapis B£¥D!!
				return 1;
			}
			return 0;



		}

unsigned char checkHitCorrect(unsigned char** chessBoard, unsigned char* changeArray2,bool* passant, bool hit,bool side) {
	unsigned char focusedChessman;
	if (*passant) {
		if (side) {
			focusedChessman = *(*(chessBoard + *(changeArray2 + 1)-1) + *changeArray2);

		}
		else {
			focusedChessman = *(*(chessBoard + *(changeArray2 + 1) + 1) + *changeArray2);
		}
	}
	else {
		focusedChessman = *(*(chessBoard + *(changeArray2 + 1)) + *changeArray2);
	}
	if (hit) {
		if (side) {
			if (!(focusedChessman >= CHESSMANS_SIZE + 1 && focusedChessman <= CHESSMANS_SIZE * 2)) {//na tym polu nie znajduje siê przeciwna figura
					focusedChessman = *(*(chessBoard + *(changeArray2 + 1) - 1) + *changeArray2);
					if (!(focusedChessman >= CHESSMANS_SIZE + 1 && focusedChessman <= CHESSMANS_SIZE * 2)) {//na tym polu nie znajduje siê przeciwna figura
						return 6;
					}
					*passant = true;
			}

		}
		else {
			if (!(focusedChessman >= 1 && focusedChessman <= CHESSMANS_SIZE)) {//na tym polu nie znajduje siê przeciwna figura
					focusedChessman = *(*(chessBoard + *(changeArray2 + 1) + 1) + *changeArray2);
					if (!(focusedChessman >= CHESSMANS_SIZE + 1 && focusedChessman <= CHESSMANS_SIZE * 2)) {//na tym polu nie znajduje siê przeciwna figura
						return 6;
					}
					*passant = true;
			
			}
		}
	}
	else {
		if (!(focusedChessman == 0)) {//na tym polu znajduje siê figura
			return 7;
		}
	}
	return 0;
}
unsigned char operatePromote(unsigned char* changeArray2, unsigned char* promote,bool* side) {
	if (*promote!=0) {//sprawdzenie poprawnosci promocji
		if (*side) {
			if (*(changeArray2 + 1) != C_SIZE - 1) {//mozna promowac figury jedynie w ostatnim rzedzie
				return 5;

			}
		}
		else {
			if (*(changeArray2 + 1) != 0) {//mozna promowac figury jedynie w ostatnim rzedzie
				return 5;
			}
		}
		*(changeArray2 + 2) = *promote;
	}
	return 0;




}

unsigned char checkPromote(const char* move, unsigned char* promote,char* mLength) {
	if (*(move + (*mLength - 2)) == EQUAL_SIGN) {//czy jest promocja
		if (identifyCM(move, promote)) {
			*mLength -= 2;
		}
		else {//taka figura nie istnieje B£¥D!!
			return 3;
		}
	}
	else {
		*promote = 0;
	}
	return 0;
}
void executeChange(unsigned char* changeArray, unsigned char** chessBoard) {
		*(*(chessBoard + *(changeArray + 1)) + *changeArray) = *(changeArray + 2);
	}

unsigned char identifyPlace0Knowledge(unsigned char* changeArray, const char* start,unsigned char* size){
	switch (*size) {
	case 0:
		return 0;
	case 1:
		if (identifyX(start, changeArray)) {
			return 1;
			}
		else if (identifyY(start, changeArray + 1)) {
			return 2;
			}
		return 4;
	case 2:
		if (identifyX(start, changeArray) && identifyY(start + 1, changeArray + 1)) {
			return 3;
			}
		return 4;
	default:
		return 4;
	}
}

unsigned char findPlace0(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool hit){
	switch (*(changeArray2 + 2)) {
	case KING_ID:
		return findKing(chessBoard, changeArray1, changeArray2,place0Knowledge, true);
	case QUEEN_ID:
		return findQueen(chessBoard, changeArray1, changeArray2, place0Knowledge, true);
	case ROCK_ID:
		return findRock(chessBoard, changeArray1, changeArray2, place0Knowledge, true);
	case BISHOP_ID:
		return findBishop(chessBoard, changeArray1, changeArray2, place0Knowledge, true);
	case KNIGHT_ID:
		return findKnight(chessBoard, changeArray1, changeArray2, place0Knowledge, true);
	case PAWD_ID:
		return findPawn(chessBoard, changeArray1, changeArray2,  hit, true);
	case KING_ID+ CHESSMANS_SIZE:
		return findKing(chessBoard, changeArray1, changeArray2, place0Knowledge, false);
	case QUEEN_ID + CHESSMANS_SIZE:
		return findQueen(chessBoard, changeArray1, changeArray2, place0Knowledge, false);
	case ROCK_ID + CHESSMANS_SIZE:
		return findRock(chessBoard, changeArray1, changeArray2, place0Knowledge, false);
	case BISHOP_ID + CHESSMANS_SIZE:
		return findBishop(chessBoard, changeArray1, changeArray2, place0Knowledge, false);
	case KNIGHT_ID + CHESSMANS_SIZE:
		return findKnight(chessBoard, changeArray1, changeArray2, place0Knowledge, false);
	case PAWD_ID + CHESSMANS_SIZE:
		return findPawn(chessBoard, changeArray1, changeArray2, hit, false);
		
	default:
		return 1;
	}


}

void checkPassant(const char* move, bool* passant, char* mLength) {
	if (strcmp(move+4, "e.p.")==0) {//magic number?
		*mLength -=4;
		*passant = true;
	}
	else {
		*passant = false;
	}
}

unsigned char checkPassantCorrect(changesSet changesSet, unsigned char** chessBoard,unsigned char* changeArray2) {
	if (*(changeArray2 + 2) == PAWD_ID) {//magic number??
		if (*changesSet.change2 == *changeArray2&&*(changesSet.change1 + 1) == 6 && *(changesSet.change2 + 1) == 4 && *(changesSet.change1 + 2) == 6 + CHESSMANS_SIZE) {//else
			return 0;
		}
	}
	else if (*(changeArray2 + 2) == PAWD_ID + CHESSMANS_SIZE) {
		if (*changesSet.change2 == *changeArray2&&*(changesSet.change1 + 1) == 1 && *(changesSet.change2 + 1) == 3 && *(changesSet.change1 + 2) == 6) {//else
			return 0;
		}

	}
	return 15;
}

unsigned char findChessmanOnXYPlace(unsigned char** chessBoard, unsigned char x, unsigned char y, unsigned char* chessman) {
	unsigned char target = *(*(chessBoard + y ) + x);
	if (target == *chessman) {
		return 1;
	}
	if (target == 0) {
		return 0;
	}
	return 2;
}

unsigned char findChessmanOnXYPlace(unsigned char** chessBoard, unsigned char x, unsigned char y) {
	unsigned char target = *(*(chessBoard + y ) + x);
	return target;
}

unsigned char findBishop(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side) {
	switch (findOnDiagonal(chessBoard, changeArray1, changeArray2, place0Knowledge, side, true)) {
	case 0:
		return 0;
	case 8:
		return 8;//wiecej niz 1 figura
	default:
			return 9;//zadna figura
	}
}
unsigned char findRock(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side) {
	switch (findOnStraightLine(chessBoard, changeArray1, changeArray2, place0Knowledge, side, true)) {
	case 0:
		return 0;
	case 8:
		return 8;//wiecej niz 1 figura
	default:
		return 9;//zadna figura
	}
}

unsigned char findQueen(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side) {
	bool found=false;
	switch (findOnDiagonal(chessBoard, changeArray1, changeArray2, place0Knowledge, side, true)) {
	case 0:
		found=true;
		break;
	case 8:
		return 8;//wiecej niz 1 figura
	}
	switch (findOnStraightLine(chessBoard, changeArray1, changeArray2, place0Knowledge, side, true)) {
	case 0:
		if (!found) {
			return 0;
		}
		else {
			return 8;
		}
	case 8:
		return 8;//wiecej niz 1 figura
	}
	if (found) {
		return 0;
	}
	else {
		return 9;
	}

}

unsigned char findKnight(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side) {
	bool found = false;
	bool Ycorrect;
	bool Xcorrect;
	char Xjump;
	char Yjump;
	char XjumpWithSign;
	char YjumpWithSign;
	unsigned char currentX;
	unsigned char currentY;
	unsigned char searchedChessman;
	if (side) {
		searchedChessman = KNIGHT_ID;
	}
	else {
		searchedChessman = KNIGHT_ID + CHESSMANS_SIZE;
	}
	for (int k = 0; k < 2; k++) {//MAGIC NUMBER(?)
		if (k == 0) {
			Yjump = KNIGHT_JUMP_LD;
			Xjump = KNIGHT_JUMP_SD;
			}
		else {
			Yjump = KNIGHT_JUMP_SD;
			Xjump = KNIGHT_JUMP_LD;
			}

		for (int i = 0; i < 2; ++i) {//magic number??
			Ycorrect = true;
			if (i == 0) {
				YjumpWithSign = Yjump;
			}
			else {
				YjumpWithSign = -Yjump;
			}
			if (*place0Knowledge == 2 || *place0Knowledge == 3) {
				if (*(changeArray1 + 1) != *(changeArray2 + 1) + YjumpWithSign) {
					Ycorrect = false;
					}
				else {
					currentY = *(changeArray1 + 1);
					}
				}
			else {
				currentY = *(changeArray2 + 1) + YjumpWithSign;
				if (!(currentY < C_SIZE)) {// sprawdzenie czy to pole znajduje siê na szachownicy 
					Ycorrect = false;
					}
				}
			if (Ycorrect) {
				for (int j = 0; j < 2; ++j) {
					if (j == 0) {
						XjumpWithSign = Xjump;
					}
					else {
						XjumpWithSign = -Xjump;
					}
					Xcorrect = true;
					if (*place0Knowledge == 1 || *place0Knowledge == 3) {
						if (*changeArray1 != *changeArray2 + XjumpWithSign) {
							Xcorrect = false;
							}
						else {
							currentX = *changeArray1;
						}
						}
					else {
						currentX = *changeArray2 + XjumpWithSign;
						if (!(currentX<C_SIZE)) {// sprawdzenie czy to pole znajduje siê na szachownicy 
							Xcorrect = false;
							}
						}
					if (Xcorrect) {
						if (findChessmanOnXYPlace(chessBoard, currentX,currentY, &searchedChessman) == 1) {
							if (!found) {
								*changeArray1 = currentX;
								*(changeArray1+1) = currentY;
								found = true;
								}
							else {//wiecej niz 1 figura spelniajaca za³o¿enia
								return 8;
								}
							}

						}
					}
				}
			}
		}
	if (found) {
		return 0;
		}
	else {//zadna figura nie moze pojawic sie na tym polu
		return 9;
		}
	}

unsigned char findKing(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side) {
	char changeX;
	char changeY;
	unsigned char currentX;
	unsigned char currentY;
	unsigned char searchedChessman;
	if (side) {
		searchedChessman = KING_ID;
	}
	else {
		searchedChessman = KING_ID + CHESSMANS_SIZE;
	}
	for (unsigned char i = 0; i < 9; ++i) {//magic number??
		if (i == 4) {
			i++;
		}
		if (i % 3 == 0) {
			changeY = -1;
		}
		else if(i % 3 == 1) {
			changeY = 0;
		}
		else {
			changeY = 1;
		}
		if (i <3) {
			changeX = -1;
		}
		else if (i < 6) {
			changeX = 0;
		}
		else {
			changeX = 1;
		}
		currentX = *(changeArray2)+changeX;
		currentY = *(changeArray2+1)+changeY;
		if (currentX < C_SIZE&&currentY < C_SIZE) {
			if (findChessmanOnXYPlace(chessBoard, currentX, currentY, &searchedChessman) == 1) {
				*(changeArray1) = currentX;
				*(changeArray1 + 1) = currentY;
				return 0;
			}
		}
	}
	return 9;//nie ma takiej figury
}

unsigned char findPawn(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, bool hit, bool side) {
	unsigned char searchedChessman;
	if (side) {
		searchedChessman = PAWD_ID;
	}
	else {
		searchedChessman = PAWD_ID + CHESSMANS_SIZE;
	}
	if (hit) {
			if (side) {//strona bialych
				*(changeArray1 + 1) = *(changeArray2 + 1) - 1;
			}
			else {//strona czarnych
				*(changeArray1 + 1) = *(changeArray2 + 1) + 1;
			}
			if (!(*(changeArray1 + 1) < C_SIZE)) {//zadna figura nie moze pojawic sie na tym polu
				return 9;
			}
			if (findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), &searchedChessman) == 1) {
				return 0;
			}
		

		//else if()//dodac passant
		else {//zadna figura nie moze pojawic sie na tym polu
			return 9;
		}
	}
	else {
		if (side) {
			*(changeArray1 + 1) = *(changeArray2 + 1) - 1;
			if (!(*(changeArray1 + 1) < C_SIZE)) {//zadna figura nie moze pojawic sie na tym polu
				return 9;
			}
			if (findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2)==1) {
				return 0;
			}
			else if(*(changeArray2+1)==3&& findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2) == 0) {//Magic Number (?)
				*(changeArray1 + 1) -=1;
				if (findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2)==1) {
					return 0;
				}
			}
			else {//zadna figura nie moze pojawic sie na tym polu
				return 9;
			}
		}
		else {
			*(changeArray1 + 1) = *(changeArray2 + 1) + 1;
			if (!(*(changeArray1 + 1) < C_SIZE)) {//zadna figura nie moze pojawic sie na tym polu
				return 9;
			}
			if (findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2)==1) {
				return 0;
			}
			else if (*(changeArray2 + 1) == C_SIZE -4&& findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2) == 0) {//Magic Number (?)
				*(changeArray1 + 1) += 1;
				if (findChessmanOnXYPlace(chessBoard, *changeArray1, *(changeArray1 + 1), changeArray2 + 2)==1) {
					return 0;
				}
			}
			else {//zadna figura nie moze pojawic sie na tym polu
				return 9;
			}
		}
	}
}

unsigned char findOnDiagonal(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side,bool isChessmanSet) {
	char changeX;
	char changeY;
	bool found = false;
	unsigned char currentSquare;
	unsigned char currentX;
	unsigned char currentY;
	unsigned char loopCounter;
	unsigned char loopLimit;
	unsigned char loopIncrementing;
	unsigned char searchedChessman;
	bool correctSquare;

	if (*place0Knowledge == 1 || *place0Knowledge == 3) {
		if (*changeArray1 > *changeArray2) {
			loopCounter = 2;
			loopLimit = 3;
		}
		else {
			loopCounter = 0;
			loopLimit = 1;
		}

	}
	else {
		loopCounter = 0;
		loopLimit = 3;
	}
	if (*place0Knowledge == 2 || *place0Knowledge == 3) {
		loopIncrementing = 2;
		if (*(changeArray1 + 1) > *(changeArray2 + 1)) {
			loopCounter++;
		}
	}
	else {
		loopIncrementing = 1;
	}
	while(loopCounter<=loopLimit) {//MagicNumber (?) //ruch po ukosie
		if (loopCounter % 2 == 0) {
			changeY = -1;
		}
		else {
			changeY = 1;
		}

		if (loopCounter <2) {
			changeX = -1;
		}
		else {
			changeX = 1;
		}

		currentX = *changeArray2;
		currentY = *(changeArray2 + 1);
		currentSquare = 2;
		do {
			currentX += changeX;
			currentY += changeY;
			if (!(currentX < C_SIZE&&currentY < C_SIZE)) {
				break;
			}
			if (isChessmanSet) {
				currentSquare = findChessmanOnXYPlace(chessBoard, currentX, currentY, changeArray2 + 2);
			}
			else {
				searchedChessman = QUEEN_ID;//magic number (?)
				if (!side) {
					searchedChessman += CHESSMANS_SIZE;
				}
				currentSquare = findChessmanOnXYPlace(chessBoard, currentX, currentY, &searchedChessman);
				if (currentSquare == 2) {
					searchedChessman += (BISHOP_ID- QUEEN_ID);//magic number (?)
					currentSquare = findChessmanOnXYPlace(chessBoard, currentX, currentY, &searchedChessman);
				}

			}
		} while (currentSquare == 0);
			if (currentSquare == 1) {
				correctSquare = true;
				if (*place0Knowledge == 1 || *place0Knowledge == 3) {
					if (*changeArray1 != currentX) {
						correctSquare = false;
					}
				}
				if (*place0Knowledge == 2 || *place0Knowledge == 3) {
					if (*(changeArray1+1) != currentY) {
						correctSquare = false;
					}
				}
				if (correctSquare) {
					if (!found) {
						found = true;
					}
					else {//wiecej niz 1 figura spelniajaca za³o¿enia TU JEST BUG
						return 8;
					}

		//			if (*place0Knowledge == 0 || *place0Knowledge == 2) {//czy warto te if'y
						*changeArray1 = currentX;
		//			}
		//			if (*place0Knowledge == 0 || *place0Knowledge == 1) {
						*(changeArray1 + 1) = currentY;
		//			}

				}
			}

		
		loopCounter += loopIncrementing;

	}
	if (found) {
		return 0;
	}
	else {//zadna figura nie moze pojawic sie na tym polu
		return 9;
	}
}

unsigned char findOnStraightLine(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side, bool isChessmanSet) {
	bool checkLine[4] = {true,true,true,true};
	unsigned char loopCounter;
	unsigned char loopLimit;
	char change;
	unsigned char current;
	unsigned char currentSquare;
	unsigned char searchedChessman;
	bool correctSquare;
	bool found = false;
	if (*place0Knowledge == 1 || *place0Knowledge == 3) {
		if (*changeArray1 < *changeArray2) {
			*(checkLine + 1) = false;
			*(checkLine + 2) = false;
			*(checkLine + 3) = false;
		}
		else if (*changeArray1 == *changeArray2) {
			*checkLine = false;
			*(checkLine + 1) = false;
		}
		else {
			*checkLine = false;
			*(checkLine + 2) = false;
			*(checkLine + 3) = false;
		}
	}
	if (*place0Knowledge == 2 || *place0Knowledge == 3) {
		if (*(changeArray1+1) < *(changeArray2 + 1)) {
			*(checkLine + 0) = false;
			*(checkLine + 1) = false;
			*(checkLine + 3) = false;
		}
		else if (*(changeArray1 + 1) == *(changeArray2 + 1)) {
			*(checkLine + 2) = false;
			*(checkLine + 3) = false;
		}
		else {
			*(checkLine + 0) = false;
			*(checkLine + 1) = false;
			*(checkLine + 2) = false;
		}
	}
	if (*(checkLine)) {
		loopCounter = 0;
	}
	else {
		loopCounter = 1;
	}
	if (*(checkLine+1)) {
		loopLimit = 1;
	}
	else {
		loopLimit = 0;
	}

	while (loopCounter <= loopLimit) {//MagicNumber (?) //ruch po ukosie
		if (loopCounter == 0) {
			change = -1;
		}
		else {
			change = 1;
		}

		current = *changeArray2;
		currentSquare = 2;
			do {
				current += change;
				if (!(current < C_SIZE)) {
					break;
				}
				if (isChessmanSet) {
					currentSquare = findChessmanOnXYPlace(chessBoard, current, *(changeArray2 + 1), changeArray2 + 2);
				}
				else {
					searchedChessman = QUEEN_ID;//magic number (?)
					if (!side) {
						searchedChessman += CHESSMANS_SIZE;
					}
					currentSquare = findChessmanOnXYPlace(chessBoard, current, *(changeArray2 + 1), &searchedChessman);
					if (currentSquare == 2) {
						searchedChessman += (ROCK_ID-QUEEN_ID);//magic number (?)
						currentSquare = findChessmanOnXYPlace(chessBoard, current, *(changeArray2 + 1), &searchedChessman);
					}

				}
			} while (currentSquare == 0);
			if (currentSquare == 1) {
				correctSquare = true;
				if (*place0Knowledge == 1 || *place0Knowledge == 3) {
					if (*changeArray1 != current) {
						correctSquare = false;
					}
				}
				if (*place0Knowledge == 2 || *place0Knowledge == 3) {
					if (*(changeArray1 + 1) != *(changeArray2+1)) {
						correctSquare = false;
					}
				}
				if (correctSquare) {
					if (!found) {
						found = true;
					}
					else {//wiecej niz 1 figura spelniajaca za³o¿enia TU JEST BUG
						return 8;
					}

					//			if (*place0Knowledge == 0 || *place0Knowledge == 2) {//czy warto te if'y
					*changeArray1 = current;
					//			}
					//			if (*place0Knowledge == 0 || *place0Knowledge == 1) {
					*(changeArray1 + 1) = *(changeArray2 + 1);
					//			}

				}
			}

		
		loopCounter ++;
	}
	if (*(checkLine+2)) {
		loopCounter = 0;
	}
	else {
		loopCounter = 1;
	}
	if (*(checkLine + 3)) {
		loopLimit = 1;
	}
	else {
		loopLimit = 0;
	}

	while (loopCounter <= loopLimit) {//MagicNumber (?) //ruch po ukosie
		if (loopCounter == 0) {
			change = -1;
		}
		else {
			change = 1;
		}

		current = *(changeArray2+1);
		currentSquare = 2;
		do {
			current += change;
			if (!(current < C_SIZE)) {
				break;
			}
			if (isChessmanSet) {
				currentSquare = findChessmanOnXYPlace(chessBoard, *changeArray2, current, changeArray2 + 2);
			}
			else {
				searchedChessman = QUEEN_ID;//magic number (?)
				if (!side) {
					searchedChessman += CHESSMANS_SIZE;
				}
				currentSquare = findChessmanOnXYPlace(chessBoard, *changeArray2, current, &searchedChessman);
				if (currentSquare == 2) {
					searchedChessman += (ROCK_ID -QUEEN_ID);//magic number (?)
					currentSquare = findChessmanOnXYPlace(chessBoard, *changeArray2, current, &searchedChessman);
				}

			}
		} while (currentSquare == 0);
		if (currentSquare == 1) {
			correctSquare = true;
			if (*place0Knowledge == 1 || *place0Knowledge == 3) {
				if (*changeArray1 != *changeArray2) {
					correctSquare = false;
				}
			}
			if (*place0Knowledge == 2 || *place0Knowledge == 3) {
				if (*(changeArray1 + 1) != current) {
					correctSquare = false;
				}
			}
			if (correctSquare) {
				if (!found) {
					found = true;
				}
				else {//wiecej niz 1 figura spelniajaca za³o¿enia TU JEST BUG
					return 8;
				}

				//			if (*place0Knowledge == 0 || *place0Knowledge == 2) {//czy warto te if'y
				*changeArray1 = *changeArray2;
				//			}
				//			if (*place0Knowledge == 0 || *place0Knowledge == 1) {
				*(changeArray1 + 1) = current;
				//			}

			}
		}


		loopCounter++;
	}
	if (found) {
		return 0;
	}
	else {//zadna figura nie moze pojawic sie na tym polu
		return 9;
	}
}

bool undo(game &game) {
	if (game.changesStack.size != 0) {
		changesSet changesSet = getFirstElement(game.changesStack);
		executeChange(changesSet.change1, game.chessBoard);
		executeChange(changesSet.change2, game.chessBoard);
		if (changesSet.castle) {
			executeChange(changesSet.change3, game.chessBoard);
			executeChange(changesSet.change4, game.chessBoard);
		}
		else if (changesSet.passant) {
			executeChange(changesSet.change3, game.chessBoard);
		}
		*game.whiteCastlesChances = *changesSet.whiteCastlesPrevChances;
		*(game.whiteCastlesChances + 1) = *(changesSet.whiteCastlesPrevChances + 1);
		*game.blackCastlesChances = *changesSet.blackCastlesPrevChances;
		*(game.blackCastlesChances + 1) = *(changesSet.blackCastlesPrevChances + 1);
		if (!game.whoToMove) {//cofniecie ruchu bia³ych
			*game.whiteKingPos = *changesSet.kingsPrevPosition;
			*(game.whiteKingPos+1) = *(changesSet.kingsPrevPosition+1);
			game.whoToMove = true;
		}
		else {//cofniecie ruchu czarnych
			*game.blackKingPos = *changesSet.kingsPrevPosition;
			*(game.blackKingPos + 1) = *(changesSet.kingsPrevPosition + 1);
			game.whoToMove = false;
		}
		return true;
	}
	return false;
}

bool isAttacked(unsigned char** chessBoard, unsigned char* foundSquare, unsigned char* place,bool whoAttack,bool kingChessCheck) {
	unsigned char place0Knowledge = 0;
	if (findKnight(chessBoard, foundSquare, place, &place0Knowledge, whoAttack) != 9) {
		return true;
	}
	if (findOnDiagonal(chessBoard, foundSquare, place, &place0Knowledge, whoAttack, false)!=9) {
		return true;
	}
	if (findOnStraightLine(chessBoard, foundSquare, place, &place0Knowledge, whoAttack, false)!=9) {
		return true;
	}
	bool hit = true;
	*foundSquare = *place - 1;
	if (*foundSquare < C_SIZE) {
		if (findPawn(chessBoard, foundSquare, place, &hit, whoAttack) != 9) {
			return true;
		}
	}
	*foundSquare = *place + 1;
	if (*foundSquare < C_SIZE) {
		if (findPawn(chessBoard, foundSquare, place, &hit, whoAttack) != 9) {
			return true;
		}
	}
	if (kingChessCheck) {
		if (findKing(chessBoard, foundSquare, place, &place0Knowledge, whoAttack) != 9) {
			return true;
		}
	}
	return false;
}

bool isChecked(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking, bool kingChessCheck) {
	unsigned char foundSquare[3];
	if (isAttacked(chessBoard, foundSquare, kingPos, whosChecking, kingChessCheck)) {
		return true;
	}
	return false;
}


bool canKingAvoidChess(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking) {
	char changeX;
	char changeY;
	unsigned char tmpChangeArray[3];
	unsigned char undoChangeArray[3];
	unsigned char currentSquare[2];
	unsigned char searchedChessman;
	unsigned char target;
	unsigned char sideDifference;
	*tmpChangeArray = *kingPos;
	*(tmpChangeArray + 1) = *(kingPos + 1);
	*(tmpChangeArray + 2) = 0;
	createUndoChangeArray(undoChangeArray, chessBoard, kingPos);
	executeChange(tmpChangeArray, chessBoard);
	if (whosChecking) {
		sideDifference = 0;
	}
	else {
		sideDifference = 6;
	}
	if (!whosChecking) {
		searchedChessman = 1;
	}
	else {
		searchedChessman = 1 + CHESSMANS_SIZE;
	}
	for (unsigned char i = 0; i < 9; ++i) {//magic number??
		if (i == 4) {
			i++;
		}
		if (i % 3 == 0) {
			changeY = -1;
		}
		else if (i % 3 == 1) {
			changeY = 0;
		}
		else {
			changeY = 1;
		}
		if (i <3) {
			changeX = -1;
		}
		else if (i < 6) {
			changeX = 0;
		}
		else {
			changeX = 1;
		}
		*currentSquare = *(kingPos)+changeX;
		*(currentSquare+1) = *(kingPos + 1) + changeY;
		if (*currentSquare < C_SIZE&&*(currentSquare+1) < C_SIZE) {
			target = findChessmanOnXYPlace(chessBoard, *currentSquare, *(currentSquare+1));
			if (target == 0 || (target >= 1 + sideDifference &&target <= CHESSMANS_SIZE + sideDifference)) {
				if (isChecked(chessBoard,currentSquare,whosChecking,true)) {
					executeChange(undoChangeArray, chessBoard);
					return true;
				}
			}


		}
	}
	return false;
}

unsigned char isCheckMate(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking) {
	unsigned char foundSquare[3];
	unsigned char foundBlockerSquare[3];
	unsigned char place0Knowledge = 0;
	unsigned char undoChangeArray[3];
	bool block = false;
	bool whosChecked;
	if (whosChecking) {
		whosChecked = false;
	}
	else {
		whosChecked = true;
	}
	if (findKnight(chessBoard, foundSquare, kingPos, &place0Knowledge, whosChecking) != 9) {
		if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
			createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
			*(foundBlockerSquare + 2) = 0;
			executeChange(foundBlockerSquare, chessBoard);
			block = true;
		}
		else {
			if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
				return 1;
			}
				return 2;
		}

	} 
	if (findOnDiagonal(chessBoard, foundSquare, kingPos, &place0Knowledge, whosChecking, false) != 9) {
		if (!block) {
			char changeX;
			char changeY;
			if(*foundSquare>*kingPos){
				changeX = -1;
			}
			else {
				changeX = 1;
			}
			if (*(foundSquare+1)>*(kingPos+1)) {
				changeY = -1;
			}
			else {
				changeY = 1;
			}
			while (*foundSquare != *kingPos) {
				if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
					createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
					*(foundBlockerSquare + 2) = 0;
					executeChange(foundBlockerSquare, chessBoard);
					block = true;
					break;
				}
				*foundSquare += changeX;
				*(foundSquare+1) += changeY;
			}
			if (!block) {
				if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
					return 1;
				}
				return 2;
			}
			
		}
		else {
			if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
				return 1;
			}
			return 2;
		}
	}
	if (findOnStraightLine(chessBoard, foundSquare, kingPos, &place0Knowledge, whosChecking, false) != 9) {
		if (!block) {
			char change;
			if (*(foundSquare + 1) == *(kingPos + 1)) {//zmiana x
				if (*foundSquare>*kingPos) {
					change = -1;
				}
				else {
					change = 1;
				}
				while (*foundSquare != *kingPos) {
					if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
						createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
						*(foundBlockerSquare + 2) = 0;
						executeChange(foundBlockerSquare, chessBoard);
						block = true;
						break;
					}
					*foundSquare += change;
				}
				if (!block) {
					if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
						return 1;
					}
					return 2;
				}

				}
			else {//zmiana y
				if (*(foundSquare+1)>*(kingPos+1)) {
					change = -1;
				}
				else {
					change = 1;
				}
				while (*(foundSquare+1) != *(kingPos+1)) {
					if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
						createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
						*(foundBlockerSquare + 2) = 0;
						executeChange(foundBlockerSquare, chessBoard);
						block = true;
						break;
					}
					*(foundSquare+1) += change;
				}
				if (!block) {
					if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
						return 1;
					}
					return 2;
				}
			
			}
		}
		else {
			if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
				return 1;
			}
			return 2;
		}
	}
	bool hit = true;
	*foundSquare = *kingPos - 1;
	if (*foundSquare < C_SIZE) {
		if (findPawn(chessBoard, foundSquare, kingPos, &hit, whosChecking) != 9) {
			if (!block) {
				if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
					createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
					*(foundBlockerSquare + 2) = 0;
					executeChange(foundBlockerSquare, chessBoard);
					block = true;
				}
				else {
					if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
						return 1;
					}
					return 2;
				}
			}
			else {
				if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
					return 1;
				}
				return 2;
			}

		}
	}
	*foundSquare = *kingPos + 1;
	if (*foundSquare < C_SIZE) {
		if (findPawn(chessBoard, foundSquare, kingPos, &hit, whosChecking) != 9) {
			if (!block) {
				if (isAttacked(chessBoard, foundBlockerSquare, foundSquare, whosChecked, false)) {
					createUndoChangeArray(undoChangeArray, chessBoard, foundBlockerSquare);
					*(foundBlockerSquare + 2) = 0;
					executeChange(foundBlockerSquare, chessBoard);
					block = true;
				}
				else {
					if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
						return 1;
					}
					return 2;
				}
			}
			else {
				if (canKingAvoidChess(chessBoard, kingPos, whosChecking)) {
					return 1;
				}
				return 2;
			}
		}
	}
	if (block) {
		executeChange(undoChangeArray, chessBoard);
		return 1;
	}
	return 0;
	
}

