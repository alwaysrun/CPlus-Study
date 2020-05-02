#pragma once
#ifndef XUGD_CLIB_CONVERT_H
#define XUGD_CLIB_CONVERT_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.convert.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义类型转换类与接口
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <sstream>

namespace xugd{	namespace clib{
	using namespace std;

	// 字符串数字转换（使用Wstr2Type）
	// 把字符串转换为浮点数。没有转换返回0，如果溢出返回+/–HUGE_VAL
//#define CxWstrTof(wzStr_)	wcstod(wzStr_, NULL)
//	// 把字符串转换为长整数。
//#define CxWstrTol(wzStr_)	wcstol(wzStr_, NULL, 10)
//#define CxWstrToul(wzStr_)	wcstoul(wzStr_, NULL, 10)
//
//#define CxWstrTox(wzStr_)	wcstoul(wzStr_, NULL, 16)

	//////////////////////////////////////////////////////////////////////////
	// 通过包含<sstream>，使用ostringstream序列化为窄字符；
	// 使用owstringstream序列化为宽字符串。
	class XCLIBCLASS_DECLARE XConvert{
	public:
		// convert UTF-8 string to wstring
		static wstring Astr2W(const std::string& str);
		// convert wstring to UTF-8 string
		static string Wstr2A(const std::wstring& str);

		// 十六进制与字节序列间转换；string中的字节序列可通过string.data获取
		static string WHex2Bytes(const wstring &strHex_);
		static string AHex2Bytes(const string &strHex_);

		static wstring Bytes2WHex(const char *pByte_, int nSize_);
		static string Bytes2AHex(const char *pByte_, int nSize_);
		static string Bytes2AHex(const string &strBytes_){
			return Bytes2AHex(strBytes_.data(), strBytes_.size());
		}

		static string Bytes2Base64Str(const char *pByte_, int nSize_);
		static string Base64Str2Bytes(const string &strBase64_);
		static string Bytes2Base64Str(const string &strBytes_){
			return Bytes2Base64Str(strBytes_.data(), strBytes_.size());
		}

		//////////////////////////////////////////////////////////////////////////
		// 数据与字符串间的转换
		// <boost/lexical_cast.hpp>可实现支持输入输出流类型与字符串间的互相转换（出错会抛出bad_lexical_cast异常）
		template<typename in_type>
		static string Type2Astr(const in_type& tValue_)
		{
			ostringstream oStream;
			oStream<<tValue_;
			return oStream.str();
		}
		// 数字如果溢出会出错；如long long(64bit)类型字符串获取int(32bit)数字，应Astr2Type<long long>(...) & 0xFFFFFFFF
		template<typename out_type>
		static out_type Astr2Type(const string &strIn_)
		{
			return Convert<out_type>(strIn_);
		}

		//  转换为数字，如果是非数字，则返回零。
		// 数字开始，混杂其他字符的，只返回前面数字
		static int Astr2Int(const string &strIn_);
		static string Int2Astr(unsigned int nNumber_, unsigned int nDigits_=0); // nDigits为最少位数，如果少于指定位数前面补零
		static float Astr2Float(const string &strIn_);

		// 数字与十六进制字符串转换
		static int AstrHex2Int(const string &strIn_);
		static string Int2AstrHex(int nHex_);

		template<typename in_type>
		static wstring Type2Wstr(const in_type& tValue_)
		{
			wostringstream oStream;
			oStream<<tValue_;
			return oStream.str();
		}
		template<typename out_type>
		static out_type Wstr2Type(const wstring &strIn_)
		{
			wstringstream strStream;
			strStream<<strIn_;

			out_type outResult;
			strStream>>outResult;
			return outResult;
		}

		//////////////////////////////////////////////////////////////////////////
		// 转换数据类型，如字符串转换为浮点型：
		// double d;
		// string s="12.56";
		// d=convert<double>(s)
		template<typename out_type,typename in_type>
		static out_type Convert(const in_type& tValue_)
		{
			stringstream strStream;
			strStream<<tValue_;

			out_type result;
			strStream>>result;
			return result;
		}
	};
} //clib
} // xugd
#endif // XUGD_CLIB_CONVERT_H