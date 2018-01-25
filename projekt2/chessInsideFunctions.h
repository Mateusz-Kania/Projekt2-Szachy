#include "stackStruct.h"

unsigned char recognizeCastle(unsigned char** chessBoard, unsigned char* changesArray1, unsigned char* changesArray2, unsigned char* changesArray3, unsigned char* changesArray4, bool side, bool kingSide, bool* castlesChances);
void secureKingsMovementAndCastles(unsigned char* kingPos, bool* whiteCastlesChances, bool* blackCastlesChances, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* kingsPrevPosition, bool* whiteCastlesPrevChances, bool* blackCastlesPrevChances, bool side);
void createUndoCastlesChances(bool* prevCastlesChances, bool* castlesChances);
void createUndoKingsMovement(unsigned char* kingsPrevPosition, unsigned char* kingPos);
void createUndoChangeArray(unsigned char* undoChangeArray, unsigned char** chessBoard, unsigned char* changeArray);

bool undo(game &game);

void show(unsigned char** chessBoard);
void showChessman(unsigned char* chessman);
void finishGame(struct game &game);

bool canKingAvoidChess(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking);
unsigned char isCheckMate(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking);
bool isChecked(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking, bool kingChessCheck);
bool isAttacked(unsigned char** chessBoard, unsigned char* foundSquare, unsigned char* kingPos, bool whoAttack, bool kingChessCheck);
unsigned char isCheckMate(unsigned char** chessBoard, unsigned char* kingPos, bool whosChecking);//0-nic 1-szach 2-szachmat
unsigned char executeMove(struct game &game, std::string move);
unsigned char identifyPlace0Knowledge(unsigned char* changeArray, const char* start, unsigned char* size);//0-nic 1-tylko X 2-tylko Y 3-X+Y 4- b³¹d
unsigned char findPlace0(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool hit);
bool identifyCM(const char* chessman, unsigned char* target);
bool identifyX(const char* x, unsigned char* target);
bool identifyY(const char* y, unsigned char* target);
void executeChange(unsigned char* changeArray, unsigned char** chessBoard);
void sendCommunique(std::string communique);
std::string waitForAnswer();
unsigned char findPawn(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, bool hit, bool side);
unsigned char findKnight(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side);
unsigned char findBishop(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side);
unsigned char findRock(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side);
unsigned char findQueen(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side);
unsigned char findKing(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side);
unsigned char findChessmanOnXYPlace(unsigned char** chessBoard, unsigned char x, unsigned char y, unsigned char* chessman);
unsigned char findChessmanOnXYPlace(unsigned char** chessBoard, unsigned char x, unsigned char y);
unsigned char findOnDiagonal(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side, bool isChessmanSet);
unsigned char findOnStraightLine(unsigned char** chessBoard, unsigned char* changeArray1, unsigned char* changeArray2, unsigned char* place0Knowledge, bool side, bool isChessmanSet);

unsigned char checkHitCorrect(unsigned char** chessBoard, unsigned char* changeArray2, bool* passant, bool hit, bool side);

unsigned char checkPromote(const char* move, unsigned char* promote, char* mLength);

void checkPassant(const char* move, bool* passant, char* mLength);

unsigned char checkPassantCorrect(changesSet changesSet, unsigned char** chessBoard, unsigned char* changeArray2);
unsigned char operatePromote(unsigned char* changeArray2, unsigned char* promote, bool* side);
unsigned char recognizeStandardMove(unsigned char* changeArray1, unsigned char* changeArray2, const char* move, char* mLength, unsigned char* place0Knowledge, unsigned char* place0TextLength, bool* hit, bool* side);



bool undo(game &game);