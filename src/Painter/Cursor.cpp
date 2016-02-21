#include "Cursor.h"

#include "Client.h"
#include "Renderer.h"
#include "ImageMgr.h"
#include "Image.h"

glm::ivec2 CCursor::m_pos;
int CCursor::m_iStatus;
int CCursor::m_iButton;
float CCursor::m_fScroll;

CCursor::CCursor()
{
	glfwSetScrollCallback(CClient::Get()->GetRenderer()->GetWindow(), ScrollCallback);
	glfwSetCursorPosCallback(CClient::Get()->GetRenderer()->GetWindow(), CursorPosCallback);
	glfwSetMouseButtonCallback(CClient::Get()->GetRenderer()->GetWindow(), CursorButtonCallback);

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
	m_pos = glm::vec2(x, y);
}

void CCursor::CursorButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	m_iButton = button;
	m_iStatus = action;
}