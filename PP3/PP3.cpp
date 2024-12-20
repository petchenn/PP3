// PP3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

DWORD WINAPI MyThread(LPVOID lpParam)
{
    BOOL* active;
    active = (BOOL*)lpParam;
    //some code
    *active = FALSE;
}

void InitThread() {
    HANDLE hThread = NULL;
    DWORD ThreadID = 0;
    BOOL active = TRUE;
    hThread = CreateThread(NULL, 0, (LPFIBER_START_ROUTINE)MyThread, (void*)&active, 0, &ThreadID);
    while(active){}
    CloseHandle(hThread);
}

int main()
{
    std::cout << "Hello World!\n";
}

