#pragma once

#include "Prerequisites.h"

class CMenu
{
private:
	static HWND s_hWnd;

private:
	static DWORD WINAPI Window(LPVOID);
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CMenu();
	~CMenu();

	void ShowWindow();
};