#pragma once

#include "Prerequisites.h"

class CColorPicker
{
private:
	cvec3 m_byColor;
	bool m_bAccept;

	static COLORREF s_dwAcrCustClr[16];
	static DWORD s_dwRgbCurrent;


public:
	CColorPicker(HWND _hWndParent = 0);
	~CColorPicker();

	bool Accepted() { return m_bAccept; }
	cvec3 GetRGB() { return m_byColor; }
};

