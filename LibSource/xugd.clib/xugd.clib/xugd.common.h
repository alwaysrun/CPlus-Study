#pragma once
#ifndef XUGD_CLIB_COMMON_H
#define XUGD_CLIB_COMMON_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.common.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义C++中常用的操作宏定义，安如下方式引用库
//	#ifdef _DEBUG
//	#pragma comment(lib, "xugd.clib_d.lib")
//	#else
//	#pragma comment(lib, "xugd.clib.lib")
//	#endif
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////

#include <iostream>

// export c-function
//#ifdef __cplusplus  
//	extern "C" {  
//#endif 
//
//#ifdef __cplusplus  
//	}  
//#endif

// 用于修饰标准C约定的导出函数
// XUGD_BEGIN_CAPI_DECLARE
//  void MyFunctions(...);
//	  ....
// XUGD_END_CAPI_DECLARE
#ifdef __cplusplus
#define XUGD_BEGIN_CAPI_DECLARE		extern "C" {
#define XUGD_END_CAPI_DECLARE		}
#else
#define XUGD_BEGIN_CAPI_DECLARE
#define XUGD_END_CAPI_DECLARE
#endif

namespace xugd{	namespace clib{

#ifdef	XUGD_CLIB_EXPORT
#define XCLIBAPI_DECLARE(_rv)	__declspec(dllexport) _rv __stdcall
#define XCLIBCLASS_DECLARE		__declspec(dllexport)
#else
#define XCLIBAPI_DECLARE(_rv)	__declspec(dllimport) _rv __stdcall
#define XCLIBCLASS_DECLARE		__declspec(dllimport)
#endif

// 可通过继承<boost/noncopyable.hpp>中的noncopyable来禁止类复制与copy构造
#define CxForbidCopyFun(cName_)	\
	cName_ & operator=(const cName_ &);	\
	cName_(const cName_ &);

// 获取数组的大小，与(sizeof(ary)/sizeof(ary[0])相等；
// 只能用于数组（字符串数组大小包括结尾0），不能作用于指针
template<typename T, size_t N>
int CxArraySize(T (&ary_)[N])
{
	return N;
}

	//////////////////////////////////////////////////////////////////////////
#define CxIsNull(_pCheck)		(NULL == (_pCheck))
#define CxNotNull(_pCheck)		(NULL != (_pCheck))
#define CxIsZero(_nCheck)		(0 == (_nCheck))
#define CxNotZero(_nCheck)		(0 != (_nCheck))
#define CxIsEqual(_nCheck, _nTarget)			((_nCheck) == (_nTarget))
#define CxNotEqual(_nCheck, _nTarget)			((_nCheck) != (_nTarget))
#define CxIsBetween(_nCheck, _nLower, _nUpper)	( ((_nCheck)>=(_nLower)) && ((_nCheck)<=(_nUpper)) )
#define CxNotBetween(_nCheck, _nLower, _nUpper)	( ((_nCheck)<(_nLower)) || ((_nCheck)>(_nUpper)) )

#define CxAstrNullOrEmpty(_pCheck)	((NULL == (_pCheck)) || (0==strlen(_pCheck)))
#define CxWstrNullOrEmpty(_pCheck)	((NULL == (_pCheck)) || (0==wcslen(_pCheck)))
#define CxAstrSetEmpty(_pStr)	(_pStr[0]='\0')
#define CxWstrSetEmpty(_pStr)	(_pStr[0]=L'\0')

#define CxZeroMemory(_pMem, _nSize)	memset(_pMem, 0, _nSize)

#define CxFieldOffset(_cType, _cField)	((int)(LONG_PTR)&(((_cType *)0)->_cField))

} //clib
} // xugd

#endif // XUGD_CLIB_COMMON_H