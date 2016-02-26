#include "Keyboard.h"

#include "Client.h"
#include "Renderer.h"

short CKeyboard::m_iKey;
char CKeyboard::m_iStatus;
char CKeyboard::m_iMode;

CKeyboard *CKeyboard::s_pInst;

CKeyboard::CKeyboard()
{
	s_pInst = this;

	glfwSetKeyCallback(CRenderer::Get()->GetWindow(), KeyCallback);

	printf("CKeyboard initialized!\n");
}


CKeyboard::~CKeyboard()
{
}


void CKeyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_iKey = key;
	m_iStatus = action;
	m_iMode = mods;
}