#pragma once

#include "Prerequisites.h"

class CNewImage
{
private:
	HWND m_hWnd;

	static glm::ivec2 m_iImageSize;
	static cvec3 m_byBgColor;

	static bool m_bAccept;

private:
	static int CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CNewImage();
	~CNewImage();

	glm::ivec2 GetSize() { return m_iImageSize; }
	cvec3 GetBgColor() { return m_byBgColor; }
	bool Accepted() { return m_bAccept; }
};

