#include "AI.h"
#include<stdlib.h>

AI::AI(Board* board){
	pBoard = board;
	init();
}
AI::~AI(){
	int size = pBoard->BoardSizeX * pBoard->BoardSizeY - 4;
	for(int i=0;i<size;i++){
		delete canPutPos[i];
	}
	delete[] canPutPos;
	delete mPutScore;
}
void AI::init(){
	int size = pBoard->BoardSizeX * pBoard->BoardSizeY - 4;
	canPutPos = new int*[size];
	for(int i=0;i<size;i++){
		canPutPos[i] = new int[2];
	}
	mStoneColor = pBoard->WHITE;
	mPutScore = new int[size];
}

//置ける座標を検索する
int AI::setCanPutPos(int stoneColor){
	mCanPutNum = 0;
	for(int y = 0; y < pBoard->BoardSizeY; y++){
		for(int x = 0; x < pBoard->BoardSizeX; x++){
			if(pBoard->checkCanPut(x,y,stoneColor) ){
				canPutPos[mCanPutNum][0] = x;
				canPutPos[mCanPutNum][1] = y;
				mCanPutNum++;
				mPutScore[mCanPutNum] = 0;
			}
		}
	}
	//その他は-1を入れておく
	int size = pBoard->BoardSizeX * pBoard->BoardSizeY - 4;
	for(int i = mCanPutNum; i < size; i++){
		canPutPos[i][0] = -1;
		canPutPos[i][1] = -1;
	}
	return mCanPutNum;
}

//置く場所をランダムに決定する
void AI::getRandomPos(int* outX,int* outY){
	int random = rand() % mCanPutNum;
	*outX = canPutPos[random][0];
	*outY = canPutPos[random][1];
}

//優先度から置く場所を決定する
void AI::getPriorityPos(int* outX,int* outY){
	//優先度を足す
	addPriority();

	int posPriority = 0;
	int posNum = 0;
	for(int i=0; i < mCanPutNum; i++){
		if(posPriority > mPutScore[i] ){
			posPriority = mPutScore[i];
			posNum = i;
		}else if(posPriority == mPutScore[i]){
			int random = rand() % 2;
			if( random == 0){
				posPriority = mPutScore[i];
				posNum = i;
			}
		}
	}

	*outX = canPutPos[posNum][0];
	*outY = canPutPos[posNum][1];
}

//置ける場所の優先度を足す
void AI::addPriority(){
	addPriorityFromCorner();
	addPriorityFromSide();
}

//置ける場所が角の場合優先度を上げる
void AI::addPriorityFromCorner(){
	for(int i = 0; i < mCanPutNum; i++){
		if(canPutPos[i][0] == 0 && canPutPos[i][1] == 0){
			mPutScore[i] += 10;
		}else if(canPutPos[i][0] == pBoard->BoardSizeX-1 && canPutPos[i][1] == 0){
			mPutScore[i] += 10;
		}else if(canPutPos[i][0] == 0 && canPutPos[i][1] == pBoard->BoardSizeY-1){
			mPutScore[i] += 10;
		}else if(canPutPos[i][0] == pBoard->BoardSizeX-1 && canPutPos[i][1] == pBoard->BoardSizeY-1){
			mPutScore[i] += 10;
		}
	}
}

//置ける場所が端の場合優先度を上げる
void AI::addPriorityFromSide(){
	for(int i = 0; i < mCanPutNum; i++){
		if(canPutPos[i][0] == 0){
			mPutScore[i] += 2;
		}else if(canPutPos[i][0] == pBoard->BoardSizeX-1){
			mPutScore[i] += 2;
		}
		if(canPutPos[i][1] == 0){
			mPutScore[i] += 2;
		}else if(canPutPos[i][0] == pBoard->BoardSizeY-1){
			mPutScore[i] += 2;
		}

	}
}