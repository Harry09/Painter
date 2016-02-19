#pragma once
#include "Prerequisites.h"


class CKeyboard
{
private:
	short m_iKey;
	char m_iStatus;

public:
	CKeyboard();
	~CKeyboard();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void SetKey(short _key) { m_iKey = _key; }
	void SetStatus(char _status) { m_iStatus = _status; }

	short GetKey() { return m_iKey; }
	char GetStatus() { return m_iStatus; }

	bool isPressed(int _button) { return m_iStatus == GLFW_PRESS && m_iKey == _button; }
	bool isReleased(int _button) { return m_iStatus == GLFW_RELEASE && m_iKey == _button; }
	bool isRepeated(int _button) { return m_iStatus == GLFW_REPEAT && m_iKey == _button; }
};

