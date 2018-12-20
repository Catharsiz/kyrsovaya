// qwer.cpp : Определяет экспортированные функции для приложения DLL.
#include "stdafx.h" 
#pragma comment(lib, "md5hash.lib")
#include "md5.h"
#include "qwer.h"

#include <fstream> 
#include <iostream> 
#include <ctime> 
#include <string> 



using namespace std;
using namespace md5;
void asdd()
{
	//Start opening file
	ifstream inBigArrayfile;
	inBigArrayfile.open("ClientC.exe", std::ios::binary | std::ios::in);

	//Find length of file 
	inBigArrayfile.seekg(0, std::ios::end);
	long Length = inBigArrayfile.tellg();
	inBigArrayfile.seekg(0, std::ios::beg);

	//read in the data from your file 
	char * InFileData = new char[Length];
	inBigArrayfile.read(InFileData, Length);

	//Calculate MD5 hash 
	std::string Temp =md5x(InFileData, Length);
	string s = "192473a2777e91cd509b29f2e00ef6b4";
	if (md5::md5(Temp.c_str()) != s)
	{
		exit(-1);
	}
	//Clean up 
	delete[] InFileData;
}