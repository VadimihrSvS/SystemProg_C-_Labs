// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

__declspec(dllexport) void add(int x, int y);
__declspec(dllexport) void sub(int x, int y);
__declspec(dllexport) void setX(int x);
__declspec(dllexport) void setY(int y);
__declspec(dllexport) int getX();
__declspec(dllexport) int getY();
__declspec(dllexport) int getF1();
__declspec(dllexport) int getF2();



int _x, _y, f1, f2 = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}

void add(int x, int y) { f1 = x + y; }
void sub(int x, int y) { f2 = x - y; }
void setX(int x) { _x = x; }
void setY(int y) { _y = y; }
int getX() { return _x; }
int getY() { return _y; }
int getF1() { return f1; }
int getF2() { return f2; }



