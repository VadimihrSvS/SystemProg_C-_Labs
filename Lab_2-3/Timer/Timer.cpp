//Т3.Ждущий таймер.
//Создать консольное приложение, которое через каждые n = 15 секунд будет
//выводить диалоговое окно MessageBox с текстом «Вызов номер <i>
//Продолжить – RETRY.Завершить работу - ABORT».После нажатия кнопки
//RETRY или ABORT в MessageBox приложение будет соответствующим
//образом реагировать.Если ответ задерживается более чем на 15 секунд, то
//это воспринимается как ответ “ RETRY ”


#include <windows.h>
#include <stdio.h>

void OnTimer();
DWORD WINAPI ThreadFunc1(LPVOID);
HANDLE hSecThread = NULL;
DWORD dwSecThreadId = 0;

void main(void)
{
	const int nTimerUnitsPerSecond = 10000000;
	int cAbrasionCount = 0;
	SYSTEMTIME st;
	LARGE_INTEGER li;
	
	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	GetLocalTime(&st);
	printf("Start time: \t\t%.2d:%.2d:%.2d\n\n", st.wHour, st.wMinute, st.wSecond);
	li.QuadPart = -(15 * nTimerUnitsPerSecond);
	if (SetWaitableTimer(hTimer, &li, 15 * 1000, NULL, NULL, FALSE)){
		while (TRUE){
			GetLocalTime(&st);
			cAbrasionCount++;
			printf("%d.Timer abrasion: \t%.2d:%.2d:%.2d\n", cAbrasionCount, st.wHour, st.wMinute, st.wSecond);

			hSecThread = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)cAbrasionCount, 0, &dwSecThreadId);
			
			DWORD dw = WaitForSingleObject(hTimer, INFINITE);
			
			TerminateThread(hSecThread, 1001);
			hSecThread = NULL;
			dwSecThreadId = 0;
		}
	}
}

void OnTimer()
{
	HWND pWnd = FindWindow(NULL,TEXT("MSG"));
	if (pWnd != NULL)
	{
		// Send close command to the message box window
		SendMessage(pWnd, WM_CLOSE, 0, 0);
	}
}

DWORD WINAPI ThreadFunc1(LPVOID pvParam)
{
	int cAbrasionCount;
	cAbrasionCount = (int)pvParam;
	TCHAR m[200];
	wsprintf(m, TEXT("Запуск № %d \n Продолжить – RETRY. Завершить работу - CANCEL"), cAbrasionCount);
	SetTimer(NULL, NULL, 15000, (TIMERPROC)OnTimer);

	if (MessageBox(NULL, m, TEXT("MSG"), MB_RETRYCANCEL)==IDCANCEL)
	{
		exit(0);
	}

	return 0;
}


