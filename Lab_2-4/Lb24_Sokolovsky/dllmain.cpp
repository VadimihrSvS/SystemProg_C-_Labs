// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

__declspec(dllexport) int Fun91(int x, int y);
__declspec(dllexport) float WINAPI Fun92(float x, float y);
__declspec(dllexport) void Fun93(double in, double* out);
__declspec(dllexport) int GetDllCalls();
__declspec(dllexport) int GetFuncCalls();

int g_nDllCallsCount;
int g_nFnCallsCount;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        g_nDllCallsCount++;
        MessageBox(NULL, L"Загружается библиотека Lb24_Sokolovsky. Проект Соколовского В.С. гр. 10333.", L"", MB_OK);
    } break;
    case DLL_THREAD_ATTACH: {
        //MessageBox(NULL, L"Загружается поток Lb24_Sokolovsky. Проект Соколовского В.С. гр. 10333.", L"", MB_OK);
    } break;
    case DLL_THREAD_DETACH: {
        //MessageBox(NULL, L"Отключается поток Lb24_Sokolovsky. Проект Соколовского В.С. гр. 10333.", L"", MB_OK);
    } break;
    case DLL_PROCESS_DETACH: {
        MessageBox(NULL, L"Отключается библиотека Lb24_Sokolovsky. Проект Соколовского В.С. гр. 10333.", L"", MB_OK);
    } break;
        
    }
    return TRUE;
}

int Fun91(int x, int y) {
    g_nFnCallsCount++;
    return x + y;
}

float WINAPI Fun92(float x, float y) {
    g_nFnCallsCount++;
    return x + y;
}

void Fun93(double in, double* out) {
    g_nFnCallsCount++;
    *out = in;
    return;
}

int GetDllCalls() {
    return g_nDllCallsCount;
}

int GetFuncCalls() {
    return g_nFnCallsCount;
}