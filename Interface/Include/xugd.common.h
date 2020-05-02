#pragma once
#ifndef XUGD_CLIB_COMMON_H
#define XUGD_CLIB_COMMON_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.common.h
// @DESCRIPTION: ����C++�г��õĲ����궨�壬�����·�ʽ���ÿ�
//	#ifdef _DEBUG
//	#pragma comment(lib, "xugd.clib_d.lib")
//	#else
//	#pragma comment(lib, "xugd.clib.lib")
//	#endif
// @VERSION: 1.0.20.502
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//	��д��Xugd��2020-05-02������C++11�������ơ�
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
// �������α�׼CԼ���ĵ�������
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

// ��ȡ����Ĵ�С����(sizeof(ary)/sizeof(ary[0])��ȣ�
// ֻ���������飨�ַ��������С������β0��������������ָ��
template<typename T, size_t N>
int CxArraySize(T (&ary_)[N])
{
	return N;
}

//////////////////////////////////////////////////////////////////////////
#define CxIsNull(_pCheck)		(nullptr == (_pCheck))
#define CxNotNull(_pCheck)		(nullptr != (_pCheck))
#define CxIsZero(_nCheck)		(0 == (_nCheck))
#define CxNotZero(_nCheck)		(0 != (_nCheck))
#define CxIsEqual(_nCheck, _nTarget)			((_nCheck) == (_nTarget))
#define CxNotEqual(_nCheck, _nTarget)			((_nCheck) != (_nTarget))
#define CxIsBetween(_nCheck, _nLower, _nUpper)	( ((_nCheck)>=(_nLower)) && ((_nCheck)<=(_nUpper)) )
#define CxNotBetween(_nCheck, _nLower, _nUpper)	( ((_nCheck)<(_nLower)) || ((_nCheck)>(_nUpper)) )

#define CxAstrNullOrEmpty(_pCheck)	((nullptr == (_pCheck)) || (0==strlen(_pCheck)))
#define CxWstrNullOrEmpty(_pCheck)	((nullptr == (_pCheck)) || (0==wcslen(_pCheck)))
#define CxAstrSetEmpty(_pStr)	(_pStr[0]='\0')
#define CxWstrSetEmpty(_pStr)	(_pStr[0]=L'\0')

#define CxZeroMemory(_pMem, _nSize)	memset(_pMem, 0, _nSize)

#define CxFieldOffset(_cType, _cField)	((int)(LONG_PTR)&(((_cType *)0)->_cField))

#define CxEnterForQuit()	\
do{	\
	std::cout << "Press Enter to quit: ";	\
	std::cin.get();		\
}while(0)

// ��ͨ���̳�<boost/noncopyable.hpp>�е�noncopyable����ֹ�ิ����copy����
// C++11��ʹ��delete�ؼ���
//#define CxForbidCopyFun(cName_)	\
//	cName_ & operator=(const cName_ &);	\
//	cName_(const cName_ &);

} //clib
} // xugd

#endif // XUGD_CLIB_COMMON_H