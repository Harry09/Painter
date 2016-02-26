#include "Cursor.h"

#include "Client.h"
#include "Renderer.h"
#include "ImageMgr.h"
#include "Image.h"

glm::ivec2 CCursor::m_iPos;
int CCursor::m_iStatus;
int CCursor::m_iButton;
float CCursor::m_fScroll;

CCursor *CCursor::s_pInst;

CCursor::CCursor()
{
	s_pInst = this;

	glfwSetScrollCallback(CRenderer::Get()->GetWindow(), ScrollCallback);
	glfwSetCursorPosCallback(CRenderer::Get()->GetWindow(), CursorPosCallback);
	glfwSetMouseButtonCallback(CRenderer::Get()->GetWindow(), CursorButtonCallback);

	printf("CCursor initialized!\n");
}

CCursor::~CCursor()
{
}

void CCursor::ScrollCallback(GLFWwindow* window, double x, double y)
{
	m_fScroll = float(y);
}

void CCursor::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	m_iPos = glm::vec2(x, y);
}

void CCursor::CursorButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	m_iButton = button;
	m_iStatus = action;
}