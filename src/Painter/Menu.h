#pragma once

#include "Prerequisites.h"

enum ModeDrawing
{
	ID_DRAWPIXEL = 0,
	ID_DRAWLINE,
	ID_DRAWQUAD,
	ID_DRAWCIRCLE
};

class CMenu
{
private:
	static HWND s_hWnd;

	static char m_sMode[4][12];
	static char m_iMode; // @ ModeDrawing

public:
	static bool m_bGetColor;

private:
	static DWORD WINAPI Window(LPVOID);
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CMenu();
	~CMenu();

	void ShowWindow();
	ModeDrawing GetModeDrawing() { return (ModeDrawing)m_iMode; }
	HWND GetHWnd() { return s_hWnd; }
};