#pragma once

#include "Prerequisites.h"

enum TypeFileBrowser
{
	DIALOG_SAVE = 0,
	DIALOG_OPEN
};

class CFileBrowser
{
private:
	wchar_t m_szFileName[MAX_PATH];
	bool m_bAccept;
	TypeFileBrowser m_iType;

public:
	// TYPE: DIALOG_SAVE, DIALOG_OPEN
	CFileBrowser(TypeFileBrowser type, wchar_t* _filer = L"All\0*.*\0", HWND _hWndParent = 0);
	~CFileBrowser();

	wchar_t* GetFileName() { return m_szFileName; }
	bool Accepted() { return m_bAccept; }
	TypeFileBrowser GetType() { return m_iType; }
};

