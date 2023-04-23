// SP_Lb_7.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_Lb_7.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HBITMAP hBitmap;
HWND hPic;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HFONT FAR PASCAL MyCreateFont(void);


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
    LoadStringW(hInstance, IDC_SPLB7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLB7));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLB7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLB7);
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
      250, 150, 700, 500, nullptr, nullptr, hInstance, nullptr);

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
            case IDM_OUTPUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_VIEW_GRAPHIC:
                HDC hDC;
                HDC hCompDC;
                HANDLE hBmp;
                HANDLE hOldBmp;
                BITMAP Bmp;
                hBmp = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), 0, 0, 0, LR_DEFAULTSIZE);
                hDC = GetDC(hWnd);
                hCompDC = CreateCompatibleDC(hDC);
                GetObject(hBmp, sizeof(BITMAP), &Bmp);
                hOldBmp = SelectObject(hCompDC, hBmp);
                RECT rct;
                GetClientRect(hWnd, &rct);
                StretchBlt(hDC, 80, 80, rct.right, rct.bottom,
                    hCompDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, SRCCOPY);
                SelectObject(hCompDC, hOldBmp);
                DeleteDC(hCompDC);
                DeleteObject(hBmp);
                ReleaseDC(hWnd, hDC);
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
            float nHeight = -MulDiv(20, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            LPCTSTR lpString = TEXT("Соколовский Вадим Сергеевич");
            HFONT hFont = CreateFont(nHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                TEXT("Microsoft Sans Serif"));
            HFONT hOldFont;
            hOldFont = (HFONT)SelectObject(hdc, (HGDIOBJ)hFont);
            SetTextColor(hdc, RGB(255, 100, 100));
            
            TextOut(hdc, 200, 100, lpString, lstrlen(lpString));

            HBRUSH hBr = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 255, 0));
            HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
            SelectObject(hdc, (HGDIOBJ)hPen);
            SelectObject(hdc, (HGDIOBJ)hOldFont);
            
            
            SelectObject(hdc, (HGDIOBJ)hBr);
            Ellipse(hdc, 30, 60, 90, 120);
            Ellipse(hdc, 90, 60, 150, 120);
            Rectangle(hdc, 60, 60, 120, 120);

            EndPaint(hWnd, &ps);
        }
        break;
   
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
    case WM_INITDIALOG: {
        hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        hPic = GetDlgItem(hDlg, IDI_FILEOUT);
        return (INT_PTR)TRUE;
    }

    case WM_PAINT: {
        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hPic, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ hbmOld = SelectObject(hdcMem, hBitmap);
        GetObject(hBitmap, sizeof(bm), &bm);
        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        EndPaint(hPic, &ps);
    }break;
        

    case WM_COMMAND:
        switch (wParam) {
        case IDOK: {
            EndDialog(hDlg, LOWORD(wParam));
        } break;
        case IDCANCEL: {
            EndDialog(hDlg, LOWORD(wParam));
        } break;
        case IDB_FILEOUT: {
            
        }break;

        default: {
        }break;
        }break;
    }
    return (INT_PTR)FALSE;
}

HFONT FAR PASCAL MyCreateFont(void)
{
    CHOOSEFONT cf;
    LOGFONT lf;
    HFONT hfont;

    // Initialize members of the CHOOSEFONT structure.  

    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = (HWND)NULL;
    cf.hDC = (HDC)NULL;
    cf.lpLogFont = &lf;
    cf.iPointSize = 0;
    cf.Flags = CF_SCREENFONTS;
    cf.rgbColors = RGB(0, 0, 0);
    cf.lCustData = 0L;
    cf.lpfnHook = (LPCFHOOKPROC)NULL;
    cf.lpTemplateName = (LPCTSTR)NULL;
    cf.hInstance = (HINSTANCE)NULL;
    cf.lpszStyle = (LPTSTR)NULL;
    cf.nFontType = SCREEN_FONTTYPE;
    cf.nSizeMin = 0;
    cf.nSizeMax = 0;


    // Display the CHOOSEFONT common-dialog box.  

    ChooseFont(&cf);

    // Create a logical font based on the user's  
    // selection and return a handle identifying  
    // that font.  

    hfont = CreateFontIndirect(cf.lpLogFont);
    return (hfont);
}
