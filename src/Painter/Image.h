#pragma once

#include "Prerequisites.h"

struct SPixel
{
	cvec3 m_color;
};

class CImage
{
private:
	glm::ivec2 m_sizeImage;

	float m_fScale; // @ in rendering
	float m_fMarkerSize;
	bool m_bRenderMarker;

	SPixel **m_pImage;

	cvec3 m_byMColor; // @ Marker Color
	cvec3 m_byBgColor; // @ Background Color

	// moving renderer
	glm::ivec2 m_iMoving;
	glm::ivec2 m_iOffset;

public:
	CImage(glm::ivec2 _size, cvec3 _bgColor);
	~CImage();

	void Pulse();
	void Render();
	void Zoom(float y);
	
	void SetPixel(glm::vec2 _pos, glm::vec3 _color, bool scaling = true);
	SPixel GetPixel(glm::vec2 _pos) const { return m_pImage[(int)_pos.x][(int)_pos.y]; }

	SPixel** GetImage() const { return m_pImage; }
	glm::ivec2 GetImageSize() { return m_sizeImage; }

	void SetBgColor(cvec3 _bgColor);
	cvec3 GetBackgroundColor() { return m_byBgColor; }

	void SetMColor(cvec3 _color) { m_byMColor = _color; }
	cvec3 GetMColor() { return m_byMColor; }

	void ClearScreen();

	void ResetView()
	{
		m_fScale = 2;
		m_iMoving = glm::ivec2(0, 0);
		m_iOffset = glm::ivec2(0, 0);
	}
};

