#pragma once

#include "Prerequisites.h"

class CClient
{
private:
	static CClient* s_pInst;

	CRenderer* m_pRenderer;
	CImageMgr* m_pImageMgr;
	CCursor* m_pCursor;
	CKeyboard* m_pKeyboard;
	CSettings* m_pSettings;
	CMenu* m_pMenu;

	HINSTANCE m_hInstance;

private:
	void MainLoop();

public:
	CClient(HINSTANCE, wchar_t*);
	~CClient();

	CRenderer* GetRenderer() const { return m_pRenderer; }
	CImageMgr* GetImgMgr() const { return m_pImageMgr; }
	CCursor* GetCursor() const { return m_pCursor; } 
	CKeyboard* GetKeyboard() const { return m_pKeyboard; }
	CSettings* GetSettings() const { return m_pSettings; }
	CMenu* GetMenu() const { return m_pMenu; }
	HINSTANCE GetHInstance() const { return m_hInstance; }

	static CClient* Get() { return s_pInst; }
};

