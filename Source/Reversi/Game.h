#ifndef __GAME__H__
#define __GAME__H__

#include "Board.h"
#include "AI.h"
#include <string>

class Game{
public:

	Game();
	Game(bool firstPlayer);
	~Game();
	void init();
	void init(bool firstPlayer);
	void VSComputer();
	int InputPut();
	bool ReceivePut(int putCood);
	void DisplayResult();
	void DisplayBoard();
	void DisplayMessage(std::string message);
	void DisplayData();

private:
	Board* mBoard;
	AI* mAI;
	int mPlayerStoneColor;
	int mRivalStoneColor;
	std::string DisplayStone(int stoneColor);

};

#endif //__GAME__H__