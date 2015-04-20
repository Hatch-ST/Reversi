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

//�g�̕\��
void Board::displayBoard(){
	//�t�B�[���h�\���p�̘g�̃T�C�Y
	const int FrameSizeX = BoardSizeX*2+1;
	const int FrameSizeY = BoardSizeY*2+1;
	system("cls");
	//�g�̕\��
	for(int y = 0; y < FrameSizeY; y++){
		for(int x = 0; x < FrameSizeX; x++){
			if(y == 0){
				//0�~0
				if(x == 0)					cout << "��";
				//0�~�ŏI��
				else if(x == FrameSizeX-1)	cout << "��";
				//0�~������
				else if(x % 2 == 0)			cout << "��";
				//0�~���
				else						cout << "��";
			}else if(y == FrameSizeY-1){
				//�ŏI�s�~0
				if(x == 0)					cout << "��";
				//�ŏI�s�~�ŏI��
				else if(x == FrameSizeX-1)	cout << "��";
				//�ŏI�s�~������
				else if(x % 2 == 0)			cout << "��";
				//�ŏI�s�~���
				else						cout << "��";
			}else if(y % 2 == 0){
				//�����s�~0
				if(x == 0)					cout << "��";
				//�����s�~�ŏI��
				else if(x == FrameSizeX-1)	cout << "��";
				//�����s�~������
				else if(x % 2 == 0)			cout << "��";
				//�����s�~���
				else						cout << "��";
			}else{
				//��s�~0
				if(x == 0){
					//�s���̕\���@
					cout << "��";
				}
				//��s�~������
				else if(x % 2 == 0)			cout << "��";
				//��s�~���
				else{
					//�g�̒��̕\��
					cout << "�@";
				}
			}
		}
		//1�s�I���ŉ��s
		cout << "\n";
	}
	////�����z�u�̕\��
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

//�J�[�\���̈ʒu�����W����g�ɍ��킹�ĕϊ�����֐�
void Board::setCursorPosition( const int x, const int y ){
	COORD pos;
	pos.X = x * 4 + 2;
	pos.Y = y * 2 + 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
}

//�L�[�{�[�h�̓��͂��󂯕t����
int Board::waitKeyInput(BoardState playerColor){
	while(true){
		char input;
		//�L�[�{�[�h�̓��͂𒲂ׂ�
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

//�΂��u���邩���ׂ�
bool Board::checkCanPut(int x,int y,int playerColor){
	//���̃}�X���󂢂Ă��邩
	if(getStoneColor(x,y) != NOTHING){
		return false;
	}
	//���͂𒲂ׂĂ���
	return checkAroundStone(x,y,playerColor);
}

//���̃}�X�̐΂̐F���擾����
int Board::getStoneColor(int x,int y){
	return mBoardArray[x + y*BoardSizeX];
}

//���̍��W�̎���ɈႤ�F�����邩���ׂ�
bool Board::checkAroundStone(int x, int y, int playerColor){
	//�Ֆʂ̒[�̏ꍇ�A�I��
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	for(int dir=0;dir<DirectionSize;dir++){
		//���̒��ׂ���W
		int nextPosX = x;
		int nextPosY = y;
	
		//�����ɂ�镪��
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
		//�F�������łȂ������ׂ�
		if(getStoneColor(nextPosX,nextPosY) != playerColor && getStoneColor(nextPosX,nextPosY) != NOTHING){
			//�����ɂ�镪��
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
			//���̕����𒲂ׂ�
			if(checkLineStone(nextPosX,nextPosY,direction,playerColor)){
				return true;
			}
		}
	}
	return false;
}

//���̕����̐΂𒲂ׂ�
bool Board::checkLineStone(int x, int y,Direction dir,int playerColor){
	//�Ֆʂ̒[�̏ꍇ�A�ċN�Ăяo���I��
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	//���̍��W�̐΂𒲂ׂ�
	if(getStoneColor(x,y) == NOTHING){
		return false;
	}else if(getStoneColor(x,y) == playerColor){
		return true;
	}
	//����Ɠ����F�̏ꍇ�͂���ɒ��ׂĂ���
	else{
		//���̒��ׂ���W
		int nextPosX = x;
		int nextPosY = y;
	
		//�����ɂ�镪��
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
		//���\�b�h�̍ċN�Ăяo��
		return checkLineStone(nextPosX,nextPosY,dir,playerColor);
	}
}

//�΂�u��
bool Board::putStone(int stoneColor){
	if(checkCanPut(mCursolPosX,mCursolPosY,stoneColor)){
		drawStone(mCursolPosX,mCursolPosY,stoneColor);
		changeAroundStone(mCursolPosX,mCursolPosY,stoneColor);
		return true;
	}
	return false;
}

//�΂�u��
void Board::putAIStone(int x,int y,int stoneColor){
	drawStone(x,y,stoneColor);
	changeAroundStone(x,y,stoneColor);
}

void Board::changeAroundStone(int x, int y, int playerColor){
	//�Ֆʂ̒[�̏ꍇ�A�I��
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return;
	for(int dir=0;dir<DirectionSize;dir++){
		//���̒��ׂ���W
		int nextPosX = x;
		int nextPosY = y;
	
		//�����ɂ�镪��
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
		//�F�������łȂ������ׂ�
		if(getStoneColor(nextPosX,nextPosY) != playerColor && getStoneColor(nextPosX,nextPosY) != NOTHING){
			//����Ɏ��̍��W
			int linePosX = nextPosX;
			int linePosY = nextPosY;
			
			//�����ɂ�镪��
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
			//���̕����𒲂ׂ�
			if(changeLineStone(linePosX,linePosY,direction,playerColor)){
				drawStone(nextPosX,nextPosY,playerColor);
			}
		}
	}

}
bool Board::changeLineStone(int x, int y,Direction dir, int playerColor){
	//�Ֆʂ̒[�̏ꍇ�A�ċN�Ăяo���I��
	if (x < 0 || BoardSizeX - 1 < x || y < 0 || BoardSizeY - 1 < y)
		return false;
	//���̍��W�̐΂𒲂ׂ�
	if(getStoneColor(x,y) == NOTHING){
		return false;
	}else if(getStoneColor(x,y) == playerColor){
		return true;
	}
	//����Ɠ����F�̏ꍇ�͂���ɒ��ׂĂ���
	else{
		//���̒��ׂ���W
		int nextPosX = x;
		int nextPosY = y;
	
		//�����ɂ�镪��
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
		//���\�b�h�̍ċN�Ăяo��
		bool flag = changeLineStone(nextPosX,nextPosY,dir,playerColor);
		if(flag){
			drawStone(x,y,playerColor);
		}
		return flag;
	}
}

//���̍��W�ɐ΂�`�悷��
void Board::drawStone(int x,int y,int stoneColor){
	setCursorPosition(x,y);
	if(stoneColor == WHITE){
		cout << "��";
	}else if(stoneColor == BLACK){
		cout << "��";
	}else{
		cout << "�@";
	}
	Sleep(100);
	//�{�[�h�ɓo�^
	mBoardArray[x+y*BoardSizeX] = stoneColor;
	setCursorPosition(mCursolPosX,mCursolPosY);
}

//�΂̐����擾����
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

//�G���^�[�̓��͂�҂�
void Board::waitEnter(){
	cout << "Enter�ŏI�����܂�\n";
	while(true){
		char input;
		//�L�[�{�[�h�̓��͂𒲂ׂ�
		if(kbhit()){
			input = getch();
			if( input == EnterKeyID ){
				break;
			}
		}
	}
}