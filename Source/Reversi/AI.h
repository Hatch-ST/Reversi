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
	int** canPutPos;	//置ける場所の場所
	int mCanPutNum;		//置ける場所の個数
	int mStoneColor;
	int* mPutScore;		//置ける場所の優先度
	void addPriority();
	void addPriorityFromCorner();
	void addPriorityFromSide();

};

#endif //__AI__H__