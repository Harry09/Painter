#include "Renderer.h"

#include "Client.h"

CRenderer::CRenderer(glm::vec2 _size) 
	: m_sizeWindow(_size)
{
	int ulStart = GetTickCount();

	InitOpenGL();

	printf("CRenderer initialized! (%d milisec) \n", (GetTickCount() - ulStart));
}

CRenderer::~CRenderer()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();

	delete m_pWindow;
}

void CRenderer::RenderRGBQuad(glm::vec2 _pos, glm::vec2 _size, cvec3 _color, int width)
{
	if (!m_pWindow)
		return;

	glPushMatrix();

	glColor3ub(_color.r, _color.g, _color.b);


	if (width)
	{
		glLineWidth(width);

		glBegin(GL_LINE_LOOP);
			glVertex2f(_pos.x, _pos.y);
			glVertex2f(_pos.x + _size.x, _pos.y);
			glVertex2f(_pos.x + _size.x, _pos.y + _size.y);
			glVertex2f(_pos.x, _pos.y + _size.y);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glVertex2f(_pos.x, _pos.y);
			glVertex2f(_pos.x + _size.x, _pos.y);
			glVertex2f(_pos.x + _size.x, _pos.y + _size.y);
			glVertex2f(_pos.x, _pos.y + _size.y);
		glEnd();
	}
	glPopMatrix();
}

HWND CreateSimpleToolbar(HWND hWndParent, HINSTANCE hInstance)
{
	HBITMAP hbmTool = (HBITMAP)LoadImage(hInstance, L"", IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

	TBBUTTON tbb[3];

	tbb[0].idCommand = 0;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].iString = (INT_PTR)L"Zapisz";

	tbb[1].idCommand = 1;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].iString = (INT_PTR)L"Wczytaj";

	tbb[2].idCommand = 2;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].iString = (INT_PTR)L"Wybierz kolor";


	HWND hToolbar = CreateToolbarEx(hWndParent, WS_CHILD | WS_VISIBLE, 500, 3, NULL, 0, tbb, 3,
		1, 1, 0, 0, sizeof(TBBUTTON));

	return hToolbar;
}

void CRenderer::InitOpenGL()
{
	if (!glfwInit())
		return;


	m_pWindow = glfwCreateWindow(m_sizeWindow.x, m_sizeWindow.y, "Painter", NULL, NULL);

	if (!m_pWindow)
	{
		glfwTerminate();
		return;
	}    
	
	glfwMakeContextCurrent(m_pWindow);

	glViewport(0, 0, m_sizeWindow.x, m_sizeWindow.y);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.f, m_sizeWindow.x, m_sizeWindow.y, 0);
	glMatrixMode(GL_MODELVIEW);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	// Disable resizing window
	SetWindowLong(glfwGetWin32Window(m_pWindow), GWL_STYLE, GetWindowLong(glfwGetWin32Window(m_pWindow), GWL_STYLE)&~WS_SIZEBOX);
	// Disable maximize window
	SetWindowLong(glfwGetWin32Window(m_pWindow), GWL_STYLE, GetWindowLong(glfwGetWin32Window(m_pWindow), GWL_STYLE)&~WS_MAXIMIZEBOX);
}