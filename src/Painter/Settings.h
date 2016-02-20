#pragma once

#include "Prerequisites.h"

class CSettings
{
private:
	glm::ivec2 m_iResolution;
	HWND m_hWnd;

public:
	CSettings();
	~CSettings();

	void ShowWindow();

	void Save(bool getDlgValue = true);
	void Load();

	glm::ivec2 GetResolution() { return m_iResolution; }

	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};