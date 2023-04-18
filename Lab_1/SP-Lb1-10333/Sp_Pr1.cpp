#include <Windows.h>
#include <tchar.h>
#include "sp_pr1.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	LPCTSTR lpszHelloWndTitle = MESSAGE_TITLE;
	LPCTSTR	lpszMsgText = TEXT("������ �� Win32 ���������� � ����������� �����������");
	LPCTSTR lpszAuthor = MESSAGE_AUTHOR;
	LPCTSTR lpszDesignTime = TEXT("Design Time = <18/04/2023 14:58>");

	TCHAR _buffer[1024];

	SYSTEMTIME _datetime;
	GetSystemTime(&_datetime);

	_stprintf(_buffer, TEXT("%s\n\n%s\n%s\nRun time = <%i/%i/%i %i:%i>"),
		lpszMsgText,
		lpszAuthor,
		lpszDesignTime,
		_datetime.wDay,
		_datetime.wMonth,
		_datetime.wYear,
		_datetime.wHour + 3,
		_datetime.wMinute
	);

	int iRetValue1, iRetValue2;
	LPCTSTR lpszMesBoxTitle = TEXT("������� MessageBox");
	LPCTSTR lpszResponce;
	do {
		iRetValue1 = MessageBox(NULL, TEXT("The message box contains three push buttons:\n"
			"Abort, Retry and Ignore."),
			lpszMesBoxTitle,
			MB_ABORTRETRYIGNORE | MB_ICONINFORMATION | MB_DEFBUTTON2);
		switch (iRetValue1)
		{
		case IDABORT:
			lpszResponce = TEXT("������ ������ �������� (ABORT)");
			break;
		case IDRETRY:
			lpszResponce = TEXT("������ ������ ��������� (RETRY)");
			break;
		case IDIGNORE:
			lpszResponce = TEXT("������ ������ ���������� (IGNORE)");
			break;
		default: lpszResponce = TEXT("����� ��� �� �������.");
		}
		TCHAR buf[200] = TEXT("");

		lstrcat(buf, lpszResponce);
		lstrcat(buf, TEXT(" ���������� �������� ������������ �������� ? "));
			iRetValue2 = MessageBox(NULL, buf, lpszMesBoxTitle,
				MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND);
	} while (iRetValue2 != IDNO);

	MessageBox(NULL, _buffer, lpszHelloWndTitle, MB_ABORTRETRYIGNORE | MB_ICONINFORMATION);
	return 0;
}