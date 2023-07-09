// ClientLB24Exp.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ClientLB24Exp.h"

#define MAX_LOADSTRING 100
typedef int(*FUN91) (int, int);
typedef float(WINAPI *FUN92) (float, float);
typedef void( *FUN93) (double, double*);
typedef int(*GDC) ();
typedef int(*GFC) ();


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
static HINSTANCE hLibrary;
static FUN91 fun91Func;
static FUN92 fun92Func;
static FUN93 fun93Func;
static GDC getDllCalls;
static GFC getFuncCalls;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_CLIENTLB24EXP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENTLB24EXP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENTLB24EXP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENTLB24EXP);
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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
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
            
            TCHAR bufnum91[40];
            TCHAR bufnum92[40];
            TCHAR bufnum93[40];
            TCHAR bufnum94[40];
            TCHAR bufnum95[40];

            RECT rc;

            GetClientRect(hWnd, &rc);

            double dbuf; 
            fun93Func(2.16, &dbuf);

            wsprintf(bufnum91, _T("Fun91(2, 3) = %d"), fun91Func(2, 3));
            swprintf_s(bufnum92, _T("Fun92(5.1, 3.2) = %f"), fun92Func(5.1f, 3.2f));
            swprintf_s(bufnum93, _T("Fun93(2.16, fun93) = %f"), dbuf);
            wsprintf(bufnum94, _T("Счетчик вызова библиотеки - %d"), getDllCalls());
            wsprintf(bufnum95, _T("Счетчик вызова функции - %d"), getFuncCalls());

            TextOut(hdc, 100, 100, bufnum91, lstrlen(bufnum91));
            TextOut(hdc, 100, 150, bufnum92, lstrlen(bufnum92));
            TextOut(hdc, 100, 200, bufnum93, lstrlen(bufnum93));
            TextOut(hdc, 100, 250, bufnum94, lstrlen(bufnum94));
            TextOut(hdc, 100, 300, bufnum95, lstrlen(bufnum95));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CREATE: {
        hLibrary = LoadLibrary(_T("Lb24_Sokolovsky"));
        if (hLibrary)
        {
            fun91Func = (FUN91)GetProcAddress(hLibrary, "Fun91");
            if (fun91Func == NULL)
            {
                MessageBox(hWnd, _T("Функция Fun91 не найдена"),
                    _T("LoadLibrary"), MB_OK | MB_ICONQUESTION);
                DestroyWindow(hWnd);
                return 0;
            }
            fun92Func = (FUN92)GetProcAddress(hLibrary, "Fun92");
            if (fun92Func == NULL)
            {
                MessageBox(hWnd, _T("Функция Fun92 не найдена"),
                    _T("LoadLibrary"), MB_OK | MB_ICONQUESTION);
                DestroyWindow(hWnd);
                return 0;
            }
            fun93Func = (FUN93)GetProcAddress(hLibrary, "Fun93");
            if (fun93Func == NULL)
            {
                MessageBox(hWnd, _T("Функция Fun93 не найдена"),
                    _T("LoadLibrary"), MB_OK | MB_ICONQUESTION);
                DestroyWindow(hWnd);
                return 0;
            }
            getDllCalls = (GDC)GetProcAddress(hLibrary, "GetDllCalls");
            if (getDllCalls == NULL)
            {
                MessageBox(hWnd, _T("Функция GetDllCalls не найдена"),
                    _T("LoadLibrary"), MB_OK | MB_ICONQUESTION);
                DestroyWindow(hWnd);
                return 0;
            }
            getFuncCalls = (GFC)GetProcAddress(hLibrary, "GetFuncCalls");
            if (getFuncCalls == NULL)
            {
                MessageBox(hWnd, _T("Функция GetFuncCalls не найдена"),
                    _T("LoadLibrary"), MB_OK | MB_ICONQUESTION);
                DestroyWindow(hWnd);
                return 0;
            }
        }
        else
        {
            MessageBox(hWnd, _T("Библиотека не найдена"), _T("LoadLibrary"),
                MB_OK | MB_ICONQUESTION);
            DestroyWindow(hWnd);
            return 0;
        }
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
