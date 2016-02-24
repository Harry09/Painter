#include "View.h"

#include "Client.h"
#include "Renderer.h"
#include "Cursor.h"
#include "Keyboard.h"
#include "ImageMgr.h"
#include "Image.h"

CView::CView()
{
	m_fScale = 2;
	m_bMoving = false;
}

CView::~CView()
{

}

void CView::Pulse()
{	
	if (CClient::Get()->GetKeyboard()->isPressed(GLFW_KEY_LEFT_SHIFT))
	{
		m_bMoving = true;
	}
	if ((CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT) || CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		&& CClient::Get()->GetKeyboard()->isPressed(GLFW_KEY_LEFT_SHIFT))
	{
		m_bMoving = true;
		m_iMoving = -m_iOffset + CClient::Get()->GetCursor()->GetPos();
	}
	else if ((CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT) || CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
		&& CClient::Get()->GetKeyboard()->isRepeated(GLFW_KEY_LEFT_SHIFT))
	{
		m_bMoving = true;
		m_iOffset = -m_iMoving + CClient::Get()->GetCursor()->GetPos();
	}
	else if ((CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_LEFT) || CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_RIGHT))
		&& CClient::Get()->GetKeyboard()->isRepeated(GLFW_KEY_LEFT_SHIFT))
	{
		m_bMoving = true;
		m_iMoving = -m_iOffset + CClient::Get()->GetCursor()->GetPos();
	}
	else if ((CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_LEFT) || CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_RIGHT))
		&& CClient::Get()->GetKeyboard()->isReleased(GLFW_KEY_LEFT_SHIFT))
	{
		m_bMoving = false;
	}
}

void CView::ResetView()
{
	m_fScale = 2;
	m_iMoving = glm::ivec2(0, 0);
	m_iOffset = glm::ivec2(0, 0);
}

void CView::SetScale(float y)
{
	m_fScale += y / 10 * m_fScale;

	m_iOffset.x -= y * -(CClient::Get()->GetRenderer()->GetWindowSize().x * 1 / 3 - CClient::Get()->GetCursor()->GetPos().x)/4;
	m_iOffset.y -= y * -(CClient::Get()->GetRenderer()->GetWindowSize().y * 1 / 3 - CClient::Get()->GetCursor()->GetPos().y)/4;
}