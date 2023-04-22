// SP_Pr4.cpp : Определяет точку входа для приложения.
//



#include "framework.h"
#include "SP_Pr4.h"
#include <commdlg.h>
#include <string>
#include <locale.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:

static TCHAR EditBuff[20];
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
static TCHAR gl_array_str[400] = TEXT("");
static TCHAR gpszTextBuff[10][400];
HWND hWndList, hWndEdit, hWnd;
int cnt;
TCHAR lpszFileSpec[MAX_PATH] = TEXT("Тест Шага 2");
std::wstring buffer;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ListBoxEdit(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DlgViewText(HWND, UINT, WPARAM, LPARAM);

BOOL Pr4_WndProc_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void Pr4_WndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Pr4_WndProc_OnPaint(HWND hwnd);
void Pr4_WndProc_OnDestroy(HWND hwnd);
void arrToString();


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
	LoadStringW(hInstance, IDC_SPPR4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPPR4));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPPR4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPPR4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		HANDLE_MSG(hWnd, WM_DESTROY, Pr4_WndProc_OnDestroy);
		HANDLE_MSG(hWnd, WM_CREATE, Pr4_WndProc_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, Pr4_WndProc_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, Pr4_WndProc_OnPaint);
	
	
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
// Обработчик сообщений для окна "ListBoxEdit".
INT_PTR CALLBACK ListBoxEdit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCTSTR str = TEXT("Введите строку");

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SYSTEMTIME lst;
		GetLocalTime(&lst);
		TCHAR time[200];
		GetDlgItemText(hDlg, IDC_TIME, time, 200);
		TCHAR time1[200];
		wsprintf(time1, TEXT(" %d час, %d минуты, %d секунд"), lst.wHour, lst.wMinute, lst.wSecond);
		lstrcat(time, time1);
		SetDlgItemText(hDlg, IDC_TIME, time);

		SetDlgItemText(hDlg, IDC_EDIT1, gl_array_str);

		cnt = 0;	//
		ZeroMemory(EditBuff, sizeof(EditBuff));

		hWndEdit = GetDlgItem(hDlg, IDC_EDIT1);
		hWndList = GetDlgItem(hDlg, IDC_LIST1);

		SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM)str);

				}
	return (INT_PTR)TRUE;
	
	static TCHAR text[200];
	case WM_COMMAND:
		int id = LOWORD(wParam);
		switch (id) {
		case IDC_BUTTON1: {
			HWND gListWnd = GetDlgItem(hDlg, IDC_LIST1);
			SendMessage(gListWnd, LB_ADDSTRING, NULL, (LPARAM)text);
			
		}break;


		case IDC_STRREAD: {
			GetDlgItemText(hDlg, IDC_EDIT1, text, 200);
		}break;


		case IDCANCEL:
		{EndDialog(hDlg, LOWORD(wParam)); break; }


		case IDOK:
		{	
			/*GetDlgItemText(hDlg, IDC_LIST1, gl_array_str, 400);
			lstrcat(gl_array_str, text)*/;
			//HDC hdc = GetDC();
			cnt = SendMessage(hWndList, LB_GETCOUNT, 0, 0L);	//

			for (int i = 0; i < cnt; i++)						//
			{													//
				SendMessage(hWndList, LB_GETTEXT, i,			//
					(LPARAM)gpszTextBuff[i]);
			}
				arrToString();

				HDC hdc = GetDC(hWnd);

				RECT rect;
				UpdateWindow(hWnd);
				rect.left = 100;
				rect.bottom = 800;
				rect.right = 800;
				rect.top = 100;

				DrawText(hdc, gl_array_str, lstrlen(gl_array_str), &rect, DT_LEFT);
			
				
			EndDialog(hDlg, LOWORD(wParam)); break; 
		}
		return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL Pr4_WndProc_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
	return 1;

}


void Pr4_WndProc_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify) {
	HDC hdc;
	// Разобрать выбор в меню:
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_ABOUT1:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ListBoxEdit);
		break;
	case IDM_DLGVIEW:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGVIEW), hWnd, DlgViewText);
		break;
	case IDM_FILE_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDM_VIEW_TEXT:
		hdc = GetDC(hWnd);
		//TextOut(hdc, 100, 100, gl_array_str, 400);
		
		ReleaseDC(hWnd, hdc);
		MessageBox(hWnd, TEXT("Выбрана команда IDM_VIEW_TEXT"),
			TEXT("Из WM_COMMAND"), MB_OK);
		break;
	case IDM_VIEW_CTL:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ListBoxEdit);
		break;
	}

}
void Pr4_WndProc_OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Добавьте сюда любой код прорисовки, использующий HDC...

	

	EndPaint(hWnd, &ps);
}


void Pr4_WndProc_OnDestroy(HWND hwnd) {
	PostQuitMessage(0);
}

#define MAX_BYTES 10300

INT_PTR CALLBACK DlgViewText(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	LPCTSTR lpszStep1 = TEXT("Шаг 1. Начало работы");
	LPCTSTR lpszStep2 = TEXT("Шаг 2. Выберите файл");
	TCHAR Textbuf[MAX_BYTES];
	switch (message)
	{

	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), lpszStep1);
		if (GetDlgCtrlID((HWND)IDC_BTOPEN) != IDC_BTOPEN)//фокус ввода на кнопку загрузить
		{
			SetFocus(GetDlgItem(hDlg, IDC_BTOPEN));
			return FALSE;
		}
		return TRUE;

	case WM_COMMAND:
	{

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDOK: {
			EndDialog(hDlg, LOWORD(wParam));
			
		}break;
			
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
		}break;

		case IDC_BTVIEW:
		{
			
		}break;

		case IDC_BTOPEN:
		{
			setlocale(LC_CTYPE, "utf8");

			SetDlgItemText(hDlg, IDC_BTOPEN, lpszStep2);
			std::wstring filename(MAX_PATH, L'\0');
			memset(&Textbuf[0], 0, sizeof(Textbuf));
			HANDLE hFile;
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), lpszFileSpec);
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = lpszFileSpec;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(lpszFileSpec);
			ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.txt\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;// (LPTSTR)TEXT("Выбор отображаемого файла");
			ofn.nMaxFileTitle = 0;// lstrlen((LPTSTR)TEXT("Выбор отображаемого файла"));
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			BOOL fRetVal = GetOpenFileName(&ofn);
			if (fRetVal == FALSE)return 0;

			hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
			if (hFile == INVALID_HANDLE_VALUE) return (-1);

			DWORD dwNumbOfBytes;
			ReadFile(hFile, Textbuf, MAX_BYTES, &dwNumbOfBytes, NULL);

			//SendMessage(GetDlgItem(hDlg, IDC_BT_LOAD), WM_SETTEXT, NULL, (LPARAM)Buffer);
			SetDlgItemText(hDlg, IDC_EDIT1, LPCTSTR(Textbuf));

			if (hFile) { CloseHandle(hFile); }
			return (INT_PTR)FALSE;
		}
		}
		return (INT_PTR)FALSE;
	} break;
	}
	return (INT_PTR)FALSE;
}

void arrToString() {
	for (int i = 0; i < cnt; i++) {
		lstrcat(gl_array_str, gpszTextBuff[i]);
		lstrcat(gl_array_str, TEXT(" "));
	}
}
