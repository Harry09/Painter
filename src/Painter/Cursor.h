#pragma once

#include "Prerequisites.h"



class CCursor
{
private:
	glm::ivec2 m_pos;

	int m_iStatus;
	int m_iButton;
	float m_fScroll;

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

	void SetPos(glm::ivec2 _pos) { m_pos = _pos; }
	void SetStatus(int _status) { m_iStatus = _status; }
	void SetButton(int _button) { m_iButton = _button; }
	void SetScroll(float _scroll) { m_fScroll = _scroll; }

	bool isPressed(int _button) { return m_iStatus == GLFW_PRESS && m_iButton == _button; }
	bool isReleased(int _button) { return m_iStatus == GLFW_RELEASE && m_iButton == _button; }
};

