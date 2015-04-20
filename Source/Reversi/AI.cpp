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

//�u������W����������
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
	//���̑���-1�����Ă���
	int size = pBoard->BoardSizeX * pBoard->BoardSizeY - 4;
	for(int i = mCanPutNum; i < size; i++){
		canPutPos[i][0] = -1;
		canPutPos[i][1] = -1;
	}
	return mCanPutNum;
}

//�u���ꏊ�������_���Ɍ��肷��
void AI::getRandomPos(int* outX,int* outY){
	int random = rand() % mCanPutNum;
	*outX = canPutPos[random][0];
	*outY = canPutPos[random][1];
}

//�D��x����u���ꏊ�����肷��
void AI::getPriorityPos(int* outX,int* outY){
	//�D��x�𑫂�
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

//�u����ꏊ�̗D��x�𑫂�
void AI::addPriority(){
	addPriorityFromCorner();
	addPriorityFromSide();
}

//�u����ꏊ���p�̏ꍇ�D��x���グ��
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

//�u����ꏊ���[�̏ꍇ�D��x���グ��
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