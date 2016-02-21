#pragma once
#include "Prerequisites.h"


class CKeyboard
{
private:
	static short m_iKey;
	static char m_iStatus;
	static char m_iMode;

public:
	CKeyboard();
	~CKeyboard();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	short GetKey() { return m_iKey; }
	char GetStatus() { return m_iStatus; }
	char GetMode() { return m_iMode; }

	bool isPressed(int _button) { return m_iStatus == GLFW_PRESS && m_iKey == _button; }
	bool isReleased(int _button) { return m_iStatus == GLFW_RELEASE && m_iKey == _button; }
	bool isRepeated(int _button) { return m_iStatus == GLFW_REPEAT && m_iKey == _button; }
};

