#pragma once

#include "Prerequisites.h"

class CHelp
{
private:
	HWND m_hWnd;

private:
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CHelp();
	~CHelp();
};

