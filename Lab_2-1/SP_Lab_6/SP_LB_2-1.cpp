// SP_Lab_6.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_LB_2-1.h"
#include "commdlg.h"
#include "shellapi.h"
#include <string>
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                        // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                          // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];                    // имя класса главного окна
HANDLE ProcHandle[4] = {NULL, NULL, NULL, NULL};        // для дескрипторов процессов
DWORD ProcId[4] = {0, 0, 0, 0};                         // для итендификаторов процессов
HANDLE ThreadHandle[4] = { NULL, NULL, NULL, NULL };    // для дескрипторов потоков
DWORD ThreadId[4] = { 0, 0, 0, 0 };                     // для итендификаторов потоков
LPCTSTR ProcImage[4] = { 0,                          // для указателей строк, идентифицирущих файлы запускаемых программ
                        L"C:\\Windows\\notepad.exe",
                        L"C:\\Windows\\notepad.exe",
                        L"C:\\Windows\\WinSxS\\wow64_microsoft-windows-calc_31bf3856ad364e35_10.0.19041.1_none_6a03b910ee7a4073\\calc.exe"
};       
TCHAR CmdParam[4][260] = { TEXT("SP_LB2_1.exe"),                        // для строк c параметрами запускаемых программ.   
                           TEXT("notepad.exe"),
                           TEXT("Notepad D:\\С++\\SP_Lab_6Sol\\SP_Lab_6\\SP_LB_2-1.cpp"),
                           TEXT("calc.exe")
};          
WORD showWnd = SW_NORMAL;

///////////////////////////////////////

LPCTSTR fileLocation = L"D:\\С++\\SP_Lab_6Sol\\SP_Lab_6\\SP_LB_2 - 1.cpp";
LPCTSTR fileName;
DWORD nCharRead;
const DWORD MaxLength = 0x7fff;
char text[MaxLength];
int sx, sy;
TCHAR name[256] = _T("");
TCHAR lpszFileSpec[MAX_PATH] = TEXT("       ");
RECT rt;
HDC hdc;

///////////////////////////////////////

FILETIME ftKernelTimeStart[5], ftUserTimeStart[5], ftDummy[5], ftCreate[5];//для хранения времени при создании
HWND hWnd;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    dialogProcess(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    dialogTestProcess(HWND, UINT, WPARAM, LPARAM);
//void showProcess(HWND hWnd) {
//    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, dialogProcess);
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)

{

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    //lstrcpy(CmdParam[2], (LPTSTR)TEXT("Notepad D:\\С++\\SP_Lab_6Sol\\SP_Lab_6\\SP_LB_2-1.cpp"));

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB6));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB6);
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
            case IDM_PROC_NOTEPAD:
            {
                PROCESS_INFORMATION pi;
                STARTUPINFO si;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);

                /*CreateProcess(L"c:\\Windows\\notepad.exe", NULL, NULL,
                    NULL, false, 0, NULL, NULL, &si, &pi);*/

                CreateProcess(ProcImage[1], NULL, NULL,
                    NULL, false, 0, NULL, NULL, &si, &pi);
                ProcHandle[1] = pi.hProcess;
                ProcId[1] = pi.dwProcessId;
                ThreadHandle[1] = pi.hThread;
                ThreadId[1] = pi.dwThreadId;
                //CloseHandle(pi.hProcess);
                //CloseHandle(pi.hThread);

            } break;
           
                case IDM_PROC_TEXT:
                {
                    
                    
                    if (MessageBox(NULL, TEXT("Желаете выбрать файл?"), TEXT("Сообщение"), MB_YESNO) == IDYES) {
                    
                        OPENFILENAME ofn;
                        ZeroMemory(&ofn, sizeof(OPENFILENAME));
                        
                        HANDLE hFile;

                        ofn.lStructSize = sizeof(OPENFILENAME);
                        ofn.hwndOwner = hWnd;
                        ofn.lpstrFile = lpszFileSpec;
                        ofn.lpstrFile[0] = '\0';
                        ofn.nMaxFile = sizeof(lpszFileSpec);
                        ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.txt\0");
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFileTitle = NULL;
                        ofn.nMaxFileTitle = 0;
                        ofn.lpstrInitialDir = NULL;
                        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
           
                        if (GetOpenFileName(&ofn)) {

                            LPCWSTR cmd = L"Notepad ";
                            lstrcpy(CmdParam[2], (LPTSTR)TEXT(""));
                            lstrcat(CmdParam[2], cmd);
                            lstrcat(CmdParam[2], ofn.lpstrFile);
                            
                        }
                    

                    }

                        PROCESS_INFORMATION pi;
                        STARTUPINFO si;

                        ZeroMemory(&si, sizeof(si));
                        si.cb = sizeof(si);

                        CreateProcess(ProcImage[2], CmdParam[2], NULL,
                            NULL, false, 0, NULL, NULL, &si, &pi);
                        ProcHandle[2] = pi.hProcess;
                        ProcId[2] = pi.dwProcessId;
                        ThreadHandle[2] = pi.hThread;
                        ThreadId[2] = pi.dwThreadId;
                
                } break;
                case IDM_PROC_TEST:
                {

                    DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, dialogTestProcess, 0);

                } break;   
            case IDM_PROC_CALC:
            {
                PROCESS_INFORMATION pi;
                STARTUPINFO si;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);

                CreateProcess(ProcImage[3], NULL, NULL,
                    NULL, false, 0, NULL, NULL, &si, &pi);
                ProcHandle[3] = pi.hProcess;
                ProcId[3] = pi.dwProcessId;
                ThreadHandle[3] = pi.hThread;
                ThreadId[3] = pi.dwThreadId;


            } break;
            case IDM_PROC_CLOSE:{

                HANDLE hFile;

                
                    hFile = CreateFile(lpszFileSpec, GENERIC_READ, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

                    ReadFile(hFile, text, MaxLength, &nCharRead, NULL);
                    InvalidateRect(hWnd, &rt, true);

                    CloseHandle(hFile);
                
                TerminateProcess(ProcHandle[1], 9);
                TerminateProcess(ProcHandle[2], 9);

            } break;
            case IDM_PROC_ICUR: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_PROC), hWnd, dialogProcess, 0);
            } break;
            case IDM_PROC_INOTEPAD: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_PROC), hWnd, dialogProcess, 1);
            } break;
            case IDM_PROC_ITEXT: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_PROC), hWnd, dialogProcess, 2);
            } break;
            case IDM_PROC_ICALC: {
                DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG_PROC), hWnd, dialogProcess, 3);
            } break;

            
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
            hdc = BeginPaint(hWnd, &ps);

            SetRect(&rt, 100, 100, 1500, 800);
            
            DrawTextA(hdc, text, nCharRead, &rt, DT_LEFT);

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

INT_PTR CALLBACK dialogProcess(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{


    switch (message)
    {
    case WM_INITDIALOG: {

        if (lParam == 0) {
            static TCHAR Buff2[260];
            GetModuleFileName(NULL, Buff2, 260);
            ProcImage[0] = Buff2;
            ProcHandle[0] = GetCurrentProcess();
        }
        

        SetDlgItemText(hDlg, IDC_EDIT1, ProcImage[lParam]);
        SetDlgItemText(hDlg, IDC_EDIT2, CmdParam[lParam]);
        SetDlgItemInt(hDlg, IDC_EDIT3, (int)ProcHandle[lParam], true);
        SetDlgItemInt(hDlg, IDC_EDIT4, ProcId[lParam], false);
        SetDlgItemInt(hDlg, IDC_EDIT5, (int)ThreadHandle[lParam], true);
        SetDlgItemInt(hDlg, IDC_EDIT6, ThreadId[lParam], false);

        DWORD procs;
        GetExitCodeProcess(ProcHandle[lParam], &procs);
        if (procs == STILL_ACTIVE)
            SetDlgItemText(hDlg, IDC_EDIT7, TEXT("Состояние - Активен"));
        else
            SetDlgItemInt(hDlg, IDC_EDIT7, procs, true);

        DWORD thread;
        GetExitCodeThread(ThreadHandle[lParam], &thread);
        if (thread == STILL_ACTIVE)
            SetDlgItemText(hDlg, IDC_EDIT8, TEXT("Состояние - Активен"));
        else
            SetDlgItemInt(hDlg, IDC_EDIT8, thread, true);

        DWORD PriorityClass = GetPriorityClass(ProcHandle[lParam]);
        TCHAR temp[100];
        _itow_s(PriorityClass, temp, 16);//перевод из hex
        SetDlgItemText(hDlg, IDC_EDIT9, temp);

        FILETIME KernelTime, UserTime, CreateTime, EndTime;//переменные для определения конечного времени работы процесса
        UINT qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed, qwNotWorkTime;//переменные для хранения времени работы процесса
        GetProcessTimes(ProcHandle[lParam], &CreateTime, &EndTime, &KernelTime, &UserTime);

        qwTotalTimeElapsed = (&EndTime - &CreateTime);
        SetDlgItemInt(hDlg, IDC_EDIT10, qwTotalTimeElapsed, false);
        qwUserTimeElapsed = (&UserTime - &ftUserTimeStart[lParam]);
        SetDlgItemInt(hDlg, IDC_EDIT11, qwUserTimeElapsed, false);
        qwKernelTimeElapsed = (&KernelTime - &ftKernelTimeStart[lParam]);
        SetDlgItemInt(hDlg, IDC_EDIT12, qwKernelTimeElapsed, false);
        qwNotWorkTime = qwTotalTimeElapsed - (qwUserTimeElapsed + qwKernelTimeElapsed);
        SetDlgItemInt(hDlg, IDC_EDIT13, qwNotWorkTime, false);
    }
        

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK dialogTestProcess(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_INITDIALOG: {
        SetDlgItemInt(hDlg, IDC_SIZE_X, 250, false);
        SetDlgItemInt(hDlg, IDC_SIZE_Y, 250, false);
        SetDlgItemInt(hDlg, IDC_X, 400, false);
        SetDlgItemInt(hDlg, IDC_Y, 400, false);
        CheckRadioButton(hDlg, ID_RBMAX, ID_RBNORMAL, ID_RBNORMAL);

        return (INT_PTR)TRUE;
    } 
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK: {

            PROCESS_INFORMATION pi;
            STARTUPINFO si;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESHOWWINDOW;
            si.wShowWindow = showWnd;
            si.dwXSize = GetDlgItemInt(hDlg, IDC_SIZE_X, NULL, false);
            si.dwYSize = GetDlgItemInt(hDlg, IDC_SIZE_Y, NULL, false);
            si.dwX = GetDlgItemInt(hDlg, IDC_X, NULL, false);
            si.dwY = GetDlgItemInt(hDlg, IDC_Y, NULL, false);


            CreateProcess(L"D:\\С++\\SP_Lab_6Sol\\Debug\\TestProc.exe", NULL, NULL,
                NULL, false, 0, NULL, NULL, &si, &pi);


            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        }
        case ID_RBMAX:
        {
            showWnd = SW_MAXIMIZE;
            break;
        }
        case ID_RBMIN:
        {
            showWnd = SW_MINIMIZE;
            break;
        }
        case ID_RBNORMAL:
        {
            showWnd = SW_NORMAL;
            break;
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

