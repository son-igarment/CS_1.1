#include "Main.h"
#include "SetupHooks.h"

char g_szBaseDir[256];
local_s g_Local;
SCREENINFO g_Screen;
GameInfo_s BuildInfo;
HINSTANCE hInstance;
cl_clientfunc_t *g_pClient = nullptr;
cl_clientfunc_t g_Client;
cl_enginefunc_t *g_pEngine = nullptr;
cl_enginefunc_t g_Engine;
engine_studio_api_t *g_pStudio = nullptr;
engine_studio_api_t g_Studio;
ofstream ofile;

string szDirFile( char* pszName )
{
	string szRet = g_szBaseDir;
	return (szRet + pszName);
}


void add_log(const char *fmt, ...)
{
	if(!fmt)
		return;
	va_list va_alist;
	char logbuf[256] = { 0 };
	va_start(va_alist, fmt);
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf)-strlen(logbuf), fmt, va_alist);
	va_end(va_alist);
	ofile << logbuf << endl;
}

BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	if( dwReason == DLL_PROCESS_ATTACH )
	{
		if ( GetLastError() != ERROR_ALREADY_EXISTS )
		{
			GetModuleFileName( hModule, g_szBaseDir, sizeof( g_szBaseDir ) );
			char* pos = g_szBaseDir + strlen( g_szBaseDir );
			while( pos >= g_szBaseDir && *pos!='\\' ) --pos; pos[ 1 ]=0;

			hInstance = hModule;
			DisableThreadLibraryCalls((HINSTANCE)hModule);

			char LogFile[1024];
			strcpy(LogFile, g_szBaseDir);
			strcat(LogFile, "\\server_log.txt");
			remove(LogFile);
			ofile.open(LogFile, ios::app);

			SetupHooks* Hook = new SetupHooks;
			Hook->StartThread(&SetupHooks::Initialize, 0);
			delete Hook;
		}
	}
	return TRUE;
}