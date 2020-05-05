#pragma once
#ifndef XUGD_CLIB_STR_H
#define XUGD_CLIB_STR_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.str.h
// @DESCRIPTION: �����ַ�����ز�������
// @VERSION: 1.0.20.502
//
// @RECORD:
//	��д��Xugd��2020-05-03����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// �ַ�������
	//////////////////////////////////////////////////////////////////////////
	class XuStr{
	public:
		static std::string int2Str(int nNumber);
		static std::string uint2Str(unsigned int nNumber);
		static std::string double2Str(double fNumber);

		static int str2Int(const std::string &strNumber);
		static unsigned int str2Uint(const std::string &strNumber);
		static double str2Double(const std::string &strNumber);

		static std::vector<std::string> split(const std::string &strTotal, bool bDiscardEmpty_=true);
		static std::vector<std::string> split(const std::string &strTotal, char chDelim, bool bDiscardEmpty_ = true);

	};

} //clib
} // xugd

#endif	//XUGD_CLIB_STR_H