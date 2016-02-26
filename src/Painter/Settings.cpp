#include "Settings.h"

#include "Client.h"
#include "Renderer.h"
#include "Menu.h"

CSettings *CSettings::s_pInst;

CSettings::CSettings()
{
	s_pInst = this;

	if (!FileExists("settings.pks"))
	{
		m_iResolution = glm::ivec2(800, 600);
		Save(false);
	}
	else
	{
		Load();
	}

	printf("CSettings initialized!\n");
}

CSettings::~CSettings()
{
}

void CSettings::ShowWindow()
{
	MSG msg;
	BOOL ret;

	m_hWnd = CreateDialogParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETTINGS), CRenderer::Get()->GetHWnd(), DialogProc, 0);

	Load();

	::ShowWindow(m_hWnd, SW_SHOW);

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

int CALLBACK CSettings::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				case SID_OK:
				{
					CSettings::Get()->Save();

					DestroyWindow(hwndDlg);
				} break;
				case SID_CANCEL:
				{
					DestroyWindow(hwndDlg);
				} break;
			}
		} break;
	}

	return 0;
}

void CSettings::Save(bool getDlgValue)
{
	if (getDlgValue)
	{
		char buffer[5] = "";

		GetWindowTextA(GetDlgItem(m_hWnd, IDC_WIDTH), buffer, 5);
		m_iResolution.x = atoi(buffer);

		GetWindowTextA(GetDlgItem(m_hWnd, IDC_HEIGHT), buffer, 5);
		m_iResolution.y = atoi(buffer);
	}

	FILE *_file = fopen("settings.pks", "wb");

	// sign
	fwrite("PKS", 1, 3, _file);

	// resolution
	fwrite(&m_iResolution, sizeof(glm::ivec2), 1, _file);

	fclose(_file);

	if (CClient::Get() && CRenderer::Get())
	{
		if (m_iResolution != CRenderer::Get()->GetWindowSize())
		{
			MessageBoxA(0, "Restart program", "Painter", MB_ICONINFORMATION);
		}
	}
}

void CSettings::Load()
{
	unsigned safeIterations = 10;
	FILE *file = nullptr;
	do {
		file = fopen("settings.pks", "rb");
	} while (--safeIterations && (!file));


	FILE *_file = fopen("settings.pks", "rb");

	// sign
	char _sign[3] = { 0 };

	fread(_sign, 1, 3, _file);

	if (!strstr(_sign, "PKS"))
		return;

	// resolution
	fread(&m_iResolution, sizeof(glm::ivec2), 1, _file);

	fclose(_file);

	
	char _sWidth[6] = "";
	
	// Width
	sprintf(_sWidth, "%d", m_iResolution.x);
	SetWindowTextA(GetDlgItem(m_hWnd, IDC_WIDTH), _sWidth);

	// Height
	char _sHeight[6] = "";
	
	sprintf(_sHeight, "%d", m_iResolution.y);
	SetWindowTextA(GetDlgItem(m_hWnd, IDC_HEIGHT), _sHeight);
}