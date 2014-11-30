#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>


#define MUTEX_NAME       "Mutex"
#define MAP_FILE_NAME    "Memory"
#define BUFFER_SIZE      100

using namespace std;

int _tmain(int argc, _TCHAR* argv[]){
	setlocale(LC_CTYPE, "rus");
	HANDLE hMutex = CreateMutex(NULL, FALSE, (LPCWSTR)MUTEX_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		puts("Мьютекс уже создан\n");
	
	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, (LPCWSTR)MAP_FILE_NAME);
	void* buffer = MapViewOfFile(hMapFile, FILE_WRITE_ACCESS, 0, 0, BUFFER_SIZE);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		hMapFile = OpenFileMapping(FILE_WRITE_ACCESS, FALSE, (LPCWSTR)MAP_FILE_NAME);
	else
	{	
		puts("Введите данные разделямой памяти: ");
		CHAR BufferString[BUFFER_SIZE];
		gets(BufferString);
		CopyMemory(buffer, BufferString, BUFFER_SIZE);
	}

	getchar();
	CHAR bufferString[BUFFER_SIZE];
	while (true)
	{
		puts("Критическая секция занята, ожидание...");
		WaitForSingleObject(hMutex, INFINITE);
		puts("\nВход в критическую секцию");
		CopyMemory(bufferString, buffer, BUFFER_SIZE);
		printf("Данные разделяемой памяти: %s\n",bufferString);
		Sleep(3000);
		puts("Выход из критической секции\n");
		ReleaseMutex(hMutex);
	}

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
	CloseHandle(hMutex);
	system("PAUSE");
	return 0;
}