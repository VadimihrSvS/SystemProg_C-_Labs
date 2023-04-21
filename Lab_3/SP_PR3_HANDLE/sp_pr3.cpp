#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <winuser.h>
#include "resource.h"
//-- Prototypes -------------------
LRESULT CALLBACK Pr3_WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL Pr3_WndProc_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void Pr3_WndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Pr3_WndProc_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void Pr3_WndProc_OnPaint(HWND hwnd);
void Pr3_WndProc_OnDestroy(HWND hwnd);
void Pr3_WndProc_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
void Pr3_WndProc_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
//-- Global Variables ------------
LPCTSTR g_lpszApplicationTitle = TEXT("������� ���� ����������. ����������� <����������� �����>");
LPCTSTR g_lpszClassName = TEXT("sp_pz3_class");
LPCTSTR g_lpszDestroyMessage = TEXT("��������� ��������� WM_DESTROY");
HINSTANCE g_hInst;
HMENU hMenu1;

// ��������� �������
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
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}


	hMenu1 = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	hWnd = CreateWindowEx(NULL, g_lpszClassName,
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
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
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
// ������� ���������
LRESULT CALLBACK Pr3_WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	int wmId, wmEvent;
	switch (msg)
	{

		HANDLE_MSG(hWnd, WM_CREATE, Pr3_WndProc_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, Pr3_WndProc_OnCommand);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, Pr3_WndProc_OnLButtonDown);
		HANDLE_MSG(hWnd, WM_PAINT, Pr3_WndProc_OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, Pr3_WndProc_OnDestroy);
		HANDLE_MSG(hWnd, WM_MENUSELECT, Pr3_WndProc_OnMenuSelect);
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, Pr3_WndProc_OnRButtonDown);

	default: // ����� "����������� �� ���������"
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return FALSE;// ��� ������ � "break"
}

BOOL Pr3_WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) {

	MessageBox(NULL, TEXT("����������� ���������� WM_CREATE"),
		TEXT("WM_CREATE"), MB_OK);
	HWND hBtExit;
	hBtExit = CreateWindowEx(0, TEXT("Button"), TEXT("�����"), WS_CHILD | WS_VISIBLE,
		50, 50, 100, 30, hWnd, (HMENU)(IDCANCEL), g_hInst, NULL);
	if (!hBtExit) return false;
	HMENU hmFile = GetSubMenu(GetMenu(hWnd), 0);				// ���������� ������� ����������� ����
	AppendMenu(hmFile, MF_STRING | MF_ENABLED, IDM_FILE_CLOSE, L"������� ��������");
	return true;
}

void Pr3_WndProc_OnCommand(HWND hWnd, int wmId, HWND hwndCtl, UINT wmEvent) {




	switch (wmId) {
	case IDCANCEL:
	{
		MessageBox(hWnd, TEXT("�����"), TEXT("�� WM_COMMAND"), MB_OK);
		DestroyWindow(hWnd);
	}break;


	case IDM_FILE_NEW: {
		MessageBox(hWnd, TEXT("�������"), TEXT("�� WM_COMMAND"), MB_OK);
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fState = MFS_ENABLED;
		mii.fMask = MIIM_STATE;
		SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 0, true, &mii);

	}break;


	case IDM_FILE_OPEN: {
		MessageBox(hWnd, TEXT("�������"), TEXT("�� WM_COMMAND"), MB_OK);
	}break;

	case IDM_FILE_CLOSE: {
		MessageBox(hWnd, TEXT("�������"), TEXT("�� WM_COMMAND"), MB_OK);
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fState = MFS_GRAYED;
		mii.fMask = MIIM_STATE;
		SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 0, true, &mii);
		SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 2, true, &mii);
	}break;


	case IDM_EDIT_COPY: {
		MessageBox(hWnd, TEXT("����������"), TEXT("�� WM_COMMAND"), MB_OK);
	}break;


	case IDM_EDIT_INSERT: {
		MessageBox(hWnd, TEXT("��������"), TEXT("�� WM_COMMAND"), MB_OK);
	}break;



	case IDM_EDIT_SEL: {
		MessageBox(hWnd, TEXT("��������"), TEXT("�� WM_COMMAND"), MB_OK);
		MENUITEMINFO mii;
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fState = MFS_ENABLED;
		mii.fMask = MIIM_STATE;
		SetMenuItemInfo(GetSubMenu(GetMenu(hWnd), 1), 2, true, &mii);
	}break;




		//case IDM_HELP_ABOUT: {
		//	MessageBox(hWnd, TEXT("� ���������"), TEXT("�� WM_COMMAND"), MB_OK);
		//}break;


	default: {
		TCHAR message[200];
		wsprintf(message, TEXT("������� - ������������� %d �� �������������."), wmId);
		MessageBox(hWnd, message, TEXT("�� WM_COMMAND"), MB_OK);
		//return(DefWindowProc(hWnd, msg, wParam, lParam));
		FORWARD_WM_COMMAND(hWnd, wmId, hwndCtl, wmEvent, DefWindowProc);
	}break;



	}




}

void Pr3_WndProc_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	LPCTSTR g_lpszlbutton = TEXT("��������� ��������� WB_LBUTTONDOWN, ������� ���������� � ����"
		" ��� ������ ����� ������� ����");
	HDC hdc;
	hdc = GetDC(hWnd);
	RECT rect;
	rect.left = x;
	rect.bottom = CW_USEDEFAULT;
	rect.right = CW_USEDEFAULT;
	rect.top = y;
	DrawText(hdc, g_lpszlbutton, lstrlen(g_lpszlbutton), &rect, DT_LEFT);
	ReleaseDC(hWnd, hdc);

}

void Pr3_WndProc_OnPaint(HWND hWnd) {
	LPCTSTR g_lpszPaint = TEXT("��������� ��������� WM_PAINT. ��� ��������� ���� �������� "
		"����� ���� ��� ��� ���� ������� ������ ����� � ����� �������");
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ���
	// ���������� ����
	TextOut(hDC, 20, 100, g_lpszPaint, lstrlen(g_lpszPaint)); // ����� � ��������
	EndPaint(hWnd, &ps); // ���������� ���������� ����
}

void Pr3_WndProc_OnDestroy(HWND hWnd) {
	MessageBox(NULL, g_lpszDestroyMessage,
		TEXT("WM_DESTROY"), MB_OK | MB_ICONERROR);
	PostQuitMessage(5); // ������� WM_QUIT ����������
}

void Pr3_WndProc_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags) {
	HDC hdc1;
	LPCTSTR lpszMsgSpace = TEXT("��������������������������");
	TCHAR Buf[300];
	HINSTANCE hInst;
	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	int size;
	size = LoadString(hInst, LOWORD(hmenu), Buf, 300);
	hdc1 = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);
	TextOut(hdc1, rc.left + 10, rc.bottom - 30,
		lpszMsgSpace, lstrlen(lpszMsgSpace));
	TextOut(hdc1, rc.left + 10, rc.bottom -
		30, Buf, lstrlen(Buf));
	ReleaseDC(hWnd, hdc1);
}

void Pr3_WndProc_OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	POINT pt;
	pt.x = x;
	pt.y = y;
	HMENU hMenu = CreatePopupMenu();
	ClientToScreen(hWnd, &pt);

	UINT CopyState = GetMenuState(GetSubMenu(GetMenu(hWnd), 1), 2, MF_BYPOSITION);
	UINT SelState = GetMenuState(GetSubMenu(GetMenu(hWnd), 1), 0, MF_BYPOSITION);

	AppendMenu(hMenu, SelState, IDM_EDIT_SEL, L"��������");
	AppendMenu(hMenu, CopyState, IDM_EDIT_COPY, L"����������");

	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, x, y, 0, hWnd, NULL);
	DestroyMenu(hMenu);
}

