#pragma once
#ifndef XUGD_CLIB_APR_H
#define XUGD_CLIB_APR_H
#include "apr.h"
#include "apr_errno.h"
#include "../xugd.clib/xugd.exception.h"
using namespace xugd::clib;

#ifdef	XUGD_APR_EXPORT
#define XAPRAPI_DECLARE(_rv)	__declspec(dllexport) _rv __stdcall
#define XAPRCLASS_DECLARE		__declspec(dllexport)
#else
#define XAPRAPI_DECLARE(_rv)	__declspec(dllimport) _rv __stdcall
#define XAPRCLASS_DECLARE		__declspec(dllimport)
#endif

namespace xugd{ namespace apr{

//////////////////////////////////////////////////////////////////////////
// Macro for test APR-function return
#define AprSuccess(fun)	((fun)==APR_SUCCESS)
#define AprFail(fun)	((fun)!=APR_SUCCESS)

	//////////////////////////////////////////////////////////////////////////
	// Apr对应的异常基类
	class AprException:public XException
	{
	public:
		AprException(const char* szMsg_, apr_status_t nError_)
			: XException(szMsg_, (int)nError_)
		{ }
		~AprException(void) {}

		//////////////////////////////////////////////////////////////////////////
		// 获取错误码对应的信息
		const char* GetErrInfo(void)
		{
			static char szInfo[256] = {0};
			return apr_strerror((apr_status_t)GetErrCode(), szInfo, 256);
		}
	};

	class AprFileException:public AprException
	{
	public:
		AprFileException(const char* szMsg_, apr_status_t nError_, const char* szFile_)
			:AprException(szMsg_, nError_)
		{
			strcpy_s(m_szName, MAX_PATH, szFile_);
		}

		const char* GetFile(void) const
		{
			return m_szName;
		}

	private:
		char m_szName[MAX_PATH];
	};
} // apr
} // xugd
#endif // XUGD_CLIB_APR_H