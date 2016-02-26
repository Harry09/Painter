#include "Renderer.h"

#include "Client.h"

CRenderer *CRenderer::s_pInst;

CRenderer::CRenderer(glm::ivec2 _size) 
	: m_iSizeWindow(_size), m_pWindow(0)
{
	s_pInst = this;

	InitOpenGL();
	InitStatusBar();

	HWND _hWnd = GetHWnd();
	// Disable resizing window
	SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE)&~WS_SIZEBOX);
	// Disable maximize window
	SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE)&~WS_MAXIMIZEBOX);

	printf("CRenderer initialized!\n");
}

CRenderer::~CRenderer()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void CRenderer::RenderRGBQuad(const glm::vec2 &_pos, const glm::vec2 &_size, const cvec3 &_color, float width)
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

void CRenderer::RenderRGBQuadByPos(const glm::vec2 &_pos1, const glm::vec2 &_pos2, const cvec3 &_color, float width)
{
	if (!m_pWindow)
		return;

	glPushMatrix();

		glColor3ub(_color.r, _color.g, _color.b);

		if (width)
		{
			glLineWidth(width);

			glBegin(GL_LINE_LOOP);
				glVertex2f(_pos1.x, _pos1.y);
				glVertex2f(_pos2.x, _pos1.y);
				glVertex2f(_pos2.x, _pos2.y);
				glVertex2f(_pos1.x, _pos2.y);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
				glVertex2f(_pos1.x, _pos1.y);
				glVertex2f(_pos2.x, _pos1.y);
				glVertex2f(_pos2.x, _pos2.y);
				glVertex2f(_pos1.x, _pos2.y);
			glEnd();
		}

	glPopMatrix();
}

void CRenderer::RenderLine(const glm::vec2 &_pos1, const glm::vec2 &_pos2, const cvec3 &_color, float width)
{
	glPushMatrix();
		glColor3ub(_color.r, _color.g, _color.b);
		glLineWidth(width);
	
		glBegin(GL_LINES);
			glVertex2f(_pos1.x, _pos1.y);
			glVertex2f(_pos2.x, _pos2.y);
		glEnd();
	glPopMatrix();
}

void CRenderer::RenderCircle(const glm::vec2 &_pos, const cvec3 &_color, float _radius, int _vertices)
{
	glPushMatrix();
		glColor3ub(_color.r, _color.g, _color.b);
		glLineWidth(5.f);

		glBegin(GL_LINE_LOOP);
			for (int i = 0; i <= _vertices; i++)
			{
				float angle = 2 * M_PI * i / _vertices;
				glVertex2f(cos(angle) * _radius + _pos.x, sin(angle) * _radius + _pos.y);
			}
		glEnd();
	glPopMatrix();
}

void CRenderer::InitOpenGL()
{
	if (!glfwInit())
		return;


	m_pWindow = glfwCreateWindow(m_iSizeWindow.x, m_iSizeWindow.y, "Painter", NULL, NULL);

	if (!m_pWindow)
	{
		glfwTerminate();
		return;
	}    
	
	glfwMakeContextCurrent(m_pWindow);

	glViewport(0, 0, m_iSizeWindow.x, m_iSizeWindow.y);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.f, m_iSizeWindow.x, m_iSizeWindow.y, 0);
	glMatrixMode(GL_MODELVIEW);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
}

void CRenderer::InitStatusBar()
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);

	m_hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GetHWnd(), (HMENU)200, GetModuleHandle(0), NULL);

	int iStatusBarWidths[] = { m_iSizeWindow.x*3/4, -1 };
	SendMessage(m_hStatusBar, SB_SETPARTS, 2, (LPARAM)iStatusBarWidths);
}

void CRenderer::Pulse()
{
	if (m_iTimeoutStatus != 0 && m_iTimeoutStatus < GetTickCount() - m_iStartTimeout)
		SetText(0, 0, L"Ready");
}

void CRenderer::SetText(int id, int _timeout, wchar_t* _text, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, _text);
	vswprintf(buffer, 256, _text, args);
	va_end(args);

	m_iTimeoutStatus = _timeout;
	m_iStartTimeout = GetTickCount();
	SendMessage(m_hStatusBar, SB_SETTEXT, id, (LPARAM)buffer);
}