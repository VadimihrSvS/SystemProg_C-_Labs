#include <windows.h>
#include <tchar.h>
#include <winuser.h>
//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
//-- Global Variables ------------
LPCTSTR g_lpszApplicationTitle = TEXT("Главное окно приложения. Программист <Соколовский Вадим>");
LPCTSTR g_lpszClassName = TEXT("sp_pz2_class");
LPCTSTR g_lpszDestroyMessage = TEXT("Поступило сообщение WM_DESTROY, из обработчика которого и выполнен"
" данный ввод. Сообщение поступило в связи с разрушением окна приложений");
HINSTANCE g_hInst;

// Стартовая функция
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
	wc.hbrBackground = CreateSolidBrush(RGB(0, 127, 127));
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindowEx(NULL, g_lpszClassName	,
		TEXT("Simple Application with Message handling"),
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX,
		0,
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
// Оконная процедура
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg,
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
			50, 50, 100, 30, hWnd,(HMENU)(IDCANCEL), g_hInst, NULL);
		if (!hBtExit) return -1;
	}return 0;
	

	case WM_COMMAND: 
		
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId) {
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		}
		break;




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