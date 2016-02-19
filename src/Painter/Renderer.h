#include "Prerequisites.h"

#pragma once
class CRenderer
{
private:
	GLFWwindow *m_pWindow;
	glm::ivec2 m_sizeWindow;

private:
	void InitOpenGL();

public:
	CRenderer(glm::	vec2 _size);
	~CRenderer();
	
	//rgb = 0 - 255
	void RenderRGBQuad(glm::vec2 _pos, glm::vec2 _size, cvec3 _rgb, int width = 0);

	GLFWwindow* GetWindow() const { return m_pWindow; }
	glm::ivec2 GetWindowSize() const { return m_sizeWindow; }
	HWND GetHWnd() { return glfwGetWin32Window(m_pWindow); }
};

