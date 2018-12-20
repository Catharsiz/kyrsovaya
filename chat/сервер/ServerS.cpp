#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "qwer.lib")
#pragma comment(lib, "md5hash.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include "md5.h"
#include "qwer.h"


#pragma warning(disable:4996)
using namespace std;
using namespace md5;

SOCKET Connections[100];   // ������ ������� ��� �������� ���� ����������
int Counter = 0;           // ����������, ������� ������ ������ ����������


bool authentication(SOCKET newConnection) {
	string truePassword = "8f82ea2d2110b906e35b9351de773b44";

	int password_size;
	recv(newConnection, (char*)&password_size, sizeof(int), NULL);
	char *password = new char[password_size + 1];
	password[password_size] = '\0';
	recv(newConnection, password, password_size, NULL);

	string salt[19] = { "k��,�!��__L����", "EM�%�QeS\3�", "T�asddd","J`=��/hu�", "53�	L�9!s", "�СZPc�2�",
		"����r@�ny�+r", "�g�v5*���1N	��", "ɰ��ʖ����", " ��CA�o1�s�", "�u�/շ��OD��/��]b","Γ}F�T�, l7� }[��73?�!",
		"�NIo's�v�4�rL�Yn����", "���g̖�T, wL^�\ }�'�r�", "0ɿ�B��W�jڒ@>����","�X����adsd" ,"��S�$,0��D",
		"�3~�Y�)��ַi�n0����", "ZqU��[�" };

	int qqq = 0;
	for (int i = 0; i < 19; i++) {
		if ((md5::md5(salt[i] + password) == truePassword)) {
			qqq = 1;
			break;
		}
	}



	string msg = "WRONG PASSWORD";     //������� ������
	if (qqq == 1) {
		msg = "SUCCESSFUL CONNECTING WITH THE SERVER";
	}
	int msg_size = msg.size();
	send(newConnection, (char*)&msg_size, sizeof(int), NULL);			//���������� ������ ������
	send(newConnection, msg.c_str(), msg_size, NULL);                 //�������� �� �������

	if (msg == "WRONG PASSWORD") {
		return false;
	}
	else
		return true;
}


void HardwareBreakpoints() {
	CONTEXT ctx = {};
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (GetThreadContext(GetCurrentThread(), &ctx))
	{
		if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
	}
	Sleep(1000);
}


void CheckRemoteDebuggerPresent1() {
	BOOL isDebuggerPresent = FALSE;
	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent))
	{
		if (isDebuggerPresent)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
	}
	Sleep(1000);
}


void CheckForDebugger() {
	HANDLE hOlly = FindWindow(TEXT("ollydbg"), NULL);
	HANDLE WireShark = FindWindow(TEXT("Wireshark"), NULL);
	HANDLE CommomView = FindWindow(TEXT("cv"), NULL);
	HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);
	HANDLE hScylla1 = FindWindow(NULL, TEXT("Scylla x86 v0.9.7c"));
	HANDLE hScylla2 = FindWindow(NULL, TEXT("Scylla x64 v0.9.7c"));
	HANDLE x32_dbg = FindWindow(NULL, TEXT("x32_dbg"));
	HANDLE x64_dbg = FindWindow(NULL, TEXT("x64_dbg"));
	HANDLE IDA = FindWindow(NULL, TEXT("IDA"));

	if (hOlly) {
		exit(-1);
	}
	if (hWinDbg) {
		exit(-1);
	}
	if (hScylla1) {
		exit(-1);
	}
	if (hScylla2) {
		exit(-1);
	}
	if (x32_dbg) {
		exit(-1);
	}
	if (x64_dbg) {
		exit(-1);
	}
	if (WireShark) {
		exit(-1);
	}
	if (CommomView) {
		exit(-1);
	}
	if (IDA) {
		exit(-1);
	}
	Sleep(1000);
}

void isDebugger() {
	if (IsDebuggerPresent()) {
		exit(-1);
	}
	Sleep(1000);
}


void ClientHandler(int index) {          //��������� ������ ���������� � ����� �������(������ �������)
	int msg_size;							//���������� ��� �������� ������� ���������, ������� ����� ���������� �� �������

	while (true) {							//����������� ����, � ������� ����������� � ������������ ��������� ��������
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);   //��������� ������ ���������, ������� �������� �� �������
		char *msg = new char[msg_size + 1];							//������������ ��������� ������
		msg[msg_size] = '\0';	

		int z = recv(Connections[index], msg, msg_size, NULL);			//��������� ��������� �� �������, ������������ ���
		if (z == SOCKET_ERROR) {
			break;
		}

		cout << msg << " | ID[" << index << "]" << endl;

		for (int i = 0; i < Counter; i++) {					//��������� ��������� ����, ����� �����������
			if (i == index) {
				continue;
			}
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);			//���������� ������ ���������
			send(Connections[i], msg, msg_size, NULL);							//���������� ���������
		}
		delete[] msg;											//������������ ������ ����� ������������� ���������
	}
	closesocket(Connections[index]);
}

int main(int argc, char* argv[]) {

	asdd();

CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckForDebugger, NULL, NULL, NULL);   //���� ����� �������� �������� (LPVOID)(���������� ����� �������)
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckRemoteDebuggerPresent1, NULL, NULL, NULL);
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HardwareBreakpoints, NULL, NULL, NULL);
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)isDebugger, NULL, NULL, NULL);

	cout << "This is SERVER\n";
	cout << "Waiting for clients connections \n";

	WSAData wsaData;                     //���������
	WORD DLLVersion = MAKEWORD(2, 1);    // ������������� ������ ���������� winsock

	if (WSAStartup(DLLVersion, &wsaData) != 0) {     //������� �������� ������� ����������(1-� �������� - ������������� ������ ����������
		cout << "Error" << endl;					//2-� - ������ �� ��������� wsaData. ����� ������ �������� �� ������� �������� ����������
		exit(1);									//����� �� ���������, ���� ���������� �� �����������
	}


	//���������� ���������� �� ������ ������

	SOCKADDR_IN addr;								//��������� ��� �������� ������
	int sizeofaddr = sizeof(addr);                  //������ ��������� SOCKADDR_IN
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");    //��������� ��� �������� ��-������
	addr.sin_port = htons(1111);                      // ���� ��� ������������� ��������� ������������ �������
	addr.sin_family = AF_INET;						// ��������� ����������, ��� ���� ���������� ��������� AF_INET


	//����� 2 ����� ����� ���������� ����������, ����� ������������� �� ������������ �����

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);    //������� �����
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));       //����������� ����� ������(� ���������� �����, ��������� �� ���������,������ ���������
	listen(sListen, SOMAXCONN);							// ������������� ����� � �������� ���������� �� ������� �������(� ���������� ����� � ���� ����� ��������)


	//��������� ����� �����, ����� ���������� ���������� � ��������

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {                                         //����, ����� ������������ �� 100 ����������(��������)
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);    //������ ��������(addr) ����� ������� �������� �� �� ������ �������
																			//������� ���������� ��������� �� ����� �����, ������� �����
																			//������������ ��� ������� � ��������

		//��������, ����������� �� ������ � �������.


		if (newConnection == 0) {
			cout << "Error #2\n";
		}
		else
		{
			int amountOfTry = 5;
			int good = 0;
			while (amountOfTry > 0 && good==0) {
				if (authentication(newConnection) == true) {
					good = 1;
				}
				amountOfTry--;
			}

			if (amountOfTry <= 0) {
				cout << "Connection attempt failed" << endl;
				closesocket(newConnection);
			}
			else {
				Connections[i] = newConnection;

				//���������� ����� ���������� � ������
				int nick_size;
				recv(newConnection, (char*)&nick_size, sizeof(int), NULL);   //��������� ������ ���������, ������� �������� �� �������
				char *nick = new char[nick_size + 1];							//������������ ��������� ������
				nick[nick_size] = '\0';
				recv(newConnection, nick, nick_size, NULL);			//��������� ��������� �� �������, ������������ ���

				string nickname = nick;

				string welcome = "User with nickname>> " + nickname + " <<connected to chat!";
				cout << welcome << endl;

				delete[] nick;

				Counter++;

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);  //������� ��������� ����� ��� ClientHandler
																											//�.� ��� �������� � �������� ���������
				// � ������ ����������� ����� ����������� ����� ����� ��� �������� ��������� �� ������ �������
				// 3-� �������� ��� ��������� �� ���������, � ������� ������� ������ ���������� ������; 4-� - �������� ���������, ���������� ���������� ����������

			}
		}
	}
	system("pause");
	return 0;
}