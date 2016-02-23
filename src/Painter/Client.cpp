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
	int ulStart = GetTickCount();

	s_pInst = this;

	m_pSettings = new CSettings();

	m_pRenderer = new CRenderer(m_pSettings->GetResolution());
	if (!m_pRenderer->GetWindow())
		return;

	m_pRenderer->SetText(0, L"Initialization...");

	m_pMenu = new CMenu();
	m_pMenu->ShowWindow();

	m_pView = new CView();

	m_pImageMgr = new CImageMgr();
	
	if (_fileName[0] == L'\0')
		m_pImageMgr->CreateImage(glm::vec2(300, 200), cvec3(255, 255, 255));
	else
		m_pImageMgr->Load(_fileName);

	m_pCursor = new CCursor();
	m_pKeyboard = new CKeyboard();

	printf("CClient initialized! (%d ms)\n", GetTickCount() - ulStart);

	m_pRenderer->SetText(0, L"Ready");

	CClient::MainLoop();
}

CClient::~CClient()
{
	delete m_pCursor;
	delete m_pKeyboard;
	delete m_pImageMgr;
	delete m_pRenderer;
	delete m_pMenu;
	delete m_pSettings;
}

void CClient::MainLoop()
{
	int ulStart = GetTickCount();

	while (!glfwWindowShouldClose(m_pRenderer->GetWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_pView->Pulse();

		m_pImageMgr->Pulse();
		m_pImageMgr->Render();

		m_pRenderer->Pulse();

		glfwSwapBuffers(m_pRenderer->GetWindow());
		glfwWaitEvents();

		Sleep(5);
	}
}