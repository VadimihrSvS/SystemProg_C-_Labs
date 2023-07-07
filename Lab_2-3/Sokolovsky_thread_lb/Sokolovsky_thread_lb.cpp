// Sokolovsky_thread_lb.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sokolovsky_thread_lb.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
UINT g_uThCount = 0;
HANDLE hSecThread[4] = { NULL, NULL, NULL, NULL };
DWORD dwSecThreadId[4] = { 0, 0, 0 , 0 };
DWORD dwSecThreadData[4] = { 0, 0, 0 , 0 };
DWORD WINAPI ThreadFunc(LPVOID pvParam);
BOOL StartThread(int i, HWND hWnd, bool run);
TCHAR* pStr;
void DestroyThread(int i);
BOOL LowerPriority(int i);
BOOL HigherPriority(int i);
SYSTEMTIME systemtime_difference(SYSTEMTIME const& st1, SYSTEMTIME const& st2);
INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
CRITICAL_SECTION g_cs;
BOOL fSynchronization = FALSE;
HANDLE ghMutex;
HANDLE ghSemaphore;


struct THREAD_PARAM
{
    int Num;
    UINT XPos;
    UINT YPos;
    HWND ThrhWnd;
};

THREAD_PARAM ThrParam;



DWORD WINAPI MyThread(LPVOID param)
{
    
    pStr = (TCHAR*)param;
    return 0;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOKOLOVSKYTHREADLB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOLOVSKYTHREADLB));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOLOVSKYTHREADLB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOKOLOVSKYTHREADLB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPCTSTR str = L"Вторичный поток создал Соколовский";
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_THREAD_INFO: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_INFO), hWnd, Info, 0);
            } break;
            case IDM_THREAD1_INFO: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_INFO), hWnd, Info, 1);
            } break;
            case IDM_THREAD2_INFO: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_INFO), hWnd, Info, 2);
            } break;
            case IDM_THREAD1_CREATE: {
                StartThread(1, hWnd, true);
                

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE_WAITING, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DESTROY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD1_CREATE_WAITING: {
                StartThread(1, hWnd, false);


                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE_WAITING, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DESTROY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD1_PAUSE: {
                SuspendThread(hSecThread[1]);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD1_PAUSE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_RESUME, MF_ENABLED | MF_BYCOMMAND);

            } break;
            case IDM_THREAD1_RESUME: {
                ResumeThread(hSecThread[1]);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD1_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_RESUME, MF_GRAYED | MF_BYCOMMAND);

            } break;
            case IDM_THREAD1_DESTROY: {
                DestroyThread(1);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_CREATE_WAITING, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DESTROY, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_DOWN_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_PAUSE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD1_UP_PRIORITY, MF_GRAYED | MF_BYCOMMAND);

            } break;
            case IDM_THREAD1_UP_PRIORITY: {
                HMENU hMenu = GetMenu(hWnd);
                if (!HigherPriority(1))
                {
                    EnableMenuItem(hMenu, IDM_THREAD1_UP_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                }
                EnableMenuItem(hMenu, IDM_THREAD1_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD1_DOWN_PRIORITY: {
                HMENU hMenu = GetMenu(hWnd);
                if (!LowerPriority(1))
                {
                    EnableMenuItem(hMenu, IDM_THREAD1_DOWN_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                }
                EnableMenuItem(hMenu, IDM_THREAD1_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);

            } break;
            case IDM_THREAD2_CREATE: {
                StartThread(2, hWnd, true);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE_WAITING, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DESTROY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);

            } break;
            case IDM_THREAD2_CREATE_WAITING: {
                StartThread(2, hWnd, false);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE_WAITING, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DESTROY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD2_PAUSE: {
                SuspendThread(hSecThread[2]);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD2_PAUSE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_RESUME, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD2_RESUME: {
                ResumeThread(hSecThread[2]);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD2_PAUSE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_RESUME, MF_GRAYED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD2_DESTROY: {
                DestroyThread(2);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_CREATE_WAITING, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DESTROY, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_DOWN_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_PAUSE, MF_GRAYED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, IDM_THREAD2_UP_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
            } break;
            case IDM_THREAD2_UP_PRIORITY: {
                HMENU hMenu = GetMenu(hWnd);
                if (!HigherPriority(2))
                {
                    EnableMenuItem(hMenu, IDM_THREAD2_UP_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                }
                EnableMenuItem(hMenu, IDM_THREAD2_DOWN_PRIORITY, MF_ENABLED | MF_BYCOMMAND);

                break;
            } break;
            case IDM_THREAD2_DOWN_PRIORITY: {
                HMENU hMenu = GetMenu(hWnd);
                if (!LowerPriority(2))
                {
                    EnableMenuItem(hMenu, IDM_THREAD2_DOWN_PRIORITY, MF_GRAYED | MF_BYCOMMAND);
                }
                EnableMenuItem(hMenu, IDM_THREAD2_UP_PRIORITY, MF_ENABLED | MF_BYCOMMAND);
            } break;
            case ID_SYNCHRONIZATION_ON: {
                fSynchronization = TRUE;
                InitializeCriticalSection(&g_cs);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, ID_SYNCHRONIZATION_OFF, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, ID_SYNCHRONIZATION_ON, MF_GRAYED | MF_BYCOMMAND);
            }break;
            case ID_SYNCHRONIZATION_OFF: {
                fSynchronization = FALSE;
                DeleteCriticalSection(&g_cs);

                HMENU hMenu = GetMenu(hWnd);
                EnableMenuItem(hMenu, ID_SYNCHRONIZATION_ON, MF_ENABLED | MF_BYCOMMAND);
                EnableMenuItem(hMenu, ID_SYNCHRONIZATION_OFF, MF_GRAYED | MF_BYCOMMAND);
                break;
            }break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
              
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY: {
        DeleteCriticalSection(&g_cs);
        PostQuitMessage(0);
    } break;
    case WM_CREATE: {
        hSecThread[0] = GetCurrentThread();
        dwSecThreadId[0] = GetCurrentThreadId();
        InitializeCriticalSection(&g_cs);
        ghMutex = CreateMutex(NULL, false, NULL);
        ghSemaphore = CreateSemaphore(NULL, 4, 4, NULL);

        return 1;
    } break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD WINAPI ThreadMutex(LPVOID pvParam) {
    DWORD dwCount = 0, dwWaitResult;

    dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);
    switch (dwWaitResult)
    {
    case WAIT_OBJECT_0: {
        THREAD_PARAM* ThrParam;
        ThrParam = (THREAD_PARAM*)pvParam;

        #define MAXSIZE  60
        TCHAR CreepingLine[MAXSIZE];
        TCHAR buf[MAXSIZE] = { 0 };

        int   iBeginningIndex;

        int   StringLength = 0;

        RECT  rc;
        HDC   hDC;
        int cRun = 0;
        int N = 5;// lstrlen(CreepingLine) + 20;

        wsprintf(CreepingLine, TEXT("Вторичный поток создал Соколовский"));

        StringLength = iBeginningIndex = lstrlen(CreepingLine);

        lstrcpy(buf, CreepingLine);

        GetClientRect(ThrParam->ThrhWnd, &rc);
        rc.top = ThrParam->YPos;
        rc.left = ThrParam->XPos;
        rc.right = rc.right + lstrlen(CreepingLine);

        hDC = GetDC(ThrParam->ThrhWnd);

        while (TRUE)
        {

            cRun = 0;
            while (cRun < N - 1)
            {
                for (int j = 0; j < StringLength + 1; j++)
                {
                    if (iBeginningIndex == 0)
                    {
                        iBeginningIndex = StringLength;
                        cRun++;
                    }

                    TCHAR c;
                    c = buf[StringLength];
                    for (int i = StringLength; i > 0; i--)
                        buf[i] = buf[i - 1];
                    buf[0] = c;

                    TextOut(hDC, rc.left, rc.top, buf, lstrlen(buf));
                    TCHAR bufnum[100];

                    wsprintf(bufnum, _T("%d"), cRun);
                    TextOut(hDC, rc.left, rc.top + 100, bufnum, 2);

                    iBeginningIndex--;

                    Sleep(20);
                }
                if(cRun % 4 == 0 && cRun != 0) ReleaseMutex(ghMutex);
            }
            break;
        }
        return 0;
    }
    }

}

DWORD WINAPI ThreadFunc(LPVOID pvParam)
{

    THREAD_PARAM* ThrParam;
    ThrParam = (THREAD_PARAM*)pvParam;

#define MAXSIZE  60
    TCHAR CreepingLine[MAXSIZE];
    TCHAR buf[MAXSIZE] = { 0 };

    int   iBeginningIndex;

    int   StringLength = 0;

    RECT  rc;
    HDC   hDC;
    int cRun = 0;
    int N = 5;// lstrlen(CreepingLine) + 20;

    wsprintf(CreepingLine, TEXT("Вторичный поток создал Соколовский")); 

    StringLength = iBeginningIndex = lstrlen(CreepingLine);

    lstrcpy(buf, CreepingLine);

    GetClientRect(ThrParam->ThrhWnd, &rc);
    rc.top = ThrParam->YPos;
    rc.left = ThrParam->XPos;
    rc.right = rc.right + lstrlen(CreepingLine);

    hDC = GetDC(ThrParam->ThrhWnd);

    while (TRUE)
    {
        if (fSynchronization)
            EnterCriticalSection(&g_cs);

        cRun = 0;
        while (cRun < N - 1)
        {
            for (int j = 0; j < StringLength + 60; j++)
            {
                if (iBeginningIndex == 0)
                {
                    iBeginningIndex = StringLength;
                    cRun++;
                }

                TCHAR c;
                c = buf[StringLength];
                for (int i = StringLength; i > 0; i--)
                    buf[i] = buf[i - 1];
                buf[0] = c;

                TextOut(hDC, rc.left, rc.top, buf, lstrlen(buf));
                TCHAR bufnum[100];

                wsprintf(bufnum, _T("%d"), j);
                TextOut(hDC, rc.left, rc.top + 100, bufnum, 2);

                iBeginningIndex--;
                
                Sleep(20);
            }
            if (fSynchronization) LeaveCriticalSection(&g_cs);
            
            break;
        }
        break;
    }
    return 0;
}

BOOL StartThread(int i, HWND hWnd, bool run)
{
    if (hSecThread[i] == NULL)
    {
        ThrParam.ThrhWnd = hWnd;
        ThrParam.Num = i;
        ThrParam.XPos = 100;
        ThrParam.YPos = i * 50 + 150;

        DWORD dwParam = (run) ? 0 : CREATE_SUSPENDED;

            hSecThread[i] = CreateThread(NULL, 0, ThreadFunc, &ThrParam, dwParam, &(dwSecThreadId[i]));


        if (!hSecThread[i])			
        {
            MessageBox(hWnd, TEXT("Ошибка. Поток не создан"), TEXT("Ошибка!"), MB_OK);
            return(FALSE);
        }
        else
        {
            g_uThCount++;
        }
    }
    else
    {
        MessageBox(hWnd, TEXT("Ошибка! Поток в работе! Вы не можете создать ещё один"), TEXT("Ошибка!"), MB_OK);
    }
}

void DestroyThread(int i)
{
    TerminateThread(hSecThread[i], 1001);
    hSecThread[i] = NULL;
    dwSecThreadId[i] = 0;
    dwSecThreadData[i] = 0;
}


BOOL HigherPriority(int i)
{
    int thPriority = GetThreadPriority(hSecThread[i]);

    switch (thPriority)
    {
    case THREAD_PRIORITY_LOWEST:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_BELOW_NORMAL);
        return true;
        break;

    case THREAD_PRIORITY_BELOW_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_NORMAL);
        return true;
        break;

    case THREAD_PRIORITY_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_ABOVE_NORMAL);
        return true;
        break;

    case THREAD_PRIORITY_ABOVE_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_HIGHEST);
        return false;
        break;
    default:
        break;
    }
}

BOOL LowerPriority(int i)
{
    int thPriority = GetThreadPriority(hSecThread[i]);
    switch (thPriority)
    {
    case THREAD_PRIORITY_BELOW_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_LOWEST);
        return false;
        break;
    case THREAD_PRIORITY_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_BELOW_NORMAL);
        return true;
        break;
    case THREAD_PRIORITY_ABOVE_NORMAL:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_NORMAL);
        return true;
        break;
    case THREAD_PRIORITY_HIGHEST:
        SetThreadPriority(hSecThread[i], THREAD_PRIORITY_ABOVE_NORMAL);
        return true;
        break;
    default:
        return true;
        break;
    }
}

INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i = (int)lParam;
    int index = 0;
    TCHAR buff[1000];
    HWND listbox = GetDlgItem(hDlg, IDC_LIST1);

    switch (message)
    {
    case WM_INITDIALOG:
    {
        wsprintf(buff, TEXT("Дескриптор :  %d"), hSecThread[i]);
        SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)buff);

        wsprintf(buff, TEXT("Индентификатор :  %d"), dwSecThreadId[i]);
        SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)buff);

        DWORD dwStatus;
            GetExitCodeThread(hSecThread[i], &dwStatus);
        if (dwStatus == STILL_ACTIVE)
        {
            SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)TEXT("Состояние потока : активный"));
        }
        else
        {
            wsprintf(buff, TEXT("Состояние потока : не активен, код = %d"), dwStatus);
            SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)buff);
        }

        int priority = GetThreadPriority(hSecThread[i]);
        wsprintf(buff, TEXT("Приоритет: %d"), priority);
        SendMessage(listbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

        FILETIME lpCreationTime, lpExitTime, lpKernelTime, lpUserTime;
        GetThreadTimes(hSecThread[i], &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime);
        SYSTEMTIME lpSystemTime, lpCreationT, lpExitT;
        if (hSecThread[i])
        {
            FileTimeToSystemTime(&lpCreationTime, &lpCreationT);
            GetLocalTime(&lpExitT);
            lpSystemTime = systemtime_difference(lpCreationT, lpExitT);
        }
        else
        {
            lpSystemTime.wMinute = 0;
            lpSystemTime.wMilliseconds = 0;
            lpSystemTime.wSecond = 0;
        }

        wsprintf(buff, TEXT("%d мин %d cек %d миллисек"), lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds);
        SendMessage(listbox, LB_ADDSTRING, (WPARAM)0, (LPARAM)buff);

        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
SYSTEMTIME systemtime_difference(SYSTEMTIME const& st1, SYSTEMTIME const& st2)
{
    FILETIME ft1;
    SystemTimeToFileTime(&st1, &ft1);
    ULARGE_INTEGER const u1 = { ft1.dwLowDateTime, ft1.dwHighDateTime };

    FILETIME ft2;
    SystemTimeToFileTime(&st2, &ft2);
    ULARGE_INTEGER const u2 = { ft2.dwLowDateTime, ft2.dwHighDateTime };

    ULARGE_INTEGER u3;
    u3.QuadPart = (u1.QuadPart > u2.QuadPart ? u1.QuadPart : u2.QuadPart) - (u1.QuadPart < u2.QuadPart ? u1.QuadPart : u2.QuadPart);
    FILETIME const ft3 = { u3.LowPart, u3.HighPart };

    SYSTEMTIME st3;
    FileTimeToSystemTime(&ft3, &st3);
    // FILETIME's starting point is 1601-01-01 
    st3.wYear -= 1601;
    st3.wMonth -= 1;
    st3.wDay -= 1;
    st3.wDayOfWeek = 0; // useless for this purpose, make it always 0 so it's always ignored 
    return st3;
}