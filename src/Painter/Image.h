#pragma once

#include "Prerequisites.h"

struct SPixel
{
	cvec3 m_color;
};

class CImage
{
private:
	glm::ivec2 m_iSizeImage;

	float m_fMarkerSize;
	bool m_bRenderMarker;

	SPixel **m_pImage;

	cvec3 m_byMColor; // @ Marker Color
	cvec3 m_byBgColor; // @ Background Color

	// Line and Quad
	glm::vec2 m_fPos1;
	glm::vec2 m_fPos2;
	char m_inClick; // int (one byte) number Click

	// Circle
	float m_fCircleRadius;


public:
	CImage(const glm::ivec2 &_size, const cvec3 &_bgColor);
	~CImage();

	void Pulse();
	void Render();
	
	void SetPixel(const glm::vec2 &_pos, const glm::vec3 &_color, bool _scaling = true);
	SPixel GetPixel(const glm::vec2 &_pos) const { return m_pImage[(int)_pos.x][(int)_pos.y]; }

	SPixel** GetImage() const { return m_pImage; }
	glm::ivec2 GetImageSize() { return m_iSizeImage; }

	void SetBgColor(const cvec3 &_bgColor);
	cvec3 GetBackgroundColor() { return m_byBgColor; }

	void SetMColor(cvec3 _color) { m_byMColor = _color; }
	cvec3 GetMColor() { return m_byMColor; }

	void ClearScreen();

	cvec3 GetColor(const glm::vec2 &_pos, bool _scaling = true);
};

