#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <winuser.h>
#include "resource.h"
//-- Prototypes -------------------
LRESULT CALLBACK Pr3_WndProc(HWND, UINT, WPARAM, LPARAM);

//-- Global Variables ------------
LPCTSTR g_lpszApplicationTitle = TEXT("Главное окно приложения. Программист <Соколовский Вадим>");
LPCTSTR g_lpszClassName = TEXT("sp_pz3_class");
LPCTSTR g_lpszDestroyMessage = TEXT("Поступило сообщение WM_DESTROY");
HINSTANCE g_hInst;
HMENU hMenu1;

// Стартовая функция
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{

	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;

	HACCEL hAccel;
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr3_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = CreateSolidBrush(RGB(0, 127, 127));
	wc.lpszMenuName = NULL; // MAKEINTRESOURCE(IDR_MENU1);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	
	hMenu1 = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	hWnd = CreateWindowEx(NULL, g_lpszClassName	,
		TEXT("Simple Application with Message handling"),
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX,
		0,
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		hMenu1, // NULL;
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{

		if (!TranslateAccelerator(hWnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		
	}
	return msg.wParam;
}
// Оконная процедура
LRESULT CALLBACK Pr3_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	int wmId, wmEvent;
	switch (msg)
	{

	case WM_CREATE: {

		MessageBox(NULL, TEXT("выполняется обработчик WM_CREATE"),
			TEXT("WM_CREATE"), MB_OK);
		HWND hBtExit;
		hBtExit = CreateWindowEx(0, TEXT("Button"), TEXT("Выход"), WS_CHILD | WS_VISIBLE,
			50, 50, 100, 30, hWnd, (HMENU)(IDCANCEL), g_hInst, NULL);
		if (!hBtExit) return -1;									
		HMENU hmFile = GetSubMenu(GetMenu(hWnd), 0);				// Добавление подменю программным путём
		AppendMenu(hmFile, MF_STRING | MF_ENABLED, IDM_FILE_CLOSE, L"Закрыть документ");


	}return 0;

		case WM_MENUSELECT:
		{HDC hdc1;
		LPCTSTR lpszMsgSpace = TEXT("ШШШШШШШШШШШШШШШШШШШШШШШШШШ");
		TCHAR Buf[300];
		HINSTANCE hInst;
		hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		int size;
		size = LoadString(hInst, LOWORD(wParam), Buf, 300);
		hdc1 = GetDC(hWnd);
		RECT rc;
		GetClientRect(hWnd, &rc);
		TextOut(hdc1, rc.left + 10, rc.bottom - 30,
			lpszMsgSpace, lstrlen(lpszMsgSpace));
		TextOut(hdc1, rc.left + 10, rc.bottom -
			30, Buf, lstrlen(Buf));
		ReleaseDC(hWnd, hdc1);
		break;
		}


	case WM_COMMAND:

		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId) {
		case IDCANCEL:
		{
			MessageBox(hWnd, TEXT("Выход"), TEXT("Из WM_COMMAND"), MB_OK);
			DestroyWindow(hWnd);
		}break;


		case IDM_FILE_NEW: {
			MessageBox(hWnd, TEXT("Создать"), TEXT("Из WM_COMMAND"), MB_OK);
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fState = MFS_ENABLED;
			mii.fMask = MIIM_STATE;
			SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 0, true, &mii);
			
		}break;


		case IDM_FILE_OPEN: {
			MessageBox(hWnd, TEXT("Открыть"), TEXT("Из WM_COMMAND"), MB_OK);
		}break;

		case IDM_FILE_CLOSE: {
			MessageBox(hWnd, TEXT("Закрыть"), TEXT("Из WM_COMMAND"), MB_OK);
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fState = MFS_GRAYED;
			mii.fMask = MIIM_STATE;
			SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 0, true, &mii);
			SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 2, true, &mii);
		}break;


		case IDM_EDIT_COPY: {
			MessageBox(hWnd, TEXT("Копировать"), TEXT("Из WM_COMMAND"), MB_OK);
		}break;


		case IDM_EDIT_INSERT: {
			MessageBox(hWnd, TEXT("Вставить"), TEXT("Из WM_COMMAND"), MB_OK);
		}break;



		case IDM_EDIT_SEL: {
			MessageBox(hWnd, TEXT("Выделить"), TEXT("Из WM_COMMAND"), MB_OK);
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fState = MFS_ENABLED;
			mii.fMask = MIIM_STATE;
			SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 2, true, &mii);
		}break;

		


		//case IDM_HELP_ABOUT: {
		//	MessageBox(hWnd, TEXT("О программе"), TEXT("Из WM_COMMAND"), MB_OK);
		//}break;


		default: {
			TCHAR message[200];
			wsprintf(message, TEXT("Команда - итендификатор %d не предусмотрена."), wmId);
			MessageBox(hWnd, message, TEXT("Из WM_COMMAND"), MB_OK);
			return(DefWindowProc(hWnd, msg, wParam, lParam));
		}break;


		}break;





	case WM_LBUTTONDOWN: {

		LPCTSTR g_lpszlbutton = TEXT("Обработка сообщения WB_LBUTTONDOWN, которое посылается в окно"
			" при щелчке левой кнопкой мыши");
		HDC hdc;
		hdc = GetDC(hWnd);
		RECT rect;
		rect.left = (long)LOWORD(lParam);
		rect.bottom = CW_USEDEFAULT;
		rect.right = CW_USEDEFAULT;
		rect.top = (long)HIWORD(lParam);
		DrawText(hdc, g_lpszlbutton, lstrlen(g_lpszlbutton), &rect, DT_LEFT);
		ReleaseDC(hWnd, hdc);
		break;

	}

	case WM_RBUTTONDOWN: {

		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		HMENU hMenu = CreatePopupMenu();
		ClientToScreen(hWnd, &pt);

		UINT CopyState = GetMenuState(GetSubMenu(GetMenu(hWnd), 1), 2, MF_BYPOSITION);
		UINT SelState = GetMenuState(GetSubMenu(GetMenu(hWnd), 1), 0, MF_BYPOSITION);

		AppendMenu(hMenu, SelState, IDM_EDIT_SEL, L"Выделить");
		AppendMenu(hMenu, CopyState, IDM_EDIT_COPY, L"Копировать");
		
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);

		break;
	}

	case WM_PAINT: { // Вывод при обновлении окна
		LPCTSTR g_lpszPaint = TEXT("Обработка сообщения WM_PAINT. Это сообщение окно получает "
			"после того как оно было закрыто другим окном и затем открыто");
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps); // Получение контекста для
		// обновления окна
		TextOut(hDC, 20, 100, g_lpszPaint, lstrlen(g_lpszPaint)); // Вывод в контекст
		EndPaint(hWnd, &ps); // Завершение обновления окна
		break;
	}

	case WM_DESTROY:// Завершение работы приложения
		MessageBox(NULL, g_lpszDestroyMessage,
			TEXT("WM_DESTROY"), MB_OK | MB_ICONERROR);
		PostQuitMessage(5); // Посылка WM_QUIT приложению
		break;
	default: // Вызов "Обработчика по умолчанию"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// Для ветвей с "break"
}
