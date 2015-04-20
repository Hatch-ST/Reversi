#include "Board.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

Board::Board(){
	init();
}

Board::~Board(){
	delete[] mBoardArray;
}

void Board::init(){
	mCursolPosX = 0;
	mCursolPosY = 0;
	initBoard();
}

void Board::initBoard(){
	mBoardArray = new int[BoardSizeX * BoardSizeY];
	for(int i=0;i<BoardSizeX*BoardSizeY;i++){
		if(i==27 || i == 36 ){
			mBoardArray[i] = WHITE;
		}else if(i==28 || i == 35 ){
			mBoardArray[i] = BLACK;
		}else{
			mBoardArray[i] = NOTHING;
		}
	}
}

//枠の表示
void Board::displayBoard(){
	//フィールド表示用の枠のサイズ
	const int FrameSizeX = BoardSizeX*2+1;
	const int FrameSizeY = BoardSizeY*2+1;
	system("cls");
	//枠の表示
	for(int y = 0; y < FrameSizeY; y++){
		for(int x = 0; x < FrameSizeX; x++){
			if(y == 0){
				//0×0
				if(x == 0)					cout << "┏";
				//0×最終列
				else if(x == FrameSizeX-1)	cout << "┓";
				//0×偶数列
				else if(x % 2 == 0)			cout << "┳";
				//0×奇数列
				else						cout << "━";
			}else if(y == FrameSizeY-1){
				//最終行×0
				if(x == 0)					cout << "┗";
				//最終行×最終列
				else if(x == FrameSizeX-1)	cout << "┛";
				//最終行×偶数列
				else if(x % 2 == 0)			cout << "┻";
				//最終行×奇数列
				else						cout << "━";
			}else if(y % 2 == 0){
				//偶数行×0
				if(x == 0)					cout << "┣";
				//偶数行×最終列
				else if(x == FrameSizeX-1)	cout << "┫";
				//偶数行×偶数列
				else if(x % 2 == 0)			cout << "╋";
				//偶数行×奇数列
				else						cout << "━";
			}else{
				//奇数行×0
				if(x == 0){
					//行数の表示①
					cout << "┃";
				}
				//奇数行×偶数列
				else if(x % 2 == 0)			cout << "┃";
				//奇数行×奇数列
				else{
					//枠の中の表示
					cout << "　";
				}
			}
		}
		//1行終了で改行
		cout << "\n";
	}
	////初期配置の表示
	for(int i=0;i<4;i++){
		int x,y;
		if(i==0){
			x=3,y=3;
		}else if(i==1){
			x=3,y=4;
		}else if(i==2){
			x=4,y=3;
		}else if(i==3){
			x=4,y=4;
		}
		drawStone(x,y,mBoardArray[x + y * BoardSizeX]);
	}
}

//カーソルの位置を座標から枠に合わせて変換する関数
void Board::setCursorPosition( const int x, const int y ){
	COORD pos;
	pos.X = x * 4 + 2;
	pos.Y = y * 2 + 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
}

//キーボードの入力を受け付ける
int Board::waitKeyInput(BoardState playerColor){
	while(true){
		char input;
		//キーボードの入力を調べる
		if(kbhit()){
			input = getch();
			switch( input ){
				case '8' :
				case UpKeyID:
					if( 0 < mCursolPosY){
						mCursolPosY--;
						setCursorPosition(mCursolPosX, mCursolPosY);
					}
					break;
				case '2' :
				case DownKeyID:
					if( mCursolPosY < BoardSizeY-1){
						mCursolPosY++;
						setCursorPosition(mCursolPosX, mCursolPosY);
					}
					break;
				case '4' :
				case LeftKeyID:
					if( 0 < mCursolPosX){
						mCursolPosX--;
						setCursorPosition(mCursolPosX, mCursolPosY);
					}
					break;
				case '6' :
				case RightKeyID:
					if( mCursolPosX < BoardSizeX-1){
						mCursolPosX++;
						setCursorPosition(mCursolPosX, mCursolPosY);
					}
					break;
				case EnterKeyID:
					if(putStone(playerColor)){
						int putCood = mCursolPosX * 10 + mCursolPosY;
						return putCood;
					}
			}
		}
	}
	return -1;
}

//石が置けるか調べる
bool Board::checkCanPut(int x,int y,int playerColor){
	//そのマスが空いているか
	if(getStoneColor(x,y) != NOTHING){
		return false;
	}
	//周囲を調べていく
	return checkAroundStone(x,y,playerColor);
}

//そのマスの石の色を取得する
int Board::getStoneColor(int x,int y){
	return mBoardArray[x + y*BoardSizeX];
}

//その座標の周りに違う色があるか調べる
bool Board::checkAroundStone(int x, int y, int playerColor){
	//盤面の端の場合、終了
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	for(int dir=0;dir<DirectionSize;dir++){
		//次の調べる座標
		int nextPosX = x;
		int nextPosY = y;
	
		//向きによる分岐
		switch (dir) {
			case Up:
				nextPosY--;
				break;
			case Down:
				nextPosY++;
				break;
			case Left:
				nextPosX--;
				break;
			case Right:
				nextPosX++;
				break;
			case UpperLeft:
				nextPosY--;
				nextPosX--;
				break;
			case UpperRight:
				nextPosY--;
				nextPosX++;
				break;
			case LowerLeft:
				nextPosY++;
				nextPosX--;
				break;
			case LowerRight:
				nextPosY++;
				nextPosX++;
				break;
		}
		//色が同じでないか調べる
		if(getStoneColor(nextPosX,nextPosY) != playerColor && getStoneColor(nextPosX,nextPosY) != NOTHING){
			//向きによる分岐
			switch (dir) {
				case Up:
					nextPosY--;
					break;
				case Down:
					nextPosY++;
					break;
				case Left:
					nextPosX--;
					break;
				case Right:
					nextPosX++;
					break;
				case UpperLeft:
					nextPosY--;
					nextPosX--;
					break;
				case UpperRight:
					nextPosY--;
					nextPosX++;
					break;
				case LowerLeft:
					nextPosY++;
					nextPosX--;
					break;
				case LowerRight:
					nextPosY++;
					nextPosX++;
					break;
			}
			Direction direction = (Direction)dir; 
			//その方向を調べる
			if(checkLineStone(nextPosX,nextPosY,direction,playerColor)){
				return true;
			}
		}
	}
	return false;
}

//その方向の石を調べる
bool Board::checkLineStone(int x, int y,Direction dir,int playerColor){
	//盤面の端の場合、再起呼び出し終了
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	//その座標の石を調べる
	if(getStoneColor(x,y) == NOTHING){
		return false;
	}else if(getStoneColor(x,y) == playerColor){
		return true;
	}
	//相手と同じ色の場合はさらに調べていく
	else{
		//次の調べる座標
		int nextPosX = x;
		int nextPosY = y;
	
		//向きによる分岐
		switch (dir) {
			case Up:
				nextPosY--;
				break;
			case Down:
				nextPosY++;
				break;
			case Left:
				nextPosX--;
				break;
			case Right:
				nextPosX++;
				break;
			case UpperLeft:
				nextPosY--;
				nextPosX--;
				break;
			case UpperRight:
				nextPosY--;
				nextPosX++;
				break;
			case LowerLeft:
				nextPosY++;
				nextPosX--;
				break;
			case LowerRight:
				nextPosY++;
				nextPosX++;
				break;
		}
		//メソッドの再起呼び出し
		return checkLineStone(nextPosX,nextPosY,dir,playerColor);
	}
}

//石を置く
bool Board::putStone(int stoneColor){
	if(checkCanPut(mCursolPosX,mCursolPosY,stoneColor)){
		drawStone(mCursolPosX,mCursolPosY,stoneColor);
		changeAroundStone(mCursolPosX,mCursolPosY,stoneColor);
		return true;
	}
	return false;
}

//石を置く
void Board::putAIStone(int x,int y,int stoneColor){
	drawStone(x,y,stoneColor);
	changeAroundStone(x,y,stoneColor);
}

void Board::changeAroundStone(int x, int y, int playerColor){
	//盤面の端の場合、終了
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return;
	for(int dir=0;dir<DirectionSize;dir++){
		//次の調べる座標
		int nextPosX = x;
		int nextPosY = y;
	
		//向きによる分岐
		switch (dir) {
			case Up:
				nextPosY--;
				break;
			case Down:
				nextPosY++;
				break;
			case Left:
				nextPosX--;
				break;
			case Right:
				nextPosX++;
				break;
			case UpperLeft:
				nextPosY--;
				nextPosX--;
				break;
			case UpperRight:
				nextPosY--;
				nextPosX++;
				break;
			case LowerLeft:
				nextPosY++;
				nextPosX--;
				break;
			case LowerRight:
				nextPosY++;
				nextPosX++;
				break;
		}
		//色が同じでないか調べる
		if(getStoneColor(nextPosX,nextPosY) != playerColor && getStoneColor(nextPosX,nextPosY) != NOTHING){
			//さらに次の座標
			int linePosX = nextPosX;
			int linePosY = nextPosY;
			
			//向きによる分岐
			switch (dir) {
				case Up:
					linePosY--;
					break;
				case Down:
					linePosY++;
					break;
				case Left:
					linePosX--;
					break;
				case Right:
					linePosX++;
					break;
				case UpperLeft:
					linePosY--;
					linePosX--;
					break;
				case UpperRight:
					linePosY--;
					linePosX++;
					break;
				case LowerLeft:
					linePosY++;
					linePosX--;
					break;
				case LowerRight:
					linePosY++;
					linePosX++;
					break;
			}
			Direction direction = (Direction)dir; 
			//その方向を調べる
			if(changeLineStone(linePosX,linePosY,direction,playerColor)){
				drawStone(nextPosX,nextPosY,playerColor);
			}
		}
	}

}
bool Board::changeLineStone(int x, int y,Direction dir, int playerColor){
	//盤面の端の場合、再起呼び出し終了
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	//その座標の石を調べる
	if(getStoneColor(x,y) == NOTHING){
		return false;
	}else if(getStoneColor(x,y) == playerColor){
		return true;
	}
	//相手と同じ色の場合はさらに調べていく
	else{
		//次の調べる座標
		int nextPosX = x;
		int nextPosY = y;
	
		//向きによる分岐
		switch (dir) {
			case Up:
				nextPosY--;
				break;
			case Down:
				nextPosY++;
				break;
			case Left:
				nextPosX--;
				break;
			case Right:
				nextPosX++;
				break;
			case UpperLeft:
				nextPosY--;
				nextPosX--;
				break;
			case UpperRight:
				nextPosY--;
				nextPosX++;
				break;
			case LowerLeft:
				nextPosY++;
				nextPosX--;
				break;
			case LowerRight:
				nextPosY++;
				nextPosX++;
				break;
		}
		//メソッドの再起呼び出し
		bool flag = changeLineStone(nextPosX,nextPosY,dir,playerColor);
		if(flag){
			drawStone(x,y,playerColor);
		}
		return flag;
	}
}

//その座標に石を描画する
void Board::drawStone(int x,int y,int stoneColor){
	setCursorPosition(x,y);
	if(stoneColor == WHITE){
		cout << "○";
	}else if(stoneColor == BLACK){
		cout << "●";
	}else{
		cout << "　";
	}
	Sleep(100);
	//ボードに登録
	mBoardArray[x+y*BoardSizeX] = stoneColor;
	setCursorPosition(mCursolPosX,mCursolPosY);
}

//石の数を取得する
int Board::getStoneNum(int stoneColor){
	int stoneCount=0;
	for(int i=0;i<BoardSizeX*BoardSizeY;i++){
		if(mBoardArray[i] == stoneColor){
			stoneCount++;
		}
	}
	return stoneCount;

}

void Board::ResetCursorPosition(){
	setCursorPosition(mCursolPosX,mCursolPosY);
}

//エンターの入力を待つ
void Board::waitEnter(){
	cout << "Enterで終了します\n";
	while(true){
		char input;
		//キーボードの入力を調べる
		if(kbhit()){
			input = getch();
			if( input == EnterKeyID ){
				break;
			}
		}
	}
}