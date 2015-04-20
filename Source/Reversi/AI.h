#ifndef __AI__H__
#define __AI__H__

#include "Board.h"

class AI{
public:
	AI(Board* board);
	~AI();
	void init();
	int setCanPutPos(int stoneColor);
	void getRandomPos(int* outX,int* outY);
	int getStoneColor(){ return mStoneColor; };
	void getPriorityPos(int* outX,int* outY);
private:
	Board* pBoard;
	int** canPutPos;	//�u����ꏊ�̏ꏊ
	int mCanPutNum;		//�u����ꏊ�̌�
	int mStoneColor;
	int* mPutScore;		//�u����ꏊ�̗D��x
	void addPriority();
	void addPriorityFromCorner();
	void addPriorityFromSide();

};

#endif //__AI__H__