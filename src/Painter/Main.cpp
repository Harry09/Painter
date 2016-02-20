#include "Client.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

bool FileExists(char* name)
{
	if (FILE *file = fopen(name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LPWSTR *szArglist;
	int nArgs;

	wchar_t file[MAX_PATH] = L"";

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
		printf("Cannot read command line!\n");
	else if (nArgs > 1)
		wcscpy(file, szArglist[1]);

	new CClient(hInstance, file);

	LocalFree(szArglist);


	return 0;
}