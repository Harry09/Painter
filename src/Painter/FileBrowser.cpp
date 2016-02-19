#include "FileBrowser.h"



CFileBrowser::CFileBrowser(TypeFileBrowser type, wchar_t *_filter, HWND _hWndParent)
{
	if (type > 1)
		return;

	OPENFILENAME ofn;
	wchar_t szFile[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hWndParent;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (type == DIALOG_OPEN)
	{
		if (GetOpenFileName(&ofn) == TRUE)
		{
			m_bAccept = true;
			wcscpy(m_szFileName, ofn.lpstrFile);
		}
		else
		{
			m_bAccept = false;
			wcscpy(m_szFileName, L"\0");
		}
	}
	else if (type == DIALOG_SAVE)
	{
		if (GetSaveFileName(&ofn) == TRUE)
		{
			m_bAccept = true;
			wcscpy(m_szFileName, ofn.lpstrFile);
		}
		else
		{
			m_bAccept = false;
			wcscpy(m_szFileName, L"\0");
		}
	}
}


CFileBrowser::~CFileBrowser()
{
}
