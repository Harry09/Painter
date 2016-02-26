#include "ImageMgr.h"

#include "Client.h"
#include "Renderer.h"
#include "Keyboard.h"
#include "FileBrowser.h"
#include "ColorPicker.h"

CImageMgr *CImageMgr::s_pInst;

CImageMgr::CImageMgr()
{
	s_pInst = this;
	m_pImage = 0;
}

CImageMgr::~CImageMgr()
{
	if (m_pImage)
		delete m_pImage;
}

CImage *CImageMgr::CreateImage(const glm::vec2 &_size, const cvec3 &_color)
{
	if (m_pImage)
		delete m_pImage;

	m_pImage = new CImage(_size, _color);

	return m_pImage;
}

void CImageMgr::Pulse()
{
	if (m_pImage)
		m_pImage->Pulse();

	if (CKeyboard::Get()->GetMode() == GLFW_MOD_CONTROL && CKeyboard::Get()->isPressed('S'))
	{
		CFileBrowser browser(DIALOG_SAVE, L"PK Image (*.pk)\0*.pk\0");

		if (browser.Accepted())
		{
			wchar_t _wsfileName[MAX_PATH] = { 0 };
			wcscpy(_wsfileName, browser.GetFileName());

			if (_wsfileName)
				Save(_wsfileName);
		}
	}

	if (CKeyboard::Get()->GetMode() == GLFW_MOD_CONTROL && CKeyboard::Get()->isPressed('O'))
	{
		CFileBrowser browser(DIALOG_OPEN, L"PK Image (*.pk)\0*.pk\0");

		if (browser.Accepted())
		{
			wchar_t _wsfileName[MAX_PATH] = { 0 };
			wcscpy(_wsfileName, browser.GetFileName());

			if (_wsfileName)
				Load(_wsfileName);
		}
	}
}

void CImageMgr::Render()
{
	if (m_pImage)
		m_pImage->Render();
}

void CImageMgr::Save(const wchar_t * _fileName)
{
	if (!m_pImage)
		return;

	CRenderer::Get()->SetText(0,0, L"Saving to \"%s\"...", _fileName);

	int _iStart = GetTickCount();

	FILE *_file = _wfopen(_fileName, L"wb");

	if (!_file)
	{
		MessageBoxA(0, "I can't open that file!\n", "ERORR!", MB_ICONERROR);
		return;
	}

	wprintf(L"Saving to file %s...\n", _fileName);

	// Sign
	fwrite("PK", sizeof(char), 2, _file);

	// Image size
	int _width = m_pImage->GetImageSize().x;
	int _height = m_pImage->GetImageSize().y;

	fwrite(&_width, sizeof(int), 1, _file);
	fwrite(&_height, sizeof(int), 1, _file);

	// Background color
	fwrite(&m_pImage->GetBackgroundColor(), sizeof(unsigned char), 3, _file);

	// Optmalization for loading
	unsigned int _uipixelCounter = 0;

	for (short x = 0; x < m_pImage->GetImageSize().x; ++x)
	{
		for (short y = 0; y < m_pImage->GetImageSize().y; ++y)
		{
			if (m_pImage->GetImage()[x][y].m_color != m_pImage->GetBackgroundColor())
				_uipixelCounter++;
		}
	}
	fwrite(&_uipixelCounter, sizeof(unsigned int), 1, _file);

	printf("Pixels = %d\n", _uipixelCounter);

	// Writing pixels to file
	for (short x = 0; x < _width; ++x)
	{
		for (short y = 0; y < _height; ++y)
		{
			if (m_pImage->GetImage()[x][y].m_color != m_pImage->GetBackgroundColor())
			{
				fwrite(&x, sizeof(short), 1, _file);
				fwrite(&y, sizeof(short), 1, _file);
				fwrite(&m_pImage->GetImage()[x][y].m_color, 1, 3, _file);
			}
		}
	}

	fseek(_file, 0, SEEK_END);
	int _iSize = ftell(_file);

	fclose(_file);

	wprintf(L"Saved to \"%s\" in %d ms File size = %d B\n", _fileName, (GetTickCount() - _iStart), _iSize);

	CRenderer::Get()->SetText(0, 2000, L"Image has been saved!");
}

void CImageMgr::Load(const wchar_t * _fileName)
{
	int _iStart = GetTickCount();

	CRenderer::Get()->SetText(0,0, L"Reading file %s...", _fileName);

	wprintf(L"Reading file %s...\n", _fileName);

	FILE * _file = _wfopen(_fileName, L"rb");

	if (!_file)
	{
		wprintf(L"I can't open %s\n", _fileName);
		return;
	}

	char _szSign[3] = { 0 };

	fread(_szSign, sizeof(char), 2, _file);

	_szSign[2] = 0;

	if (!strstr(_szSign, "PK"))
	{
		wprintf(L"\"%s\" is not pk file format!\n", _fileName);
		return;
	}

	// Image size
	int _width = 0;
	int _height = 0;

	fread(&_width, sizeof(int), 1, _file);
	fread(&_height, sizeof(int), 1, _file);


	// Background color
	cvec3 _bgColor;

	fread(&_bgColor, sizeof(unsigned char), 3, _file);

	// Creating image
	CreateImage(glm::vec2(_width, _height), _bgColor);

	unsigned int pixelCounter = 0;
	fread(&pixelCounter, sizeof(unsigned int), 1, _file);
	printf("Pixels = %d\n", pixelCounter);

	short _x = 0, _y = 0;

	for (unsigned i = 0; i < pixelCounter; ++i)
	{
		_x = 0; _y = 0;
		fread(&_x, sizeof(short), 1, _file);
		fread(&_y, sizeof(short), 1, _file);

		cvec3 _rgb;
		fread(&_rgb, 1, 3, _file);

		m_pImage->SetPixel(glm::vec2(_x, _y), _rgb, false);
	}
	
	fclose(_file);

	wprintf(L"Loaded file from \"%s\" in %d ms\n", _fileName, GetTickCount() - _iStart);
	
	CRenderer::Get()->SetText(0, 2000, L"File has been read!");
}

void CImageMgr::ExportTo(int _format, const wchar_t *_fileName)
{
	CRenderer::Get()->SetText(0,0, L"Exporting to \"%s\"...", _fileName);

	int cx = m_pImage->GetImageSize().x,
		cy = m_pImage->GetImageSize().y;

	BYTE* _bPixels = new BYTE[3 * cx * cy];

	int i = 0;

	for (int y = cy - 1; y >= 0; --y)
	{
		for (int x = 0; x < cx; ++x)
		{
			cvec3 _color = m_pImage->GetPixel(glm::vec2(x, y)).m_color;

			_bPixels[i] = _color.b;
			i++;
			_bPixels[i] = _color.g;
			i++;
			_bPixels[i] = _color.r;
			i++;
		}
	}

	FIBITMAP* image = FreeImage_ConvertFromRawBits(_bPixels, cx, cy, 3 * cx, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
	FreeImage_SaveU((FREE_IMAGE_FORMAT)_format, image, _fileName, 0);

	FreeImage_Unload(image);
	delete[] _bPixels;

	CRenderer::Get()->SetText(0, 2000, L"Image has been exported!");
}
