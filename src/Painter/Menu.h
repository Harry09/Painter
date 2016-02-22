#pragma once

#include "Prerequisites.h"

enum ModeDrawing
{
	ID_DRAWPIXEL = 0,
	ID_DRAWLINE = 1,
	ID_DRAWQUAD = 2
};

class CMenu
{
private:
	static HWND s_hWnd;

	static char m_sMode[3][11];
	static char m_iMode; // @ ModeDrawing

private:
	static DWORD WINAPI Window(LPVOID);
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CMenu();
	~CMenu();

	void ShowWindow();
	ModeDrawing GetModeDrawing() { return (ModeDrawing)m_iMode; }
};