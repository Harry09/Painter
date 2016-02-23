#include "Prerequisites.h"

#pragma once
class CRenderer
{
private:
	GLFWwindow *m_pWindow;
	glm::ivec2 m_sizeWindow;

	HWND m_hStatusBar;
	int m_iTimeoutStatus;
	int m_ulStartTimeout;

private:
	void InitOpenGL();
	void InitStatusBar();

public:
	CRenderer(glm::ivec2 _size);
	~CRenderer();
	
	//rgb = 0 - 255
	void RenderRGBQuad(glm::vec2 _pos, glm::vec2 _size, cvec3 _rgb, float width = 0);
	void RenderRGBQuadByPos(glm::vec2 _pos1, glm::vec2 _pos2, cvec3 _rgb, float width = 0);
	void RenderLine(glm::vec2 _pos1, glm::vec2 _pos2, cvec3 _rgb, float width);
	void RenderCircle(glm::vec2 _pos, cvec3 _rgb, float _radius, int _vertices = 180);

	GLFWwindow* GetWindow() const { return m_pWindow; }
	glm::ivec2 GetWindowSize() const { return m_sizeWindow; }
	HWND GetHWnd() { return glfwGetWin32Window(m_pWindow); }

	void Pulse();

	// Timeout in ms. (0 = never)
	void SetText(int timeout, wchar_t* _text, ...);
};

