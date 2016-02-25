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

float _fScale;

void CView::SetScale(float y)
{
	if (y > 0 && m_fScale >= 20)
		return;

	m_fScale += y / 10 * m_fScale;

	if (m_fScale > 17.5)
		_fScale = 0.5f;
	else if (m_fScale > 15)
		_fScale = 0.8f;
	else if (m_fScale > 10)
		_fScale = 1.f;
	else if (m_fScale > 5)
		_fScale = 1.5f;
	else
		_fScale = 1 + (21 - m_fScale)/10;
		
	m_iOffset.x -= y * -(CClient::Get()->GetRenderer()->GetWindowSize().x / 3 - CClient::Get()->GetCursor()->GetPos().x) / _fScale;
	m_iOffset.y -= y * -(CClient::Get()->GetRenderer()->GetWindowSize().y / 3 - CClient::Get()->GetCursor()->GetPos().y) / _fScale;
}