#include "Keyboard.h"

#include "Client.h"
#include "Renderer.h"

CKeyboard::CKeyboard()
{
	glfwSetKeyCallback(CClient::Get()->GetRenderer()->GetWindow(), KeyCallback);

	printf("CKeyboard initialized!\n");
}


CKeyboard::~CKeyboard()
{
}


void CKeyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	CClient::Get()->GetKeyboard()->SetKey(key);
	CClient::Get()->GetKeyboard()->SetStatus(action);
}