#pragma once
#include "Prerequisites.h"
#include "Image.h"

class CImageMgr
{
private:
	CImage* m_pImage;

public:
	CImageMgr();
	~CImageMgr();

	CImage* CreateImage(glm::vec2 _size, cvec3 _color);
	void DeleteImage() { delete m_pImage; }

	void Pulse();
	void Render();

	void Save(const wchar_t* _file);
	void Load(const wchar_t* _file);

	CImage* GetImage() const { return m_pImage; }
};

