#include "Cursor.h"

#include "Client.h"
#include "Renderer.h"
#include "ImageMgr.h"
#include "Image.h"

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
	CClient::Get()->GetCursor()->SetScroll(y);
}

void CCursor::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	CClient::Get()->GetCursor()->SetPos(glm::vec2(x, y));
}

 void CCursor::CursorButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	CClient::Get()->GetCursor()->SetButton(button);
	CClient::Get()->GetCursor()->SetStatus(action);
}