#pragma once

#include "Prerequisites.h"



class CCursor
{
private:
	static glm::ivec2 m_pos;
	static int m_iStatus;
	static int m_iButton;
	static float m_fScroll;

private:
	static void ScrollCallback(GLFWwindow* _window, double _x, double _y);
	static void CursorPosCallback(GLFWwindow* _window, double _x, double _y);
	static void CursorButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
public:
	CCursor();
	~CCursor();

	glm::ivec2 GetPos() { return m_pos; }
	int GetStatus() { return m_iStatus; }
	int GetButton() { return m_iButton; }
	float GetScroll() { float _scroll = m_fScroll; m_fScroll = 0; return _scroll; } // m_fScroll nie zerowa³ siê je¿eli event siê nie wykonywa³

	bool isPressed(int _button) { return m_iStatus == GLFW_PRESS && m_iButton == _button; }
	bool isReleased(int _button) { return m_iStatus == GLFW_RELEASE && m_iButton == _button; }
};

