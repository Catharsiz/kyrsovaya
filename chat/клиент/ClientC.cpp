#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "qwer.lib")
#include <winsock2.h>
#include <fstream>
#include <iostream>

#include <string>
#include <time.h>
#include <tchar.h> 
#include <Strsafe.h>
#include "qwer.h"
using namespace std;

#pragma warning(disable: 4996)


boolean password(SOCKET Connection) {
	char *pass = new char[25];
	cout << "Enter password:" << endl;
	cin >> pass;

	if (pass[0] != NULL) {
		int z = (int)pass[5];
		z = z + 5;
		pass[0] = (char)z;
	}

	if (pass[2] != NULL) {
		int z = (int)pass[3];
		z = z + 9 / 2;
		pass[2] = (char)z;
	}

	if (pass[3] != NULL) {
		int z = (int)pass[3];
		z = z * 5 / 7 + 8;
		pass[3] = (char)z;
	}

	if (pass[5] != NULL) {
		int z = (int)pass[3];
		z = z / 3 + 10;
		pass[5] = (char)z;
	}

	if (pass[7] != NULL) {
		int z = (int)pass[3];
		z = z - 1;
		pass[7] = (char)z;
	}

	string password = pass;
	delete[] pass;

	//Отправляем пароль на сервер
	int password_size = password.size();
	send(Connection, (char*)&password_size, sizeof(int), NULL);
	send(Connection, password.c_str(), password_size, NULL);


	int allow_size;
	recv(Connection, (char*)&allow_size, sizeof(int), NULL);    
	char *allow1 = new char[allow_size + 1];					
	allow1[allow_size] = '\0';
	recv(Connection, allow1, allow_size, NULL);					
	cout << allow1 << endl;
	string allow = allow1;

	if (allow == "WRONG PASSWORD") {
		return false;
	} else {
		return true;
	}
	
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
		}}	Sleep(1000);}

 
void CheckRemoteDebuggerPresent1() {
	BOOL isDebuggerPresent = FALSE;
	if (CheckRemoteDebuggerPresent(GetCurrentProcess(),
		&isDebuggerPresent))
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

SOCKET Connection;
void ClientHandler() {				//функция для принятия отправленного сервером сообщения, работает в отдельном потоке
	int msg_size;						//хранение размера строки
	while (true) {                  //принимаем сообщения от сервера и выводим их на экран
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);    //принимаем размер сообщения
		char *msg = new char[msg_size + 1];					//создаем динамический массив
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);					//принимаем сообщение
		cout << msg << endl;
		delete[] msg;													//освобождаем память
	}
}


int main(int argc, char* argv[]) {

	asdd();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckForDebugger, NULL, NULL, NULL);   //Если нужно передать значение (LPVOID)(переменные через запятую)
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckRemoteDebuggerPresent1, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HardwareBreakpoints, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)isDebugger, NULL, NULL, NULL);

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	//Загрузка сетевой библиотеки
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;										//
	int sizeofaddr = sizeof(addr);                         //
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");          //   Информация об адресе сокета, копируется из сервера
	addr.sin_port = htons(1111);                            //
	addr.sin_family = AF_INET;                              //



	//создаем новый сокет для соединения с сервером

	Connection = socket(AF_INET, SOCK_STREAM, NULL);             //создаем новый сокет для соединения с сервером
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {     //пытаемся присоединиться к серверу и проверка, удалось ли клиенту соединиться с сервом
		cout << "Error: failed connect to server.\n";
		return 1;
	}
	int amountOfTry = 5;
	int good = 0;
	while (amountOfTry > 0 && good==0) {
		if (password(Connection) == true) {
			good = 1;
			break;
		}
		cout << "you have " << amountOfTry - 1 << " more attempts" << endl;
		amountOfTry--;
	}
	if (amountOfTry <= 0) {
		closesocket(Connection);
		exit(-1);
	}
	else if (amountOfTry > 0)
	{

		char *nick = new char[5];
		cout << "enter your nickname" << endl;
		cin >> nick;

		string nickname = nick;

		int nick_size = nickname.size();
		send(Connection, (char*)&nick_size, sizeof(int), NULL);
		send(Connection, nickname.c_str(), nick_size, NULL);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL); //создаем поток для принятия сообщений от сервера

		while (true) {
			string msg1;  	//считываем написанные пользователем символы, записываем в переменную
			char *enter = new char[5];
			cin >> enter;
			msg1 = enter;
			msg1 = nickname + ": " + msg1;
			int msg_size = msg1.size();								//записываем размер сообщения
			send(Connection, (char*)&msg_size, sizeof(int), NULL);   //отправляем размер сообщения на сервер.
			send(Connection, msg1.c_str(), msg_size, NULL);				//отправляем сообщение
			Sleep(10);												//пауза
		}
		system("pause");
		return 0;
	}
}

