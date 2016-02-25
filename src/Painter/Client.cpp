#include "Client.h"

#include "Renderer.h"
#include "View.h"
#include "ImageMgr.h"
#include "Cursor.h"
#include "Keyboard.h"
#include "Settings.h"
#include "Menu.h"

CClient* CClient::s_pInst;

CClient::CClient(HINSTANCE _hInstance, wchar_t *_fileName)
	: m_hInstance(_hInstance)
{
	int iStart = GetTickCount();

	m_pRenderer = 0;
	m_pView = 0;
	m_pImageMgr = 0;
	m_pCursor = 0;
	m_pKeyboard = 0;
	m_pSettings = 0;
	m_pMenu = 0;
	m_iExitCode = 0;

	s_pInst = this;

	m_pSettings = new CSettings();

	m_pRenderer = new CRenderer(m_pSettings->GetResolution());
	if (!m_pRenderer->GetWindow())
		return;

	m_pRenderer->SetText(0,0, L"Initialization...");

	m_pMenu = new CMenu();
	m_pMenu->ShowWindow();

	SetPosOfWindows();
	SetFocus(m_pRenderer->GetHWnd());

	m_pView = new CView();

	m_pImageMgr = new CImageMgr();
	
	if (!_fileName || _fileName[0] == L'\0')
		m_pImageMgr->CreateImage(glm::vec2(300, 200), cvec3(255, 255, 255));
	else
		m_pImageMgr->Load(_fileName);

	m_pCursor = new CCursor();
	m_pKeyboard = new CKeyboard();

	printf("CClient initialized! (%d ms)\n", GetTickCount() - iStart);

	m_pRenderer->SetText(0,0, L"Ready");

	m_iExitCode = 1;

	CClient::MainLoop();
}

CClient::~CClient()
{
	if (m_iExitCode == 1)
	{
		delete m_pCursor;
		delete m_pKeyboard;
		delete m_pImageMgr;
		delete m_pRenderer;
		delete m_pMenu;
		delete m_pSettings;
	}
}

void CClient::ShowCursorPos()
{
	glm::ivec2 _pos = m_pCursor->GetPos();

	glm::ivec2 _offset = CClient::Get()->GetView()->GetOffset();
	float _scale = CClient::Get()->GetView()->GetScale();

	_pos -= _offset;

	_pos.x = _pos.x/_scale + 1;
	_pos.y = _pos.y/_scale + 1;

	m_pRenderer->SetText(1, 0, L"(%d, %d)", _pos.x, _pos.y);
}

void CClient::MainLoop()
{
	while (!glfwWindowShouldClose(m_pRenderer->GetWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_pView->Pulse();

		m_pImageMgr->Pulse();
		m_pImageMgr->Render();

		ShowCursorPos();

		m_pRenderer->Pulse();

		glfwSwapBuffers(m_pRenderer->GetWindow());
		glfwWaitEvents();
	}
}

void CClient::SetPosOfWindows()
{
	RECT rcMain, rcMenu;
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	HWND hRenderer = m_pRenderer->GetHWnd(), hMenu = m_pMenu->GetHWnd();

	GetWindowRect(hRenderer, &rcMain);
	SetWindowPos(hRenderer, HWND_TOP, cx / 2 - (rcMain.right - rcMain.left) / 3, cy / 2 - (rcMain.bottom - rcMain.top) / 2, 0, 0, SWP_NOSIZE);

	GetWindowRect(hMenu, &rcMenu);
	GetWindowRect(hRenderer, &rcMain);
	SetWindowPos(hMenu, HWND_TOP, rcMain.left - (rcMenu.right - rcMenu.left), rcMain.top, 0, 0, SWP_NOSIZE);
}
