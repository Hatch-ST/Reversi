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

//ゲームメイン処理 (CPUと対戦)
void Game::VSComputer(){
	mBoard->displayBoard();
	DisplayData();
	bool isGameLooping = true;
	while(isGameLooping){
		//入力ループ
		//置く場所がなければパス
		if(mAI->setCanPutPos(mPlayerStoneColor) != 0){
			mBoard->waitKeyInput((Board::BoardState)mPlayerStoneColor);
		}
		Sleep(300);
		//AIの操作
		if(mAI->setCanPutPos(mRivalStoneColor) != 0){
			int x,y;
			//ランダムに置く場所を決定
			//mAI->getRandomPos(&x,&y);
			//優先度から置く場所を決定
			mAI->getPriorityPos(&x,&y);

			mBoard->putAIStone(x,y,mRivalStoneColor);
			mBoard->setCursorPosition(0,8);
			cout << "相手は(" << x << "," << y << ")に置きました\n";
			mBoard->ResetCursorPosition();
		}
		

		//お互い置く場所がないとき
		if(mAI->setCanPutPos(mPlayerStoneColor) == 0 && mAI->setCanPutPos(mRivalStoneColor) == 0 ){
			isGameLooping = false;
		}
		//石の数が0のとき
		else if( mBoard->getStoneNum(mPlayerStoneColor) == 0 &&  mBoard->getStoneNum(mRivalStoneColor) == 0){
			isGameLooping = false;
		}
	}
	//結果表示
	DisplayResult();
}

//盤面の表示
void Game::DisplayBoard(){
	mBoard->displayBoard();
}

//石を置く操作
int Game::InputPut(){
	mBoard->setCursorPosition(0,8);
	cout << "　　　　　　　　　　　";
	cout << "あなたの手番です\n";
	mBoard->ResetCursorPosition();
	int putCood = -1;
	//入力ループ
	//置く場所がなければパス
	if(mAI->setCanPutPos(mPlayerStoneColor) != 0){
		putCood = mBoard->waitKeyInput((Board::BoardState)mPlayerStoneColor);
	}else{
		mBoard->setCursorPosition(0,8);
		cout << "置ける場所がありません\n";
		mBoard->ResetCursorPosition();
	}
	return putCood;
}

//相手の石を置く（ゲーム終了時にtrue）
bool Game::ReceivePut(int putCood){
	//相手の石を置く
	if(putCood != -1){
		int putX = putCood / 10;
		int putY = putCood % 10;
		mBoard->putAIStone( putX, putY ,mRivalStoneColor);
		mBoard->setCursorPosition(0,8);
		cout << "相手は(" << putX << "," << putY << ")に置きました\n";
		mBoard->ResetCursorPosition();
	}else{
		mBoard->setCursorPosition(0,8);
		cout << "相手はパスしました　　\n";
		mBoard->ResetCursorPosition();
		//お互い置く場所がないとき
		if(mAI->setCanPutPos(mPlayerStoneColor) == 0 && mAI->setCanPutPos(mRivalStoneColor) == 0 ){
			return true;
		}
		//石の数が0のとき
		else if( mBoard->getStoneNum(mPlayerStoneColor) == 0 &&  mBoard->getStoneNum(mRivalStoneColor) == 0){
			return true;
		}
	}

	return false;
}

void Game::DisplayResult(){
	//結果表示
	mBoard->setCursorPosition(0,8);
	cout << "　　　　　　　　　　　　　　\n";
	cout << "あなた：" << mBoard->getStoneNum(mPlayerStoneColor) << "\n";
	cout << " 相手 ：" << mBoard->getStoneNum(mRivalStoneColor) << "\n";
	if(mBoard->getStoneNum(mPlayerStoneColor) > mBoard->getStoneNum(mRivalStoneColor)){
		cout << "あなたの勝ちです!　\n";
	}else if(mBoard->getStoneNum(mPlayerStoneColor) < mBoard->getStoneNum(mRivalStoneColor)){
		cout << "あなたの負けです!　\n";
	}else{
		cout << "引き分けです!\n";
	}
	mBoard->waitEnter();
}

void Game::DisplayMessage(string message){
	mBoard->setCursorPosition(0,9);
	cout << message << "\n";
}

void Game::DisplayData(){
	mBoard->setCursorPosition(9,0);
	cout << "カーソルキー：移動\n";
	mBoard->setCursorPosition(9,1);
	cout << "Enter:石を置く\n";
	mBoard->setCursorPosition(9,2);
	cout << "あなた:" << DisplayStone(mPlayerStoneColor) << "\n";
	mBoard->setCursorPosition(9,3);
	cout << " 相手 :" << DisplayStone(mRivalStoneColor) << "\n";
	mBoard->ResetCursorPosition();
}

string Game::DisplayStone(int stoneColor){
	if(stoneColor == mBoard->WHITE){
		return "○";
	}else if(stoneColor == mBoard->BLACK){
		return "●";
	}else{
		return "　";
	}

}