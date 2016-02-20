#include "Image.h"

#include "Client.h"
#include "Renderer.h"
#include "Math.h"
#include "Keyboard.h"
#include "Cursor.h"

#include <string>


CImage::CImage(glm::ivec2 _size, cvec3 _bgColor)
	: m_sizeImage(_size), m_byBgColor(_bgColor)
{
	m_pImage = new SPixel*[m_sizeImage.x];

	for (int x = 0; x < m_sizeImage.x; ++x)
	{
		m_pImage[x] = new SPixel[m_sizeImage.y];
	}

	for (int x = 0; x < m_sizeImage.x; ++x)
	{
		for (int y = 0; y < m_sizeImage.y; ++y)
		{
			m_pImage[x][y].m_color = m_byBgColor; //glm::ivec3(rand() & 255, rand() & 255, rand() & 255);
		}
	}

	srand(time(NULL));

	m_fScale = 2;
	m_fMarkerSize = 1;
	m_bRenderMarker = true;

	printf("CImage initialized! Pixels = %d Size = %d KB \n", m_sizeImage.x * m_sizeImage.y, (m_sizeImage.x * m_sizeImage.y * sizeof(SPixel)) / 1024);
}

CImage::~CImage()
{
	for (int x = 0; x<m_sizeImage.x; ++x) 
		delete[] m_pImage[x];

	delete[] m_pImage;
}

void CImage::Pulse()
{
	float scroll = CClient::Get()->GetCursor()->GetScroll();

	if ((CClient::Get()->GetKeyboard()->isPressed(GLFW_KEY_LEFT_ALT) || CClient::Get()->GetKeyboard()->isRepeated(GLFW_KEY_LEFT_ALT)) && scroll)
	{
		if (m_fMarkerSize <= 0.5f && scroll < 0) 
			return;

		m_fMarkerSize += scroll / 2;
	}	
	else
		Zoom(scroll);

	if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT) && CClient::Get()->GetKeyboard()->isPressed(GLFW_KEY_LEFT_SHIFT))
	{
		m_bRenderMarker = false;

		m_iMoving = -m_iOffset + CClient::Get()->GetCursor()->GetPos();
	}
	else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT) && CClient::Get()->GetKeyboard()->isRepeated(GLFW_KEY_LEFT_SHIFT))
	{
		m_iOffset = -m_iMoving + CClient::Get()->GetCursor()->GetPos();
	}
	else if (CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_LEFT) && CClient::Get()->GetKeyboard()->isRepeated(GLFW_KEY_LEFT_SHIFT))
	{
		m_iMoving = -m_iOffset + CClient::Get()->GetCursor()->GetPos();
	}
	else if (CClient::Get()->GetCursor()->isReleased(GLFW_MOUSE_BUTTON_LEFT) && CClient::Get()->GetKeyboard()->isReleased(GLFW_KEY_LEFT_SHIFT))
	{
		m_bRenderMarker = true;
	}
	else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		SetPixel(CClient::Get()->GetCursor()->GetPos(), m_byMColor);
	}
	else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT)) 
	{
		SetPixel(CClient::Get()->GetCursor()->GetPos(), m_byBgColor);
	}
}

void CImage::Render()
{
	// T³o
	CClient::Get()->GetRenderer()->RenderRGBQuad(m_iOffset, glm::vec2(m_sizeImage.x * m_fScale, m_sizeImage.y * m_fScale), m_byBgColor);

	// Piksele
	for (int x = 0; x < m_sizeImage.x; ++x)
	{
		for (int y = 0; y < m_sizeImage.y; ++y)
		{
			glm::vec2 _winSize = CClient::Get()->GetRenderer()->GetWindowSize();

			if (x * m_fScale < _winSize.x - m_iOffset.x && y * m_fScale < _winSize.y - m_iOffset.y && m_pImage[x][y].m_color != m_byBgColor)
				CClient::Get()->GetRenderer()->RenderRGBQuad(glm::ivec2(x * m_fScale, y * m_fScale) + m_iOffset, glm::vec2(m_fScale, m_fScale), m_pImage[x][y].m_color);
		}
	}

	// WskaŸnik
	if (m_bRenderMarker)
	{
		CClient::Get()->GetRenderer()->RenderRGBQuad(
			glm::vec2(CClient::Get()->GetCursor()->GetPos().x - 1 * m_fMarkerSize*m_fScale,
				CClient::Get()->GetCursor()->GetPos().y - 1 * m_fMarkerSize*m_fScale),
			glm::vec2(m_fMarkerSize * m_fScale * 2,
				m_fMarkerSize * m_fScale * 2),
			m_byBgColor == cvec3(0, 0, 0) ? cvec3(255, 255, 255) : cvec3(0, 0, 0),
			5);
	}
}

void CImage::Zoom(float y)
{
	m_fScale += y/10 * m_fScale;
}

void CImage::SetPixel(glm::vec2 _pos, glm::vec3 _color, bool scaling)
{
	if (scaling)
	{
		_pos -= m_iOffset;

		for (int x = 0; x < m_sizeImage.x; ++x)
		{
			for (int y = 0; y < m_sizeImage.y; ++y)
			{
				if (CMath::inRange(_pos.x, (x - m_fMarkerSize) * m_fScale, ( x + m_fMarkerSize) * m_fScale) &&
					CMath::inRange(_pos.y, (y - m_fMarkerSize) * m_fScale, ( y + m_fMarkerSize) * m_fScale))
				{
					m_pImage[x][y].m_color = _color;
				}
			}
		}
	}
	else
	{
		m_pImage[(int)_pos.x][(int)_pos.y].m_color = _color;
	}
}

void CImage::SetBgColor(cvec3 _bgColor)
{
	for (int x = 0; x < m_sizeImage.x; ++x)
	{
		for (int y = 0; y < m_sizeImage.y; ++y)
		{
			if (m_pImage[x][y].m_color == m_byBgColor)
				m_pImage[x][y].m_color = _bgColor;
		}
	}

	m_byBgColor = _bgColor;
}

void CImage::ClearScreen()
{
	if (MessageBoxA(0, "Do you want to clear a screen?", "SRSLY?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		for (int x = 0; x < m_sizeImage.x; ++x)
			for (int y = 0; y < m_sizeImage.y; ++y)
				m_pImage[x][y].m_color = m_byBgColor;

		printf("Screen clean\n");
	}
}

