#pragma once
#ifndef XUGD_CLIB_STR_H
#define XUGD_CLIB_STR_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.str.h
// @DESCRIPTION: 定义字符串相关操作与类
// @VERSION: 1.0.20.502
//
// @RECORD:
//	编写：Xugd，2020-05-03，初始版本。
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// 字符串操作
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