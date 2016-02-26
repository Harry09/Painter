#pragma once

#include "Prerequisites.h"

class CSettings
{
private:
	HWND m_hWnd;

public:
	glm::ivec2 m_iResolution;

private:
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CSettings();
	~CSettings();

	void ShowWindow();

	void Save(bool getDlgValue = true);
	void Load();


	static CSettings *Get() { return s_pInst; }

private:
	static CSettings *s_pInst;
};