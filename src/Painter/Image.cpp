#include "Image.h"

#include "Client.h"
#include "Renderer.h"
#include "Math.h"
#include "Keyboard.h"
#include "Cursor.h"
#include "View.h"
#include "Menu.h"
#include "Math.h"

CImage::CImage(const glm::ivec2 &_size, const cvec3 &_bgColor)
	: m_iSizeImage(_size), m_byBgColor(_bgColor)
{
	m_pImage = new SPixel*[m_iSizeImage.x];

	for (int x = 0; x < m_iSizeImage.x; ++x)
	{
		m_pImage[x] = new SPixel[m_iSizeImage.y];
	}

	for (int x = 0; x < m_iSizeImage.x; ++x)
	{
		for (int y = 0; y < m_iSizeImage.y; ++y)
		{
			m_pImage[x][y].m_color = m_byBgColor; //glm::ivec3(rand() & 255, rand() & 255, rand() & 255);
		}
	}

	m_fMarkerSize = 1;
	m_bRenderMarker = true;

	m_fCircleRadius = 2.f;

	printf("CImage initialized! Pixels = %d Size = %d KB \n", m_iSizeImage.x * m_iSizeImage.y, (m_iSizeImage.x * m_iSizeImage.y * sizeof(SPixel)) / 1024);
}

CImage::~CImage()
{
	for (int x = 0; x<m_iSizeImage.x; ++x) 
		delete[] m_pImage[x];

	delete[] m_pImage;
}

void CImage::Pulse()
{
	float _fScroll = CCursor::Get()->GetScroll();

	if ((CKeyboard::Get()->isPressed(GLFW_KEY_LEFT_ALT) || CKeyboard::Get()->isRepeated(GLFW_KEY_LEFT_ALT)) && _fScroll)
	{
		if (CMenu::Get()->GetModeDrawing() == ID_DRAWCIRCLE)
		{
			if (m_fCircleRadius <= 0.5f && _fScroll < 0)
				return;

			m_fCircleRadius += _fScroll / 2;
		}
		else 
		{
			if (m_fMarkerSize <= 0.5f && _fScroll < 0)
				return;

			m_fMarkerSize += _fScroll / 2;
		}
	}
	else
		CView::Get()->SetScale(_fScroll);




	if (CView::Get()->isMoving())
		m_bRenderMarker = false;
	else if (CMenu::Get()->m_bGetColor && CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		cvec3 _bColor = GetColor(CCursor::Get()->GetPos());

		printf("Color = %d %d %d\n", _bColor.r, _bColor.g, _bColor.b);

		m_byMColor = _bColor;

		CMenu::Get()->m_bGetColor = false;

		CRenderer::Get()->SetText(0,0,L"Ready");
	}
	else if (CMenu::Get()->GetModeDrawing() == ID_DRAWPIXEL)
	{
		m_bRenderMarker = true;

		if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			SetPixel(CCursor::Get()->GetPos(), m_byMColor);
		}
		else if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			SetPixel(CCursor::Get()->GetPos(), m_byBgColor);
		}
	}
	else if (CMenu::Get()->GetModeDrawing() == ID_DRAWLINE)
	{
		if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_bRenderMarker = true;
			m_inClick++;

			if (m_inClick == 1)
			{
				m_fPos1 = CCursor::Get()->GetPos();
				m_inClick = 2;

				CRenderer::Get()->SetText(0,0,L"Now click somewhere on image to set the second position");
			}
			else if (m_inClick == 3)
			{
				float dir = CMath::PointDir(m_fPos1, m_fPos2);
				float len = CMath::GetLen(m_fPos1, m_fPos2);

				float angle = 2 * M_PI * dir / 360;

				for (int i = 0; i < len; ++i)
				{
					SetPixel(glm::vec2(cos(angle) * i + m_fPos1.x, sin(angle) * i + m_fPos1.y), m_byMColor);
				}

				m_fPos1 = glm::ivec2(0, 0);
				m_fPos2 = glm::ivec2(0, 0);

				m_inClick = 0;

				CRenderer::Get()->SetText(0,0,L"Click somewhere on image to set first position");
			}
		}
		else if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
			m_inClick = 0;

		if (m_inClick == 2)
		{
			m_fPos2 = CCursor::Get()->GetPos();
		}
	}
	else if (CMenu::Get()->GetModeDrawing() == ID_DRAWQUAD)
	{
		if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_bRenderMarker = true;
			m_inClick++;

			if (m_inClick == 1)
			{
				m_fPos1 = CCursor::Get()->GetPos();
				m_inClick = 2;

				CRenderer::Get()->SetText(0,0,L"Now click somewhere on image to set the second position");
			}
			else if (m_inClick == 3)
			{
				float cx = 0.f, cy = 0.f;

				cx = m_fPos2.x - m_fPos1.x;
				cy = m_fPos2.y - m_fPos1.y;

				// Top And Button
				if (cx > 0)
				{
					for (int x = 0; x < cx; ++x)
						SetPixel(glm::vec2(m_fPos1.x + float(x), m_fPos1.y), m_byMColor);
					for (int x = 0; x < cx; ++x)
						SetPixel(glm::vec2(m_fPos1.x + float(x), m_fPos2.y), m_byMColor);				
				}
				if (cx < 0)
				{
					for (int x = 0; x < -cx; ++x)
						SetPixel(glm::vec2(m_fPos1.x + float(-x), m_fPos2.y), m_byMColor);
					for (int x = 0; x < -cx; ++x)
						SetPixel(glm::vec2(m_fPos1.x + float(-x), m_fPos1.y), m_byMColor);
				}

				// Left and Right
				if (cy > 0)
				{
					for (int y = 0; y < cy; ++y)
						SetPixel(glm::vec2(m_fPos1.x, m_fPos1.y + float(y)), m_byMColor);
					for (int y = 0; y < cy; ++y)
						SetPixel(glm::vec2(m_fPos2.x, m_fPos1.y + float(y)), m_byMColor);
				}
				else if (cy < 0)
				{
					for (int y = 0; y < -cy; ++y)
						SetPixel(glm::vec2(m_fPos1.x, m_fPos1.y + float(-y)), m_byMColor);
					for (int y = 0; y < -cy; ++y)
						SetPixel(glm::vec2(m_fPos2.x, m_fPos1.y + float(-y)), m_byMColor);
				}

				m_fPos1 = glm::ivec2(0, 0);
				m_fPos2 = glm::ivec2(0, 0);

				m_inClick = 0;

				CRenderer::Get()->SetText(0,0,L"Click somewhere on image to set first position");
			}
		}
		else if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
			m_inClick = 0;

		if (m_inClick == 2)
		{
			m_fPos2 = CCursor::Get()->GetPos();
		}
	}
	else if (CMenu::Get()->GetModeDrawing() == ID_DRAWCIRCLE)
	{
		m_bRenderMarker = true;

		if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			float _markerSize = m_fMarkerSize;
			float _fScale = CView::Get()->GetScale();
			glm::vec2 _pos = CCursor::Get()->GetPos();

			m_fMarkerSize = 0.7f;

			for (int i = 0; i <= m_fCircleRadius * m_fCircleRadius; i++)
			{
				float angle = 2 * M_PI * i / (m_fCircleRadius * m_fCircleRadius);

				SetPixel(
					glm::vec2(cos(angle) * m_fCircleRadius * _fScale + _pos.x,
						sin(angle) * m_fCircleRadius * _fScale + _pos.y),
					m_byMColor);
			}

			m_fMarkerSize = _markerSize;
		}
		else if (CCursor::Get()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			float _markerSize = m_fMarkerSize;
			float _fScale = CView::Get()->GetScale();
			glm::vec2 _pos = CCursor::Get()->GetPos();

			m_fMarkerSize = 0.7f;

			for (int i = 0; i <= m_fCircleRadius * m_fCircleRadius; i++)
			{
				float angle = 2 * M_PI * i / (m_fCircleRadius * m_fCircleRadius);

				SetPixel(
					glm::vec2(cos(angle) * m_fCircleRadius * _fScale + _pos.x,
						sin(angle) * m_fCircleRadius * _fScale + _pos.y),
					m_byBgColor);
			}

			m_fMarkerSize = _markerSize;
		}
	}
	else
		m_bRenderMarker = false;
}

void CImage::Render()
{
	glm::ivec2 _iOffset = CView::Get()->GetOffset();
	float _fScale = CView::Get()->GetScale();

	//glScalef(zoom,zoom,zoom);

	// Background
	CRenderer::Get()->RenderRGBQuad(_iOffset, glm::vec2((m_iSizeImage.x - 1) * _fScale, (m_iSizeImage.y - 1) * _fScale), m_byBgColor);

	// Pixels
	for (int x = 0; x < m_iSizeImage.x; ++x)
	{
		for (int y = 0; y < m_iSizeImage.y; ++y)
		{
			glm::vec2 _winSize = CRenderer::Get()->GetWindowSize();

			if (x * _fScale < _winSize.x - _iOffset.x && y * _fScale < _winSize.y - _iOffset.y && m_pImage[x][y].m_color != m_byBgColor)
				CRenderer::Get()->RenderRGBQuad(glm::ivec2(x * _fScale, y * _fScale) + _iOffset, glm::vec2(_fScale, _fScale), m_pImage[x][y].m_color);
		}
	}

	// Pointer to marker
	if (m_bRenderMarker)
	{
		if (CMenu::Get()->GetModeDrawing() == ID_DRAWPIXEL)
		{
			CRenderer::Get()->RenderRGBQuad(
				glm::vec2(CCursor::Get()->GetPos().x - 1 * m_fMarkerSize * _fScale,
					CCursor::Get()->GetPos().y - 1 * m_fMarkerSize * _fScale),
				glm::vec2(m_fMarkerSize * _fScale * 2,
					m_fMarkerSize * _fScale * 2),
				m_byMColor);

			CRenderer::Get()->RenderRGBQuad(
				glm::vec2(CCursor::Get()->GetPos().x - 1 * m_fMarkerSize * _fScale,
					CCursor::Get()->GetPos().y - 1 * m_fMarkerSize * _fScale),
				glm::vec2(m_fMarkerSize * _fScale * 2,
					m_fMarkerSize * _fScale * 2),
				m_byBgColor == cvec3(0, 0, 0) ? cvec3(255, 255, 255) : m_byMColor == cvec3(0, 0, 0) ? m_byBgColor == cvec3(255, 255, 255) ? cvec3(192, 192, 192) : cvec3(255,255,255) : cvec3(0, 0, 0), 3);
		}
		else if (CMenu::Get()->GetModeDrawing() == ID_DRAWLINE && m_inClick == 2) // Line
			CRenderer::Get()->RenderLine(m_fPos1, m_fPos2, m_byMColor, m_fMarkerSize * 5);
		else if (CMenu::Get()->GetModeDrawing() == ID_DRAWQUAD && m_inClick == 2) // Quad
			CRenderer::Get()->RenderRGBQuadByPos(m_fPos1, m_fPos2, m_byMColor, m_fMarkerSize * 5);
		else if (CMenu::Get()->GetModeDrawing() == ID_DRAWCIRCLE) // Circle
		{
			CRenderer::Get()->RenderCircle(
				glm::vec2(CCursor::Get()->GetPos().x,
					CCursor::Get()->GetPos().y),
				m_byMColor,
				m_fCircleRadius * _fScale, m_fCircleRadius * _fScale);
		}
	}
}

void CImage::SetPixel(const glm::vec2 &_pos, const glm::vec3 &_color, bool scaling)
{
	glm::vec2 _fPos = _pos;

	if (scaling)
	{
		glm::ivec2 _iOffset = CView::Get()->GetOffset();
		float _fScale = CView::Get()->GetScale();

		_fPos -= _iOffset;

		for (int x = 0; x < m_iSizeImage.x; ++x)
		{
			for (int y = 0; y < m_iSizeImage.y; ++y)
			{
				if (CMath::inRange(_fPos.x, (x - 0.5 - m_fMarkerSize) * _fScale, ( x - 0.5 + m_fMarkerSize) * _fScale) &&
					CMath::inRange(_fPos.y, (y - 0.5 - m_fMarkerSize) * _fScale, ( y - 0.5 + m_fMarkerSize) * _fScale))
				{
					if ((x - 1) < 0 || (y - 1) < 0)
						return;
				
					m_pImage[x-1][y-1].m_color = _color;
				}
			}
		}
	}
	else
	{
		m_pImage[(int)_fPos.x][(int)_fPos.y].m_color = _color;
	}
}

void CImage::SetBgColor(const cvec3 &_bgColor)
{
	for (int x = 0; x < m_iSizeImage.x; ++x)
	{
		for (int y = 0; y < m_iSizeImage.y; ++y)
		{
			if (m_pImage[x][y].m_color == m_byBgColor)
				m_pImage[x][y].m_color = _bgColor;
		}
	}

	m_byBgColor = _bgColor;
}

void CImage::ClearScreen()
{
	if (MessageBoxA(0, "Are you sure that you want to clean screen?", "SRSLY?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		for (int x = 0; x < m_iSizeImage.x; ++x)
			for (int y = 0; y < m_iSizeImage.y; ++y)
				m_pImage[x][y].m_color = m_byBgColor;

		printf("Screen clean\n");
	}
}

cvec3 CImage::GetColor(const glm::vec2 &_pos, bool scaling)
{
	cvec3 _color = cvec3(0, 0, 0);
	glm::vec2 _fPos = _pos;
	
	if (scaling)
	{
		glm::ivec2 _iOffset = CView::Get()->GetOffset();
		float _fScale = CView::Get()->GetScale();

		_fPos -= _iOffset;

		for (int x = 0; x < m_iSizeImage.x; ++x)
		{
			for (int y = 0; y < m_iSizeImage.y; ++y)
			{
				if (CMath::inRange(_fPos.x, (x - 1.f) * _fScale, x * _fScale) &&
					CMath::inRange(_fPos.y, (y - 1.f) * _fScale, y * _fScale))
				{
					if ((x - 1) < 0 || (y - 1) < 0)
						return cvec3(255,255,255);

					_color = m_pImage[x - 1][y - 1].m_color;
				}
			}
		}
	}
	else
	{
		_color = m_pImage[(int)_fPos.x][(int)_fPos.y].m_color;
	}
	
	return _color;
}