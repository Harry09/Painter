#include "Renderer.h"

#include "Client.h"

CRenderer::CRenderer(glm::ivec2 _size) 
	: m_sizeWindow(_size)
{
	InitOpenGL();

	printf("CRenderer initialized!\n");
}

CRenderer::~CRenderer()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void CRenderer::RenderRGBQuad(glm::vec2 _pos, glm::vec2 _size, cvec3 _color, float width)
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

void CRenderer::RenderRGBQuadByPos(glm::vec2 _pos1, glm::vec2 _pos2, cvec3 _color, float width)
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

void CRenderer::RenderLine(glm::vec2 _pos1, glm::vec2 _pos2, cvec3 _color, float width)
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

	HWND _hWnd = GetHWnd();

	// Disable resizing window
	SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE)&~WS_SIZEBOX);
	// Disable maximize window
	SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE)&~WS_MAXIMIZEBOX);
}