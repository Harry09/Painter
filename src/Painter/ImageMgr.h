#pragma once
#include "Prerequisites.h"
#include "Image.h"

enum ExportFormats
{
	BMP_FF = 0,
	ICO_FF = 1,
	JPEG_FF = 2,
	PNG_FF = 13
};

class CImageMgr
{
private:
	CImage* m_pImage;

public:
	CImageMgr();
	~CImageMgr();

	CImage* CreateImage(const glm::vec2 &_size, const cvec3 &_color);
	void DeleteImage() { delete m_pImage; }

	void Pulse();
	void Render();

	void Save(const wchar_t* _file);
	void Load(const wchar_t* _file);

	void ExportTo(int _format, const wchar_t *_file);

	CImage* GetImage() const { return m_pImage; }
};

