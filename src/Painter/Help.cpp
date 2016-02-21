#include "Help.h"

#include "Client.h"
#include "Renderer.h"

CHelp::CHelp()
{
	MSG msg;
	BOOL ret;

	m_hWnd = CreateDialogParam(CClient::Get()->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOGHELP), CClient::Get()->GetRenderer()->GetHWnd(), DialogProc, 0);

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

CHelp::~CHelp()
{
}

int CALLBACK CHelp::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			case IDOK:
			{
				DestroyWindow(hwndDlg);
			} break;
		}
	} break;
	}

	return 0;
}
