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

	static void KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);

	short GetKey() { return m_iKey; }
	char GetStatus() { return m_iStatus; }
	char GetMode() { return m_iMode; }

	bool isPressed(int _button) { return m_iStatus == GLFW_PRESS && m_iKey == _button; }
	bool isReleased(int _button) { return m_iStatus == GLFW_RELEASE && m_iKey == _button; }
	bool isRepeated(int _button) { return m_iStatus == GLFW_REPEAT && m_iKey == _button; }	
	
	
	static CKeyboard *Get() { return s_pInst; }

private:
	static CKeyboard *s_pInst;
};

