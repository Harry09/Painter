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

	m_fMarkerSize = 1;
	m_bRenderMarker = true;

	m_fCircleRadius = 2.f;

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
		if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWCIRCLE)
		{
			if (m_fCircleRadius <= 0.5f && scroll < 0)
				return;

			m_fCircleRadius += scroll / 2;
		}
		else 
		{
			if (m_fMarkerSize <= 0.5f && scroll < 0)
				return;

			m_fMarkerSize += scroll / 2;
		}
	}
	else
		CClient::Get()->GetView()->SetScale(scroll);




	if (CClient::Get()->GetView()->isMoving())
		m_bRenderMarker = false;
	else if (CClient::Get()->GetMenu()->m_bGetColor && CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		cvec3 _color = GetColor(CClient::Get()->GetCursor()->GetPos());

		printf("Color = %d %d %d\n", _color.r, _color.g, _color.b);

		m_byMColor = _color;

		CClient::Get()->GetMenu()->m_bGetColor = false;

		CClient::Get()->GetRenderer()->SetText(0, L"Ready");
	}
	else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWPIXEL)
	{
		m_bRenderMarker = true;

		if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			SetPixel(CClient::Get()->GetCursor()->GetPos(), m_byMColor);
		}
		else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			SetPixel(CClient::Get()->GetCursor()->GetPos(), m_byBgColor);
		}
	}
	else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWLINE)
	{
		if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_bRenderMarker = true;
			m_inClick++;

			if (m_inClick == 1)
			{
				m_fPos1 = CClient::Get()->GetCursor()->GetPos();
				m_inClick = 2;

				CClient::Get()->GetRenderer()->SetText(0, L"Now click somewhere on image to set the second position");
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

				CClient::Get()->GetRenderer()->SetText(0, L"Click somewhere on image to set first position");
			}
		}
		else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
			m_inClick = 0;

		if (m_inClick == 2)
		{
			m_fPos2 = CClient::Get()->GetCursor()->GetPos();
		}
	}
	else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWQUAD)
	{
		if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			m_bRenderMarker = true;
			m_inClick++;

			if (m_inClick == 1)
			{
				m_fPos1 = CClient::Get()->GetCursor()->GetPos();
				m_inClick = 2;

				CClient::Get()->GetRenderer()->SetText(0, L"Now click somewhere on image to set the second position");
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

				CClient::Get()->GetRenderer()->SetText(0, L"Click somewhere on image to set first position");
			}
		}
		else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
			m_inClick = 0;

		if (m_inClick == 2)
		{
			m_fPos2 = CClient::Get()->GetCursor()->GetPos();
		}
	}
	else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWCIRCLE)
	{
		m_bRenderMarker = true;

		if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			float _markerSize = m_fMarkerSize;
			float _scale = CClient::Get()->GetView()->GetScale();
			glm::vec2 _pos = CClient::Get()->GetCursor()->GetPos();

			m_fMarkerSize = 0.7f;

			for (int i = 0; i <= m_fCircleRadius * m_fCircleRadius; i++)
			{
				float angle = 2 * M_PI * i / (m_fCircleRadius * m_fCircleRadius);

				SetPixel(
					glm::vec2(cos(angle) * m_fCircleRadius * _scale + _pos.x,
						sin(angle) * m_fCircleRadius * _scale + _pos.y),
					m_byMColor);
			}

			m_fMarkerSize = _markerSize;
		}
		else if (CClient::Get()->GetCursor()->isPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			float _markerSize = m_fMarkerSize;
			float _scale = CClient::Get()->GetView()->GetScale();
			glm::vec2 _pos = CClient::Get()->GetCursor()->GetPos();

			m_fMarkerSize = 0.7f;

			for (int i = 0; i <= m_fCircleRadius * m_fCircleRadius; i++)
			{
				float angle = 2 * M_PI * i / (m_fCircleRadius * m_fCircleRadius);

				SetPixel(
					glm::vec2(cos(angle) * m_fCircleRadius * _scale + _pos.x,
						sin(angle) * m_fCircleRadius * _scale + _pos.y),
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
	glm::ivec2 _offset = CClient::Get()->GetView()->GetOffset();
	float _scale = CClient::Get()->GetView()->GetScale();

	//glScalef(zoom,zoom,zoom);

	// Background
	CClient::Get()->GetRenderer()->RenderRGBQuad(_offset, glm::vec2((m_sizeImage.x - 1) * _scale, (m_sizeImage.y - 1) * _scale), m_byBgColor);

	// Pixels
	for (int x = 0; x < m_sizeImage.x; ++x)
	{
		for (int y = 0; y < m_sizeImage.y; ++y)
		{
			glm::vec2 _winSize = CClient::Get()->GetRenderer()->GetWindowSize();

			if (x * _scale < _winSize.x - _offset.x && y * _scale < _winSize.y - _offset.y && m_pImage[x][y].m_color != m_byBgColor)
				CClient::Get()->GetRenderer()->RenderRGBQuad(glm::ivec2(x * _scale, y * _scale) + _offset, glm::vec2(_scale, _scale), m_pImage[x][y].m_color);
		}
	}

	// Pointer to marker
	if (m_bRenderMarker)
	{
		if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWPIXEL)
		{
			CClient::Get()->GetRenderer()->RenderRGBQuad(
				glm::vec2(CClient::Get()->GetCursor()->GetPos().x - 1 * m_fMarkerSize * _scale,
					CClient::Get()->GetCursor()->GetPos().y - 1 * m_fMarkerSize * _scale),
				glm::vec2(m_fMarkerSize * _scale * 2,
					m_fMarkerSize * _scale * 2),
				m_byMColor);

			CClient::Get()->GetRenderer()->RenderRGBQuad(
				glm::vec2(CClient::Get()->GetCursor()->GetPos().x - 1 * m_fMarkerSize * _scale,
					CClient::Get()->GetCursor()->GetPos().y - 1 * m_fMarkerSize * _scale),
				glm::vec2(m_fMarkerSize * _scale * 2,
					m_fMarkerSize * _scale * 2),
				m_byBgColor == cvec3(0, 0, 0) ? cvec3(255, 255, 255) : m_byMColor == cvec3(0, 0, 0) ? m_byBgColor == cvec3(255, 255, 255) ? cvec3(192, 192, 192) : cvec3(255,255,255) : cvec3(0, 0, 0), 3);
		}
		else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWLINE && m_inClick == 2) // Line
			CClient::Get()->GetRenderer()->RenderLine(m_fPos1, m_fPos2, m_byMColor, m_fMarkerSize * 5);
		else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWQUAD && m_inClick == 2) // Quad
			CClient::Get()->GetRenderer()->RenderRGBQuadByPos(m_fPos1, m_fPos2, m_byMColor, m_fMarkerSize * 5);
		else if (CClient::Get()->GetMenu()->GetModeDrawing() == ID_DRAWCIRCLE) // Circle
		{
			CClient::Get()->GetRenderer()->RenderCircle(
				glm::vec2(CClient::Get()->GetCursor()->GetPos().x,
					CClient::Get()->GetCursor()->GetPos().y),
				m_byMColor,
				m_fCircleRadius * _scale, m_fCircleRadius * _scale);
		}
	}
}

void CImage::SetPixel(const glm::vec2 &_pos, const glm::vec3 &_color, bool scaling)
{
	glm::vec2 pos = _pos;

	if (scaling)
	{
		glm::ivec2 _offset = CClient::Get()->GetView()->GetOffset();
		float _scale = CClient::Get()->GetView()->GetScale();

		pos -= _offset;

		for (int x = 0; x < m_sizeImage.x; ++x)
		{
			for (int y = 0; y < m_sizeImage.y; ++y)
			{
				if (CMath::inRange(pos.x, (x - 0.5 - m_fMarkerSize) * _scale, ( x - 0.5 + m_fMarkerSize) * _scale) &&
					CMath::inRange(pos.y, (y - 0.5 - m_fMarkerSize) * _scale, ( y - 0.5 + m_fMarkerSize) * _scale))
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
		m_pImage[(int)pos.x][(int)pos.y].m_color = _color;
	}
}

void CImage::SetBgColor(const cvec3 &_bgColor)
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
	if (MessageBoxA(0, "Are you sure that you want to clean screen?", "SRSLY?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		for (int x = 0; x < m_sizeImage.x; ++x)
			for (int y = 0; y < m_sizeImage.y; ++y)
				m_pImage[x][y].m_color = m_byBgColor;

		printf("Screen clean\n");
	}
}

cvec3 CImage::GetColor(const glm::vec2 &_pos, bool scaling)
{
	cvec3 _color = cvec3(0, 0, 0);
	glm::vec2 pos = _pos;
	
	if (scaling)
	{
		glm::ivec2 _offset = CClient::Get()->GetView()->GetOffset();
		float _scale = CClient::Get()->GetView()->GetScale();

		pos -= _offset;

		for (int x = 0; x < m_sizeImage.x; ++x)
		{
			for (int y = 0; y < m_sizeImage.y; ++y)
			{
				if (CMath::inRange(pos.x, (x - 1.f) * _scale, x * _scale) &&
					CMath::inRange(pos.y, (y - 1.f) * _scale, y * _scale))
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
		_color = m_pImage[(int)pos.x][(int)pos.y].m_color;
	}
	
	return _color;
}