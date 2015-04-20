#include "Game.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

Game::Game(){
	init();
}

Game::Game(bool firstPlayer){
	init(firstPlayer);
}

Game::~Game(){
	delete mBoard;
	delete mAI;
}

void Game::init(){
	mBoard = new Board();
	mAI = new AI(mBoard);
	mPlayerStoneColor = mBoard->BLACK;
	mRivalStoneColor = mBoard->WHITE;
}

void Game::init(bool firstPlayer){
	mBoard = new Board();
	mAI = new AI(mBoard);
	if(firstPlayer){
		mPlayerStoneColor = mBoard->BLACK;
		mRivalStoneColor = mBoard->WHITE;
	}else{
		mPlayerStoneColor = mBoard->WHITE;
		mRivalStoneColor = mBoard->BLACK;
	}
}

//�Q�[�����C������ (CPU�Ƒΐ�)
void Game::VSComputer(){
	mBoard->displayBoard();
	DisplayData();
	bool isGameLooping = true;
	while(isGameLooping){
		//���̓��[�v
		//�u���ꏊ���Ȃ���΃p�X
		if(mAI->setCanPutPos(mPlayerStoneColor) != 0){
			mBoard->waitKeyInput((Board::BoardState)mPlayerStoneColor);
		}
		Sleep(300);
		//AI�̑���
		if(mAI->setCanPutPos(mRivalStoneColor) != 0){
			int x,y;
			//�����_���ɒu���ꏊ������
			//mAI->getRandomPos(&x,&y);
			//�D��x����u���ꏊ������
			mAI->getPriorityPos(&x,&y);

			mBoard->putAIStone(x,y,mRivalStoneColor);
			mBoard->setCursorPosition(0,8);
			cout << "�����(" << x << "," << y << ")�ɒu���܂���\n";
			mBoard->ResetCursorPosition();
		}
		

		//���݂��u���ꏊ���Ȃ��Ƃ�
		if(mAI->setCanPutPos(mPlayerStoneColor) == 0 && mAI->setCanPutPos(mRivalStoneColor) == 0 ){
			isGameLooping = false;
		}
		//�΂̐���0�̂Ƃ�
		else if( mBoard->getStoneNum(mPlayerStoneColor) == 0 &&  mBoard->getStoneNum(mRivalStoneColor) == 0){
			isGameLooping = false;
		}
	}
	//���ʕ\��
	DisplayResult();
}

//�Ֆʂ̕\��
void Game::DisplayBoard(){
	mBoard->displayBoard();
}

//�΂�u������
int Game::InputPut(){
	mBoard->setCursorPosition(0,8);
	cout << "�@�@�@�@�@�@�@�@�@�@�@";
	cout << "���Ȃ��̎�Ԃł�\n";
	mBoard->ResetCursorPosition();
	int putCood = -1;
	//���̓��[�v
	//�u���ꏊ���Ȃ���΃p�X
	if(mAI->setCanPutPos(mPlayerStoneColor) != 0){
		putCood = mBoard->waitKeyInput((Board::BoardState)mPlayerStoneColor);
	}else{
		mBoard->setCursorPosition(0,8);
		cout << "�u����ꏊ������܂���\n";
		mBoard->ResetCursorPosition();
	}
	return putCood;
}

//����̐΂�u���i�Q�[���I������true�j
bool Game::ReceivePut(int putCood){
	//����̐΂�u��
	if(putCood != -1){
		int putX = putCood / 10;
		int putY = putCood % 10;
		mBoard->putAIStone( putX, putY ,mRivalStoneColor);
		mBoard->setCursorPosition(0,8);
		cout << "�����(" << putX << "," << putY << ")�ɒu���܂���\n";
		mBoard->ResetCursorPosition();
	}else{
		mBoard->setCursorPosition(0,8);
		cout << "����̓p�X���܂����@�@\n";
		mBoard->ResetCursorPosition();
		//���݂��u���ꏊ���Ȃ��Ƃ�
		if(mAI->setCanPutPos(mPlayerStoneColor) == 0 && mAI->setCanPutPos(mRivalStoneColor) == 0 ){
			return true;
		}
		//�΂̐���0�̂Ƃ�
		else if( mBoard->getStoneNum(mPlayerStoneColor) == 0 &&  mBoard->getStoneNum(mRivalStoneColor) == 0){
			return true;
		}
	}

	return false;
}

void Game::DisplayResult(){
	//���ʕ\��
	mBoard->setCursorPosition(0,8);
	cout << "�@�@�@�@�@�@�@�@�@�@�@�@�@�@\n";
	cout << "���Ȃ��F" << mBoard->getStoneNum(mPlayerStoneColor) << "\n";
	cout << " ���� �F" << mBoard->getStoneNum(mRivalStoneColor) << "\n";
	if(mBoard->getStoneNum(mPlayerStoneColor) > mBoard->getStoneNum(mRivalStoneColor)){
		cout << "���Ȃ��̏����ł�!�@\n";
	}else if(mBoard->getStoneNum(mPlayerStoneColor) < mBoard->getStoneNum(mRivalStoneColor)){
		cout << "���Ȃ��̕����ł�!�@\n";
	}else{
		cout << "���������ł�!\n";
	}
	mBoard->waitEnter();
}

void Game::DisplayMessage(string message){
	mBoard->setCursorPosition(0,9);
	cout << message << "\n";
}

void Game::DisplayData(){
	mBoard->setCursorPosition(9,0);
	cout << "�J�[�\���L�[�F�ړ�\n";
	mBoard->setCursorPosition(9,1);
	cout << "Enter:�΂�u��\n";
	mBoard->setCursorPosition(9,2);
	cout << "���Ȃ�:" << DisplayStone(mPlayerStoneColor) << "\n";
	mBoard->setCursorPosition(9,3);
	cout << " ���� :" << DisplayStone(mRivalStoneColor) << "\n";
	mBoard->ResetCursorPosition();
}

string Game::DisplayStone(int stoneColor){
	if(stoneColor == mBoard->WHITE){
		return "��";
	}else if(stoneColor == mBoard->BLACK){
		return "��";
	}else{
		return "�@";
	}

}