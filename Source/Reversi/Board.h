#ifndef __BOARD__H__
#define __BOARD__H__

class Board{
public:
	enum BoardState{
		WHITE = -1,
		NOTHING = 0,
		BLACK = 1,
	};
	enum Direction{
		Up,
		Down,
		Left,
		Right,
		UpperLeft,
		UpperRight,
		LowerLeft,
		LowerRight,
		DirectionSize
	};
	Board();
	~Board();
	void init();
	void initBoard();
	void displayBoard();
	static const int BoardSizeX = 8;
	static const int BoardSizeY = 8;
	int waitKeyInput(BoardState playerColor);
	bool putStone(int stoneColor);
	void putAIStone(int x,int y,int stoneColor);
	bool checkCanPut(int x,int y,int playerColor);
	void changeAroundStone(int x, int y, int playerColor);
	int getStoneNum(int stoneColor);
	void setCursorPosition( const int x, const int y );
	void ResetCursorPosition();
	void waitEnter();
private:
	int* mBoardArray;
	
	int mCursolPosX;
	int mCursolPosY;
	static const int UpKeyID = 0x48;			//↑キー
	static const int DownKeyID = 0x50;			//↓キー
	static const int LeftKeyID = 0x4B;			//←キー
	static const int RightKeyID = 0x4D;			//→キー
	static const int EnterKeyID = 0x0d;			//エンターキー

	int getStoneColor(int x,int y);
	bool checkAroundStone(int x, int y, int playerColor);
	bool checkLineStone(int x, int y,Direction dir,int playerColor);
	bool changeLineStone(int x, int y,Direction dir, int playerColor);
	void drawStone(int x,int y,int stoneColor);
};

#endif //__BOARD__H__