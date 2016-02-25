#include "ColorPicker.h"

COLORREF CColorPicker::s_dwAcrCustClr[16];
DWORD CColorPicker::s_dwRgbCurrent;

CColorPicker::CColorPicker(HWND _hWndParent)
{
	CHOOSECOLOR cc;
	HBRUSH hbrush;
	DWORD rgbCurrent = 0;

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = _hWndParent;
	cc.lpCustColors = (LPDWORD)s_dwAcrCustClr;
	cc.rgbResult = s_dwRgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;


	if (ChooseColor(&cc))
	{
		hbrush = CreateSolidBrush(cc.rgbResult);
		s_dwRgbCurrent = cc.rgbResult;

		m_byColor.r = GetRValue(s_dwRgbCurrent);
		m_byColor.g = GetGValue(s_dwRgbCurrent);
		m_byColor.b = GetBValue(s_dwRgbCurrent);

		m_bAccept = true;
	}
	else
	{
		m_byColor = cvec3(0, 0, 0);

		m_bAccept = false;
	}
}


CColorPicker::~CColorPicker()
{
}
