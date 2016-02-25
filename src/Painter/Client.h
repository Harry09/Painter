#pragma once

#include "Prerequisites.h"

enum ExitCodes
{
	ID_EXIT_CODE_UNINIT = 0,
	ID_EXIT_CODE_OK
};

class CClient
{
private:
	static CClient *s_pInst;

	CRenderer *m_pRenderer;
	CView *m_pView;
	CImageMgr *m_pImageMgr;
	CCursor *m_pCursor;
	CKeyboard *m_pKeyboard;
	CSettings *m_pSettings;
	CMenu *m_pMenu;

	HINSTANCE m_hInstance;

	int m_iExitCode;

private:
	void MainLoop();
	void SetPosOfWindows();


public:
	CClient(HINSTANCE, wchar_t*);
	~CClient();

	CRenderer* GetRenderer() const { return m_pRenderer; }
	CView* GetView() const { return m_pView; }
	CImageMgr* GetImgMgr() const { return m_pImageMgr; }
	CCursor* GetCursor() const { return m_pCursor; } 
	CKeyboard* GetKeyboard() const { return m_pKeyboard; }
	CSettings* GetSettings() const { return m_pSettings; }
	CMenu* GetMenu() const { return m_pMenu; }
	HINSTANCE GetHInstance() const { return m_hInstance; }

	static CClient* Get() { return s_pInst; }

	int GetExitCode() { return m_iExitCode; }

	void ShowCursorPos();
};

