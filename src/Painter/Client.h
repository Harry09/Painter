#pragma once

#include "Prerequisites.h"

enum ExitCodes
{
	ID_EXIT_CODE_UNINIT = 0,
	ID_EXIT_CODE_OK
};

class CClient
{
	CRenderer *m_pRenderer;
	CView *m_pView;
	CImageMgr *m_pImageMgr;
	CCursor *m_pCursor;
	CKeyboard *m_pKeyboard;
	CSettings *m_pSettings;
	CMenu *m_pMenu;


	int m_iExitCode;

private:
	void MainLoop();
	void SetPosOfWindows();


public:
	CClient(wchar_t*);
	~CClient();

	int GetExitCode() { return m_iExitCode; }

	void ShowCursorPos();


	static CClient *Get() { return s_pInst; }

private:
	static CClient *s_pInst;
};

