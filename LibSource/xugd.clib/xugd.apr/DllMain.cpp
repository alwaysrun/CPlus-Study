#define WIN32_LEAN_AND_MEAN		//  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include "xugd.private.h"

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		xugd::apr::AprDllStart();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		xugd::apr::AprDllStop();
		break;
	}
	return TRUE;
}