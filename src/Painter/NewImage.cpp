#include "NewImage.h"

#include "Client.h"
#include "Renderer.h"
#include "ColorPicker.h"

glm::ivec2 CNewImage::m_iImageSize;
cvec3 CNewImage::m_byBgColor;
bool CNewImage::m_bAccept;

CNewImage::CNewImage()
{
	MSG msg;
	BOOL ret;

	m_hWnd = CreateDialogParam(CClient::Get()->GetHInstance(), MAKEINTRESOURCE(IDD_NEWIMAGE), CClient::Get()->GetRenderer()->GetHWnd(), DialogProc, 0);

	SetWindowTextA(GetDlgItem(m_hWnd, IDC_NWIDTH), "300");
	SetWindowTextA(GetDlgItem(m_hWnd, IDC_NHEIGHT), "200");

	SetWindowTextA(GetDlgItem(m_hWnd, IDC_NR), "255");
	SetWindowTextA(GetDlgItem(m_hWnd, IDC_NG), "255");
	SetWindowTextA(GetDlgItem(m_hWnd, IDC_NB), "255");

	ShowWindow(m_hWnd, SW_SHOW);

	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1)
			return;

		if (!IsDialogMessage(m_hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

CNewImage::~CNewImage()
{
}

int CALLBACK CNewImage::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
			DestroyWindow(hwndDlg);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDC_NBBGCOLOR:
				{
					CColorPicker colorpicker;

					if (colorpicker.Accepted())
					{
						char buffer[10] = "";

						sprintf(buffer, "%d", colorpicker.GetRGB().r);
						SetWindowTextA(GetDlgItem(hwndDlg, IDC_NR), buffer);
						sprintf(buffer, "%d", colorpicker.GetRGB().g);
						SetWindowTextA(GetDlgItem(hwndDlg, IDC_NG), buffer);
						sprintf(buffer, "%d", colorpicker.GetRGB().b);
						SetWindowTextA(GetDlgItem(hwndDlg, IDC_NB), buffer);
					}
				} break;
				case IDOK:
				{
					char buffer[10] = "";
					GetWindowTextA(GetDlgItem(hwndDlg, IDC_NWIDTH), buffer, 10);
					m_iImageSize.x = atoi(buffer);
					GetWindowTextA(GetDlgItem(hwndDlg, IDC_NHEIGHT), buffer, 10);
					m_iImageSize.y = atoi(buffer);

					GetWindowTextA(GetDlgItem(hwndDlg, IDC_NR), buffer, 10);
					m_byBgColor.r = atoi(buffer);
					GetWindowTextA(GetDlgItem(hwndDlg, IDC_NG), buffer, 10);
					m_byBgColor.g = atoi(buffer);
					GetWindowTextA(GetDlgItem(hwndDlg, IDC_NB), buffer, 10);
					m_byBgColor.b = atoi(buffer);

					m_bAccept = true;

					DestroyWindow(hwndDlg);
				} break;
				case IDCANCEL:
				{
					m_bAccept = false;

					DestroyWindow(hwndDlg);
				} break;
			}
		} break;
	}

	return 0;
}
