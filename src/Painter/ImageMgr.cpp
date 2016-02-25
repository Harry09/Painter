#include "ImageMgr.h"

#include "Client.h"
#include "Renderer.h"
#include "Keyboard.h"
#include "FileBrowser.h"
#include "ColorPicker.h"

CImageMgr::CImageMgr()
{
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

	if (CClient::Get()->GetKeyboard()->GetMode() == GLFW_MOD_CONTROL && CClient::Get()->GetKeyboard()->isPressed('S'))
	{
		CFileBrowser browser(DIALOG_SAVE, L"PK Image (*.pk)\0*.pk\0");

		if (browser.Accepted())
		{
			wchar_t _fileName[MAX_PATH] = { 0 };
			wcscpy(_fileName, browser.GetFileName());
			int _lenFN = wcslen(_fileName);

			if (_fileName[_lenFN-1] != 'k' && _fileName[_lenFN-2] != 'p' && _fileName[_lenFN-3] != '.')
			{
				
				wcscat(_fileName, L".pk");
			}

			Save(_fileName);
		}
	}

	if (CClient::Get()->GetKeyboard()->GetMode() == GLFW_MOD_CONTROL && CClient::Get()->GetKeyboard()->isPressed('O'))
	{
		CFileBrowser browser(DIALOG_OPEN, L"PK Image (*.pk)\0*.pk\0");

		if (browser.Accepted())
		{
			wchar_t _fileName[MAX_PATH] = { 0 };
			wcscpy(_fileName, browser.GetFileName());
			int _lenFN = wcslen(_fileName);

			if (_fileName[_lenFN - 1] != 'k' && _fileName[_lenFN - 2] != 'p' && _fileName[_lenFN - 3] != '.')
			{
				wcscat(_fileName, L".pk");
			}

			Load(_fileName);
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

	CClient::Get()->GetRenderer()->SetText(0, L"Saving to \"%s\"...", _fileName);

	int iStart = GetTickCount();

	FILE * file = _wfopen(_fileName, L"wb");

	if (!file)
	{
		MessageBoxA(0, "I can't open that file!\n", "ERORR!", MB_ICONERROR);
		return;
	}

	wprintf(L"Saving to file %s...\n", _fileName);

	// Sign
	fwrite("PK", sizeof(char), 2, file);

	// Image size
	int _width = m_pImage->GetImageSize().x;
	int _height = m_pImage->GetImageSize().y;

	fwrite(&_width, sizeof(int), 1, file);
	fwrite(&_height, sizeof(int), 1, file);

	// Background color
	fwrite(&m_pImage->GetBackgroundColor(), sizeof(unsigned char), 3, file);

	// Optmalization for loading
	unsigned int pixelCounter = 0;

	for (short x = 0; x < m_pImage->GetImageSize().x; ++x)
	{
		for (short y = 0; y < m_pImage->GetImageSize().y; ++y)
		{
			if (m_pImage->GetImage()[x][y].m_color != m_pImage->GetBackgroundColor())
				pixelCounter++;
		}
	}
	fwrite(&pixelCounter, sizeof(unsigned int), 1, file);

	printf("Pixels = %d\n", pixelCounter);

	// Writing pixels to file
	for (short x = 0; x < _width; ++x)
	{
		for (short y = 0; y < _height; ++y)
		{
			if (m_pImage->GetImage()[x][y].m_color != m_pImage->GetBackgroundColor())
			{
				fwrite(&x, sizeof(short), 1, file);
				fwrite(&y, sizeof(short), 1, file);
				fwrite(&m_pImage->GetImage()[x][y].m_color, 1, 3, file);
			}
		}
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);

	fclose(file);

	wprintf(L"Saved to \"%s\" in %d ms File size = %d B\n", _fileName, (GetTickCount() - iStart), size);

	CClient::Get()->GetRenderer()->SetText(2000, L"Image has been saved!");
}

void CImageMgr::Load(const wchar_t * _fileName)
{
	int iStart = GetTickCount();

	CClient::Get()->GetRenderer()->SetText(0, L"Reading file %s...", _fileName);

	wprintf(L"Reading file %s...\n", _fileName);

	FILE * file = _wfopen(_fileName, L"rb");

	if (!file)
	{
		wprintf(L"I can't open %s\n", _fileName);
		return;
	}

	char _sign[3] = { 0 };

	fread(_sign, sizeof(char), 2, file);

	_sign[2] = 0;

	if (!strstr(_sign, "PK"))
	{
		wprintf(L"\"%s\" is not pk file format!\n", _fileName);
		return;
	}

	// Image size
	int _width = 0;
	int _height = 0;

	fread(&_width, sizeof(int), 1, file);
	fread(&_height, sizeof(int), 1, file);


	// Background color
	cvec3 _bgColor;

	fread(&_bgColor, sizeof(unsigned char), 3, file);

	// Creating image
	CreateImage(glm::vec2(_width, _height), _bgColor);

	unsigned int pixelCounter = 0;
	fread(&pixelCounter, sizeof(unsigned int), 1, file);
	printf("Pixels = %d\n", pixelCounter);

	short _x = 0, _y = 0;

	for (unsigned i = 0; i < pixelCounter; ++i)
	{
		_x = 0; _y = 0;
		fread(&_x, sizeof(short), 1, file);
		fread(&_y, sizeof(short), 1, file);

		cvec3 _rgb;
		fread(&_rgb, 1, 3, file);

		m_pImage->SetPixel(glm::vec2(_x, _y), _rgb, false);
	}
	
	fclose(file);

	wprintf(L"Loaded file from \"%s\" in %d ms\n", _fileName, GetTickCount() - iStart);
	
	CClient::Get()->GetRenderer()->SetText(2000, L"File has been read!");
}

void CImageMgr::ExportTo(int _format, const wchar_t *_fileName)
{
	CClient::Get()->GetRenderer()->SetText(0, L"Exporting to \"%s\"...", _fileName);

	int cx = m_pImage->GetImageSize().x,
		cy = m_pImage->GetImageSize().y;

	BYTE* pixels = new BYTE[3 * cx * cy];

	int i = 0;

	for (int y = cy - 1; y >= 0; --y)
	{
		for (int x = 0; x < cx; ++x)
		{
			cvec3 _color = m_pImage->GetPixel(glm::vec2(x, y)).m_color;

			pixels[i] = _color.b;
			i++;
			pixels[i] = _color.g;
			i++;
			pixels[i] = _color.r;
			i++;
		}
	}

	FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, cx, cy, 3 * cx, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
	FreeImage_SaveU((FREE_IMAGE_FORMAT)_format, image, _fileName, 0);

	FreeImage_Unload(image);
	delete[] pixels;

	CClient::Get()->GetRenderer()->SetText(2000, L"Image has been exported!");
}
