// lb_2_5.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lb_2_5.h"
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commdlg.h>
#include <stdio.h>

#define MAX_LOADSTRING 100
#define FILENAME TEXT("TEMP.DAT")

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL OpenFile_Dialog(HWND);
void CreateIndex(char*, char*);
void OpenInNotepad();
HANDLE hFile;
HANDLE hFileMap;
LPVOID lpvFile;
DWORD dwFileSize;

LPTSTR ProcImage = TEXT("c:\\WINDOWS\\Notepad.exe");
static TCHAR CmdParam[260] = TEXT("Notepad.exe");

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LB_2_5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LB_2_5));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LB_2_5));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LB_2_5);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_OPEN:
		{
			if (OpenFile_Dialog(hWnd) && hFile != NULL)
			{
				MessageBox(NULL, TEXT("File opened!"), TEXT(""), MB_OK);

				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_OPEN, MF_GRAYED | MF_BYCOMMAND);
				EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_ENABLED | MF_BYCOMMAND);
			}
			else
			{
				MessageBox(NULL, TEXT("File can't be opened!"), TEXT("Error"), MB_OK);
			}
			break;
		}
		case ID_FILE_CREATE_MAP:
		{
			if (hFile != NULL)
			{
				dwFileSize = GetFileSize(hFile, NULL);

				hFileMap = CreateFileMapping(
					hFile,  // Дескриптор проецирумого объекта "Файл" 
					NULL,   // 
					PAGE_READWRITE, // Атрибуты защиты страниц 
					0,  // LPSECURITY_ATTRIBUTES
					dwFileSize + 10000,   //Младшие 32 разряда
					NULL  // и старшие 32 разряда размера файла.
					);
				
				HMENU hMenu = GetMenu(hWnd);
				
				if (hFileMap == NULL)
				{	// Открыть объект "проецируемый файл" не удалось
					MessageBox(NULL, TEXT("File map could not be opened."), TEXT("Error!"), MB_OK);
					CloseHandle(hFile);
					
					EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_GRAYED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_OPEN, MF_ENABLED | MF_BYCOMMAND);
				}
				else
				{
					MessageBox(NULL, TEXT("File map created!"), TEXT(""), MB_OK);
					EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_GRAYED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_ENABLED | MF_BYCOMMAND);
				}
			}
			else
			{
				MessageBox(NULL,TEXT("File doesn't exist"),TEXT("Error!"), MB_OK);
				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_GRAYED | MF_BYCOMMAND);
				EnableMenuItem(hMenu, ID_FILE_OPEN, MF_ENABLED | MF_BYCOMMAND);
			}
			break;
		}
		case ID_FILE_CREATE_VIEW:
		{
			if (hFileMap != NULL)
			{
				lpvFile = MapViewOfFile(
					hFileMap, // Дескриптор объекта "Проецируемый файл" 
					FILE_MAP_WRITE, // Режим доступа
					0, // Старшие 32 разряда смещения от начала файла, 
					0, // младшие 32 разряда смещения от начала файла
					0  // и количество отображаемых байт. 0 - весь файл.
					);
				
				HMENU hMenu = GetMenu(hWnd);
				
				if (lpvFile == NULL)
				{// Спроецировать оконное представление файла не удалось
					MessageBox(NULL, TEXT("Could   not map view of tile."),TEXT("ERror!"), MB_OK);
					CloseHandle(hFileMap);// Перед выходом закрываем открытые объекты
					CloseHandle(hFile);

					EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_GRAYED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_ENABLED | MF_BYCOMMAND);
				}
				else
				{
					MessageBox(NULL, TEXT("View created!"), TEXT(""), MB_OK);

					EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_GRAYED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_CREATE_INDEX, MF_ENABLED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_DESTROY_VIEW, MF_ENABLED | MF_BYCOMMAND);
				}
			}
			else
			{
				MessageBox(NULL, TEXT("File map doesn't exist"), TEXT("Error!"), MB_OK);
				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_GRAYED | MF_BYCOMMAND);
				EnableMenuItem(hMenu, ID_FILE_CREATE_MAP, MF_ENABLED | MF_BYCOMMAND);
			}
			break;
		}
		case ID_FILE_CREATE_INDEX:
		{
			if (lpvFile != NULL)
			{
				char *text = (char*)lpvFile;
				char index[10000] = "";
				CreateIndex(text, index);
				int size = strlen(index);

				for (int i = 0; i < size; i++)
				{
					text[dwFileSize + i] = index[i];
				}
				
				dwFileSize += size;

				MessageBox(NULL, TEXT("Index created!"), TEXT(""), MB_OK);
				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_CREATE_INDEX, MF_GRAYED | MF_BYCOMMAND);
			}
			else
			{
				MessageBox(NULL, TEXT("View doesn't exist"), TEXT("Error!"), MB_OK);
				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_CREATE_INDEX, MF_GRAYED | MF_BYCOMMAND);
				EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_ENABLED | MF_BYCOMMAND);
			}
			break;
		}
		case ID_FILE_DESTROY_VIEW:
		{
			if (lpvFile != NULL)
			{
				if (UnmapViewOfFile(lpvFile))
				{
					MessageBox(NULL, TEXT("View close"), TEXT(""), MB_OK);
					HMENU hMenu = GetMenu(hWnd);
					EnableMenuItem(hMenu, ID_FILE_DESTROY_VIEW, MF_GRAYED | MF_BYCOMMAND);
					EnableMenuItem(hMenu, ID_FILE_CLOSE, MF_ENABLED | MF_BYCOMMAND);
				}
				else
				{
					MessageBox(NULL, TEXT("View can't be destroyed"), TEXT("Error"), MB_OK);
				}
			}
			else
			{
				MessageBox(NULL, TEXT("View doesn't exist"), TEXT("Error!"), MB_OK);
				HMENU hMenu = GetMenu(hWnd);
				EnableMenuItem(hMenu, ID_FILE_CREATE_INDEX, MF_GRAYED | MF_BYCOMMAND);
				EnableMenuItem(hMenu, ID_FILE_CREATE_VIEW, MF_ENABLED | MF_BYCOMMAND);
			}
			break;
		}
		case ID_FILE_CLOSE:
		{
			if (hFileMap != NULL)
			{
				CloseHandle(hFileMap);
			}

			SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
			SetEndOfFile(hFile);
			if (hFile != NULL)
			{
				CloseHandle(hFile);
			}

			MessageBox(NULL, TEXT("Handle close"), TEXT(""), MB_OK);
			
			HMENU hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, ID_FILE_CLOSE, MF_GRAYED | MF_BYCOMMAND);
			EnableMenuItem(hMenu, ID_FILE_OPEN, MF_ENABLED | MF_BYCOMMAND);

			OpenInNotepad();
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	{
		/*if (hFile != NULL)
		{
			CloseHandle(hFile);
		}
		if (hFileMap != NULL)
		{
			CloseHandle(hFileMap);
		}
		if (lpvFile != NULL)
		{
			UnmapViewOfFile(lpvFile);
		}*/
		PostQuitMessage(0);
		break;
	}
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

BOOL OpenFile_Dialog(HWND hWnd)
{
	TCHAR lpszFileSpec[260];
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd; 
	ofn.lpstrFile = lpszFileSpec;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(lpszFileSpec);

	ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	BOOL fRet = GetOpenFileName(&ofn);
	if (fRet == FALSE) return FALSE;

	if (!CopyFile(ofn.lpstrFile, FILENAME, FALSE))
	{
		MessageBox(NULL, TEXT("Can't create file copy"), TEXT("Error!"), MB_OK);
		return FALSE;
	}

	hFile = CreateFile(
		FILENAME,  // Указатель на строку с имененм файла
		GENERIC_WRITE | GENERIC_READ, // Требуемый режим доступа
		0,   //  Режим разделения,0 - безраздельный доступ к файлу.       
		NULL,// LPSECURITY_ATTRIBUTES=0 - объект не наследуемый.
		OPEN_EXISTING,//Если файл не существует, то возвратить ошибку.
		FILE_ATTRIBUTE_NORMAL,//Оставить атрибуты как есть 
		NULL //Не давать имя объекту ядра "Файл"
		);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

void CreateIndex(char* text, char* index)
{
	int size = strlen(text);
	int count = 1;
	strcat_s(index, 1000, " 0 0 ");

	for (int i = 0; i < size; i++)
	{
		char temp = text[i];

		if (temp == ' ')
		{
			i++;
			char temp1[10] = "";
			sprintf_s(temp1, "%d %d ", count, i);
			strcat_s(index, 10000, temp1);
			count++;
		}
	}
}

void OpenInNotepad()
{
	STARTUPINFO sti;
	ZeroMemory(&sti, sizeof(STARTUPINFO));
	sti.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	BOOL f;

	TCHAR temp[260] = TEXT("notepad.exe ");
	lstrcat(temp, FILENAME);
	lstrcpyn(CmdParam, temp, 260);

	f = CreateProcess(ProcImage, CmdParam,NULL,	NULL, FALSE,0,NULL,	NULL,&sti,&pi);
	
	if (f)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox(NULL, TEXT("File can't be opened in notepad"), TEXT("Error!"), MB_OK);
	}
}