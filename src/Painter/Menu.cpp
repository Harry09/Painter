#include "Menu.h"

#include "Client.h"
#include "Renderer.h"
#include "ImageMgr.h"
#include "Image.h"
#include "ColorPicker.h"
#include "FileBrowser.h"
#include "Settings.h"
#include "Help.h"
#include "NewImage.h"
#include "View.h"

HWND CMenu::s_hWnd;

char CMenu::m_sMode[4][12] = { "Draw Line", "Draw Quad", "Draw Circle", "Draw Pixel" };
char CMenu::m_iMode;
bool CMenu::m_bGetColor;

CMenu *CMenu::s_pInst;

CMenu::CMenu()
{
	s_pInst = 0;

	s_hWnd = CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MENU), CRenderer::Get()->GetHWnd(), DialogProc, 0);

	printf("CMenu initialized!\n");
}

CMenu::~CMenu()
{
}

void CMenu::ShowWindow()
{
	CreateThread(0, 0, &Window, 0, 0, 0);
}

DWORD WINAPI CMenu::Window(LPVOID)
{
	MSG msg;
	BOOL ret;

	::ShowWindow(s_hWnd, SW_SHOW);

	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1)
			return 1;

		if (!IsDialogMessage(s_hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

int CALLBACK CMenu::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
			DestroyWindow(CRenderer::Get()->GetHWnd());
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDC_MCOLOR:
				{
					CColorPicker cpicker;

					if (cpicker.Accepted())
					{
						CImageMgr::Get()->GetImage()->SetMColor(cpicker.GetRGB());
					}
				} break;
				case IDC_BGCOLOR:
				{
					CColorPicker cpicker;

					if (cpicker.Accepted())
					{
						CImageMgr::Get()->GetImage()->SetBgColor(cpicker.GetRGB());
					}
				} break;
				case IDC_ISAVE:
				{
					CFileBrowser browser(DIALOG_SAVE, L"Project PK (*.pk)\0*.pk\0");

					if (browser.Accepted())
					{
						wchar_t _wsfileName[MAX_PATH] = { 0 };
						wcscpy(_wsfileName, browser.GetFileName());
						if (_wsfileName)
							CImageMgr::Get()->Save(_wsfileName);
					}
				} break;
				case IDC_ILOAD:
				{
					CFileBrowser browser(DIALOG_OPEN, L"PK Image (*.pk)\0*.pk\0");

					if (browser.Accepted())
					{
						wchar_t _wsfileName[MAX_PATH] = { 0 };
						wcscpy(_wsfileName, browser.GetFileName());
						if (_wsfileName)
							CImageMgr::Get()->Load(_wsfileName);
					}
				} break;
				case IDC_INEW:
				{
					CNewImage newImage;
					
					if (newImage.Accepted())
						CImageMgr::Get()->CreateImage(newImage.GetSize(), newImage.GetBgColor());
				} break;
				case IDC_ICLEAR:
				{
					CImageMgr::Get()->GetImage()->ClearScreen();
				} break;
				case IDC_RESETVIEW:
				{
					CView::Get()->ResetView();
				} break;
				case IDC_DRAWINGMODE:
				{
					SetWindowTextA(GetDlgItem(hwndDlg, IDC_DRAWINGMODE), m_sMode[m_iMode]);
				
					m_iMode++;
				
					if (m_iMode > 3)
						m_iMode = 0;

					if (m_iMode == 1 || m_iMode == 2)
						CRenderer::Get()->SetText(0,0, L"First click somewhere on image to set first position");
					else
						CRenderer::Get()->SetText(0,0, L"Ready");

				} break;
				case IDC_GETCOLOR:
				{
					printf("Getting color...\n");
					m_bGetColor = true;

					CRenderer::Get()->SetText(0,0, L"Just click somewhere to get color. Color will be copied to clipboard");
				} break;
				case IDC_EXPORT:
				{
					CFileBrowser browser(DIALOG_SAVE, L"Image Bitmap (*.bmp)\0*.bmp\0Icon (*.ico)\0*.ico\0Image JPG/JPEG (*.jpg)\0*.jpg\0Image PNG (*.png)\0*.png\0");

					if (browser.Accepted())
					{
						wchar_t _wsfileName[MAX_PATH] = { 0 };
						wcscpy(_wsfileName, browser.GetFileName());

						if (!wcscmp(getExt(_wsfileName), L".bmp"))
							CImageMgr::Get()->ExportTo(BMP_FF, _wsfileName);
						if (!wcscmp(getExt(_wsfileName), L".ico"))
							CImageMgr::Get()->ExportTo(ICO_FF, _wsfileName);
						if (!wcscmp(getExt(_wsfileName), L".jpg") || !wcscmp(getExt(_wsfileName), L".jpeg"))
							CImageMgr::Get()->ExportTo(JPEG_FF, _wsfileName);
						if (!wcscmp(getExt(_wsfileName), L".png"))
							CImageMgr::Get()->ExportTo(PNG_FF, _wsfileName);
					}

				} break;
				case IDC_SETTINGS:
				{
					CSettings::Get()->ShowWindow();
				} break;
				case IDC_SHOWHELP:
				{
					CHelp help;
				} break;
			}

			// Auto focus
			SetFocus(CRenderer::Get()->GetHWnd());
		} break;
	}

	return 0;
}
