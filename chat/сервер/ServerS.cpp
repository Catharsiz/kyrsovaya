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

SOCKET Connections[100];   // массив сокетов для хранения всех соединений
int Counter = 0;           // переменная, которая хранит индекс соединения


bool authentication(SOCKET newConnection) {
	string truePassword = "8f82ea2d2110b906e35b9351de773b44";

	int password_size;
	recv(newConnection, (char*)&password_size, sizeof(int), NULL);
	char *password = new char[password_size + 1];
	password[password_size] = '\0';
	recv(newConnection, password, password_size, NULL);

	string salt[19] = { "kЦл,©!»п__LьмѕФн", "EM™%ЋQeS\3м", "T§asddd","J`=Ло/hu“", "53ф	Lж9!s", "‹РЎZPc‘2Ш",
		"ЁйщЂr@ЌnyмЎ+r", "’g¶v5*кЭу1N	рП", "Й°№•К–ЫьАм", " ГОCA‚o1Дsь", "ќuќ/Х·›пODыд/УЭ]b","О“}FпT‰, l7Ш }[мВ73?щ!",
		"ыNIo'sњv¬4тrLдYn”‰мЕ", "ъРыgМ–§T, wL^‹\ }Р' r•", "0ЙїФBЏЙWЩjЪ’@>ЯЯРЙ","щXЖюњ—adsd" ,"¬ЇSч$,0±™D",
		"з3~­Y¦)ЋРЦ·i‚n0ћ’гр", "ZqUЬЪ[Ѓ" };

	int qqq = 0;
	for (int i = 0; i < 19; i++) {
		if ((md5::md5(salt[i] + password) == truePassword)) {
			qqq = 1;
			break;
		}
	}



	string msg = "WRONG PASSWORD";     //создаем строку
	if (qqq == 1) {
		msg = "SUCCESSFUL CONNECTING WITH THE SERVER";
	}
	int msg_size = msg.size();
	send(newConnection, (char*)&msg_size, sizeof(int), NULL);			//отправляем размер строки
	send(newConnection, msg.c_str(), msg_size, NULL);                 //посылаем ее клиенту

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


void ClientHandler(int index) {          //принимаем индекс соединения в сокет массиве(индекс клиента)
	int msg_size;							//переменная для хранения размера сообщения, которое будет отправлено от клиента

	while (true) {							//бесконечный цикл, в котором принимаются и отправляются сообщения клиентов
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);   //принимаем размер сообщения, которое поступит от клиента
		char *msg = new char[msg_size + 1];							//динамическое выделение памяти
		msg[msg_size] = '\0';	

		int z = recv(Connections[index], msg, msg_size, NULL);			//принимаем сообщение от клиента, отправившего его
		if (z == SOCKET_ERROR) {
			break;
		}

		cout << msg << " | ID[" << index << "]" << endl;

		for (int i = 0; i < Counter; i++) {					//рассылаем сообщение всем, кроме отправителя
			if (i == index) {
				continue;
			}
			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);			//отправляем размер сообщения
			send(Connections[i], msg, msg_size, NULL);							//отправляем сообщение
		}
		delete[] msg;											//освобождение памяти после динамического выделения
	}
	closesocket(Connections[index]);
}

int main(int argc, char* argv[]) {

	asdd();

CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckForDebugger, NULL, NULL, NULL);   //Если нужно передать значение (LPVOID)(переменные через запятую)
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckRemoteDebuggerPresent1, NULL, NULL, NULL);
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HardwareBreakpoints, NULL, NULL, NULL);
CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)isDebugger, NULL, NULL, NULL);

	cout << "This is SERVER\n";
	cout << "Waiting for clients connections \n";

	WSAData wsaData;                     //структура
	WORD DLLVersion = MAKEWORD(2, 1);    // запрашиваемая версия библиотеки winsock

	if (WSAStartup(DLLVersion, &wsaData) != 0) {     //функция загрузки сетевой библиотеки(1-й параметр - запрашиваемая версия библиотеки
		cout << "Error" << endl;					//2-й - ссылка на структура wsaData. Далее делаем проверку на удачную загрузку библиотеки
		exit(1);									//выход из программы, если библиотека не загрузилась
	}


	//заполнение информации об адресе сокета

	SOCKADDR_IN addr;								//структура для хранения адреса
	int sizeofaddr = sizeof(addr);                  //размер структуры SOCKADDR_IN
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");    //структура для хранения ип-адреса
	addr.sin_port = htons(1111);                      // порт для идентификации программы поступающими данными
	addr.sin_family = AF_INET;						// семейство протоколов, для инет протоколов константа AF_INET


	//Чтобы 2 компа могли установить соединение, нужно прослушивание на определенном порту

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);    //создаем сокет
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));       //привязываем адрес сокету(в параметрах сокет, указатель на структуру,размер структуры
	listen(sListen, SOMAXCONN);							// прослушивание порта в ожидании соединения со стороны клиента(в параметрах сокет и макс число запросов)


	//Объявляем новый сокет, чтобы удерживать соединение с клиентом

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {                                         //цикл, чтобы поддерживать до 100 соединений(клиентов)
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);    //Второй параметр(addr) будет держать сведения об ип адресе клиента
																			//функция возвращает указатель на новый сокет, который можно
																			//использовать для общения с клиентом

		//Проверка, подключился ли клиент к серверу.


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

				//Записываем новое соединение в массив
				int nick_size;
				recv(newConnection, (char*)&nick_size, sizeof(int), NULL);   //принимаем размер сообщения, которое поступит от клиента
				char *nick = new char[nick_size + 1];							//динамическое выделение памяти
				nick[nick_size] = '\0';
				recv(newConnection, nick, nick_size, NULL);			//принимаем сообщение от клиента, отправившего его

				string nickname = nick;

				string welcome = "User with nickname>> " + nickname + " <<connected to chat!";
				cout << welcome << endl;

				delete[] nick;

				Counter++;

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);  //создаем отдельный поток для ClientHandler
																											//т.е для ожидание и отправки сообщения
				// с каждым соединением будет запускаться новый поток для принятия сообщения от нового клиента
				// 3-й параметр это указатель на процедуру, с которой следует начать выполнение потока; 4-й - аргумент процедуры, переданный предыдущий аргументом

			}
		}
	}
	system("pause");
	return 0;
}