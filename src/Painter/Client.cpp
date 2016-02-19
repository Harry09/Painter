#include "Client.h"

#include "Renderer.h"
#include "ImageMgr.h"
#include "Cursor.h"
#include "Keyboard.h"

CClient* CClient::s_pInst;

CClient::CClient(HINSTANCE _hInstance, wchar_t *_fileName)
	: m_hInstance(_hInstance)
{
	int ulStart = GetTickCount();

	s_pInst = this;

	m_pRenderer = new CRenderer(glm::vec2(800, 600));
	if (!m_pRenderer->GetWindow())
		return;

	m_pImageMgr = new CImageMgr();
	
	if (_fileName[0] == L'\0')
		m_pImageMgr->CreateImage(glm::vec2(300, 200), cvec3(255, 255, 255));
	else
		m_pImageMgr->Load(_fileName);

	m_pCursor = new CCursor();
	m_pKeyboard = new CKeyboard();

	printf("CClient initialized! (%d milisec)\n", GetTickCount() - ulStart);

	CClient::MainLoop();
}

CClient::~CClient()
{
	delete m_pCursor;
	delete m_pKeyboard;
	delete m_pImageMgr;
	delete m_pRenderer;
}

void CClient::MainLoop()
{
	int ulStart = GetTickCount();

	while (!glfwWindowShouldClose(m_pRenderer->GetWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_pImageMgr->Pulse();
		m_pImageMgr->Render();

		glfwSwapBuffers(m_pRenderer->GetWindow());
		glfwWaitEvents();

		Sleep(5);
	}
}