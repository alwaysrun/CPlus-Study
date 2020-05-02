// ExceptTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <dbghelp.h>     
#include <shellapi.h>     
#include <shlobj.h>
#include <iostream>
#include "xugd.thread.h"

#include "../../xugd.winlib/xugd.winlib.h"

#pragma comment(lib, "xugd.winlib.lib")
#pragma comment(lib, "xugd.clib.lib")

//#pragma comment( lib, "DbgHelp")  
//
//static LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)  
//{  
//	HANDLE lhDumpFile = CreateFile(L"DumpFile.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);  
//	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;  
//	loExceptionInfo.ExceptionPointers = ExceptionInfo;  
//	loExceptionInfo.ThreadId = GetCurrentThreadId();  
//	loExceptionInfo.ClientPointers = FALSE;  
//
//	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);  
//
//	CloseHandle(lhDumpFile);  
//
//	//::ShellExecuteW(NULL, NULL, L"BugReport.exe", NULL, NULL, SW_NORMAL);  
//
//	return EXCEPTION_EXECUTE_HANDLER;  
//}  

void ThrowError()
{
	int nTest = 0;
	int nCount = 1/nTest;
}

void GetError()
{
	printf("To get error");

	xugd::clib::XThread::Sleep(1000);
	ThrowError();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	xugd::winlib::XDumpUnhanleException();

	printf("Set Unhandle OK\n");

	int nTest = 0;
	int nCount = 1/nTest;
	char *pTest = NULL;
	*pTest = '1';
	GetError();

	printf("Press any key:");
	getchar();
	return 0;
}

