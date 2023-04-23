// SP_Lb6_1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_Lb6_1.h"
#include <windowsx.h>

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
BOOL WndProc_OnCreate(HWND, LPCREATESTRUCT);
void WndProc_OnDestroy(HWND);
void WndProc_OnPaint(HWND);
void WndProc_OnCommand(HWND, int, HWND, UINT);
//void WndProc_OnMouseMove(HWND, int, int, UINT);
void MoveWnd(HWND, int, int, int, int);
void DisplayCoordinates(HWND);

int cx = 150;
int cy = 300;
int x0 = 90;
int y0 = 190;
int d = 10;
int n = 4;
BOOL OnPaint = FALSE;
int countStepMove = 0;
HDC drawDC;
BOOL fDraw = FALSE;
POINT ptPrevious;


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
    LoadStringW(hInstance, IDC_SPLB61, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLB61));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLB61));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLB61);
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
      x0, y0, cx, cy, nullptr, nullptr, hInstance, nullptr);



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
    case WM_LBUTTONDOWN:

        ptPrevious.x = LOWORD(lParam);
        ptPrevious.y = HIWORD(lParam);
        return 0L;

    case WM_LBUTTONUP:
        if (OnPaint)
        {
            drawDC = GetDC(hWnd);
            MoveToEx(drawDC, ptPrevious.x, ptPrevious.y, NULL);
            LineTo(drawDC, LOWORD(lParam), HIWORD(lParam));
            ReleaseDC(hWnd, drawDC);
        }
        fDraw = FALSE;
        return 0L;
        HANDLE_MSG(hWnd, WM_DESTROY, WndProc_OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, WndProc_OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, WndProc_OnCommand);
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

void WndProc_OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
    DisplayCoordinates(hWnd);
    EndPaint(hWnd, &ps);
}

void WndProc_OnDestroy(HWND hWnd) {
    PostQuitMessage(0);
}

void WndProc_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    case ID_VK_B:
    {
        MoveWnd(hWnd, d, 0, 0, 0);
        break;
    }
    case ID_VK_N:
    {
        MoveWnd(hWnd, 0, 0, d, 0);
        break;
    }
    case ID_VK_M:
    {
        MoveWnd(hWnd, 0, 0, 0, d);
        break;
    }
    case ID_VK_LArr:
    {
        MoveWnd(hWnd, 0, d, 0, 0);
        break;
    }
    case ID_VK_ON_PAINT:
    {
        OnPaint = TRUE;
        countStepMove = 0;
        break;
    }
    case ID_VK_OFF_PAINT:
    {
        OnPaint = FALSE;
        break;
    }
    default:
        MessageBox(hWnd, TEXT("Choice nothing"), TEXT("Error"), MB_OK);
        break;
    }
}

void MoveWnd(HWND hWnd, int l, int r, int t, int b)
{
    RECT lpRect;
    memset(&lpRect, 0, sizeof(RECT));

    GetWindowRect(hWnd, &lpRect);
    SetWindowPos(hWnd, NULL, lpRect.left - l + r, lpRect.top - t + b, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    UpdateWindow(hWnd);
    DisplayCoordinates(hWnd);
}

void DisplayCoordinates(HWND hWnd)
{
    HDC hDC = GetDC(hWnd);

    TCHAR S[45];
    RECT CRect;
    GetClientRect(hWnd, &CRect);
    RECT WRect;
    GetWindowRect(hWnd, &WRect);

    SetTextColor(hDC, RGB(0, 127, 255));
    wsprintf(S, TEXT("x = %d y = %d "), WRect.left, WRect.top);
    TextOut(hDC, CRect.right - 100, CRect.bottom - 20, S, 45);
    ReleaseDC(hWnd, hDC);
}


