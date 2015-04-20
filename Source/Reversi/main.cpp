#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <winsock2.h>
#include "Game.h"

using namespace std;

enum CPUType{
	Server, Client, CPUOther	
};

enum GameMode{
	VSComputer, VSHuman, ModeOther
};

int ServerStart(unsigned short portNum, SOCKET* soc);
int ClientStart(string serverName, unsigned short portNum, SOCKET* soc);
int SocketEnd(SOCKET* soc);


void InputHost(string* host);
void InputPort(unsigned short* port);

string Encode(int sendData, int sendDataSize);
int Decode(char* receiveData);

int main()
{
	int sendDataSize=2;
	string inputBuffer;

	string message;
	char receiveBuffer[256];

	string host;
	unsigned short port=0;
	SOCKET soc;
	int rcv;
	cout << "���o�[�X���n�߂܂�\n";
	GameMode mode = ModeOther;
	while(mode==ModeOther){
		cout << "CPU�Ƒΐ킷��ꍇ��'1' ���̃v���C���[�Ƒΐ킷��̏ꍇ��'2'����͂��Ă�������\n";
		getline(cin, inputBuffer);
		switch(atoi(inputBuffer.c_str())){
			case 1:
				mode=VSComputer;
				break;
			case 2:
				mode=VSHuman;
				break;
			default:
				cout << "1�܂���2����͂��Ă��������B\n";
				break;
		}
	}

	if(mode==VSComputer){
		Game game = Game();
		game.VSComputer();
	}else if(mode==VSHuman){
		CPUType cpu=CPUOther;
		while(cpu==CPUOther){
			cout << "�T�[�o�[�̏ꍇ��'1' �N���C�A���g�̏ꍇ��'2'����͂��Ă�������\n";
			getline(cin, inputBuffer);
			switch(atoi(inputBuffer.c_str())){
				case 1:
					cpu=Server;
					break;
				case 2:
					cpu=Client;
					break;
				default:
					cout << "1�܂���2����͂��Ă��������B\n";
					break;
			}
		}
		bool isGameLooping = true;
		Game* game =  new Game();
		int putCood;
		switch(cpu){
			case Server:
				
				InputPort(&port);

				if(ServerStart(port, &soc)){
					cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���B\n";
				}else{
					game->init(true);
					game->DisplayBoard();
					game->DisplayData();
					while(isGameLooping){
						//���͂Ƒ��M
						putCood = game->InputPut();
						message = Encode(putCood,sendDataSize);
						send(soc, message.c_str(), message.length(), 0);

						//��M
						game->DisplayMessage("�ʐM�҂��@�@�@�@�@�@�@");
						rcv=recv(soc, receiveBuffer, sizeof(receiveBuffer)-1, 0);
						if(rcv==SOCKET_ERROR){
							cout << "��M�ł��܂���ł����B\n";
							break;
						}else{
							receiveBuffer[rcv]='\0';
							putCood=Decode(receiveBuffer);
						}
						if(game->ReceivePut(putCood)){
							//�Q�[���I��
							message = 'P';
							send(soc, message.c_str(), message.length(), 0);
							break;
						}
					}
				}
				break;
			case Client:
				InputHost(&host);
				InputPort(&port);

				if(ClientStart(host, port, &soc)){
					cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���B\n";
				}else{
					game->init(false);
					game->DisplayBoard();
					game->DisplayData();
					while(isGameLooping){
						//��M
						game->DisplayMessage("�ʐM�҂��@�@�@�@�@�@�@");
						rcv=recv(soc, receiveBuffer, sizeof(receiveBuffer)-1, 0);
						if(rcv==SOCKET_ERROR){
							cout << "��M�ł��܂���ł����B\n";
							break;
						}else{
							receiveBuffer[rcv]='\0';
							putCood=Decode(receiveBuffer);
						}
						if(game->ReceivePut(putCood)){
							//�Q�[���I��
							message = 'P';
							send(soc, message.c_str(), message.length(), 0);
							break;
						}

						//���͂Ƒ��M
						putCood = game->InputPut();
						message = Encode(putCood,sendDataSize);
						send(soc, message.c_str(), message.length(), 0);			
					}
				}
				break;
			default:
				break;
		}
		game->DisplayResult();

		SocketEnd(&soc);
		delete game;
	}
	
	return 0;
}

int ServerStart(unsigned short portNum, SOCKET* soc)
{
	WSADATA wsaData;
	SOCKET listenSoc;
	SOCKADDR_IN saddr, from;
	int fromlen, ret;

	ret=WSAStartup(MAKEWORD(1, 1), &wsaData);
	if(ret){
		cout << "�\�P�b�g�ʐM�@�\�̏������Ɏ��s���܂����B�v���O�������I�����܂��B\n";
		return -1;
	}

	listenSoc=socket(AF_INET, SOCK_STREAM, 0);
	if(listenSoc<0){
		cout << "�\�P�b�g���I�[�v���ł��܂���ł����B�v���O�������I�����܂��B\n";
		WSACleanup();
		return -1;
	}

	ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(portNum);
	saddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(listenSoc, (struct sockaddr *)&saddr, sizeof(saddr))==SOCKET_ERROR){
		cout << "bind�̃G���[\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}

	if(listen(listenSoc, 0)==SOCKET_ERROR){
		cout << "listen error.\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}else{
		cout << "�N���C�A���g�̐ڑ���҂��܂��B\n";
		cout << "�I������ꍇ��ctrl+c�ŋ����I�ɏI�����Ă��������B\n";
	}

	fromlen=sizeof(from);

	*soc=accept(listenSoc, (SOCKADDR *)&from, &fromlen);
	if(*soc==INVALID_SOCKET){
		cout << "accept error.\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}else{
		cout << inet_ntoa(from.sin_addr) << "���ڑ����Ă��܂����B\n";
	}
	closesocket(listenSoc);

	return 0;
}

int ClientStart(string serverName, unsigned short portNum, SOCKET* soc)
{
	WSADATA wsaData;
	HOSTENT* host;
	unsigned int addr;
	SOCKADDR_IN saddr;
	int ret;

	ret=WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(ret){
		cout << "�\�P�b�g�ʐM�@�\�̏������Ɏ��s���܂����B\n";
		return -1;
	}

	*soc=socket(AF_INET, SOCK_STREAM, 0);
	if(*soc<0){
		cout << "�\�P�b�g���I�[�v���ł��܂���ł����B\n";
		WSACleanup();
		return -1;
	}

	host=gethostbyname(serverName.c_str());
	if(!host){
		addr=inet_addr(serverName.c_str());
		host=gethostbyaddr((char*)&addr, 4, AF_INET);
	}
	if(!host){
		cout << "�z�X�g���������ł��B\n";
		closesocket(*soc);
		WSACleanup();
		return -1;
	}

	ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
	saddr.sin_family=host->h_addrtype;
	saddr.sin_port=htons(portNum);
	saddr.sin_addr.s_addr=*((u_long*)host->h_addr);

	if(connect(*soc, (SOCKADDR*)&saddr, sizeof(saddr))==SOCKET_ERROR){
		cout << "connect�̃G���[\n";
		closesocket(*soc);
		WSACleanup();
		return -1;
	}

	return 0;
}

int SocketEnd(SOCKET* soc)
{
	if(*soc<0){
		cout << "�����ȃ\�P�b�g�ł��B\n";
		return -1;
	}

	int ret;

	shutdown(*soc, SD_BOTH);
	closesocket(*soc);

	ret=WSACleanup();
	if(ret){
		cout << "WSACleanup�̎��s�B\n";
		return -1;
	}

	return 0;
}


void InputHost(string* host)
{
	cout << "�z�X�g���܂���IP�A�h���X����͂��Ă������� > ";
	getline(cin, *host);
	if(*host==""){
		cout << "���[�J���}�V���̃A�h���X���g�p���܂��B\n";
		*host="127.0.0.1";
	}
}

void InputPort(unsigned short* port)
{
	const unsigned short privatePortL=49152;
	const unsigned short privatePortU=65535;
	const unsigned short defaultPort=55555;
	string buffer;
	
	while(!(*port)){
		cout << "�|�[�g�ԍ��i" << privatePortL << " �` " << privatePortU << "�j����͂��Ă������� > ";
		getline(cin, buffer);
		if(buffer==""){
			cout << "�f�t�H���g�l�F" << defaultPort << "���g�p���܂��B\n";
			*port=defaultPort;
		}else{
			*port=atoi(buffer.c_str());
			if(*port<privatePortL || *port>privatePortU){
				cout << "�|�[�g�ԍ��� " << privatePortL << " �` " << privatePortU << " �͈̔͂ɂ��Ă��������B\n\n";
				*port=0;
			}
		}
	}
}

string Encode(int sendData, int sendDataSize)
{
	stringstream text;
	if(sendData == -1){
		text << 'P';
	}else{
		text << sendData;	
	}

	return text.str();
}

int Decode(char* receiveData)
{
	if(receiveData[0] == 'P'){
		return -1;
	}

	char numChar[3];

	numChar[0]=receiveData[0];
	numChar[1]=receiveData[1];
	numChar[2]='\0';

	return atoi(numChar);
}

