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
	if (FILE *file = fopen(name, "r")) 
	{
		fclose(file);
		return true;
	}
	else
		return false;
}

const wchar_t *getExt(const wchar_t *fspec) {
	wchar_t *e = wcsrchr((wchar_t *)fspec, '.');

	if (e == NULL)
		e = L"";

	return e;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LPWSTR *szArglist;
	int nArgs;

	wchar_t file[MAX_PATH] = L"";

#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
#endif

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
		printf("Cannot read command line!\n");
	else if (nArgs > 1)
		wcscpy(file, szArglist[1]);

	//int argc = __argc;
	//char ** argv = __argv;


	new CClient(hInstance, file);

	LocalFree(szArglist);

	return 0;
}