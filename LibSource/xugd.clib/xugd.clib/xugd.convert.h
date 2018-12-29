#pragma once
#ifndef XUGD_CLIB_CONVERT_H
#define XUGD_CLIB_CONVERT_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.convert.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: ��������ת������ӿ�
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <sstream>

namespace xugd{	namespace clib{
	using namespace std;

	// �ַ�������ת����ʹ��Wstr2Type��
	// ���ַ���ת��Ϊ��������û��ת������0������������+/�CHUGE_VAL
//#define CxWstrTof(wzStr_)	wcstod(wzStr_, NULL)
//	// ���ַ���ת��Ϊ��������
//#define CxWstrTol(wzStr_)	wcstol(wzStr_, NULL, 10)
//#define CxWstrToul(wzStr_)	wcstoul(wzStr_, NULL, 10)
//
//#define CxWstrTox(wzStr_)	wcstoul(wzStr_, NULL, 16)

	//////////////////////////////////////////////////////////////////////////
	// ͨ������<sstream>��ʹ��ostringstream���л�Ϊխ�ַ���
	// ʹ��owstringstream���л�Ϊ���ַ�����
	class XCLIBCLASS_DECLARE XConvert{
	public:
		// convert UTF-8 string to wstring
		static wstring Astr2W(const std::string& str);
		// convert wstring to UTF-8 string
		static string Wstr2A(const std::wstring& str);

		// ʮ���������ֽ����м�ת����string�е��ֽ����п�ͨ��string.data��ȡ
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
		// �������ַ������ת��
		// <boost/lexical_cast.hpp>��ʵ��֧������������������ַ�����Ļ���ת����������׳�bad_lexical_cast�쳣��
		template<typename in_type>
		static string Type2Astr(const in_type& tValue_)
		{
			ostringstream oStream;
			oStream<<tValue_;
			return oStream.str();
		}
		// �����������������long long(64bit)�����ַ�����ȡint(32bit)���֣�ӦAstr2Type<long long>(...) & 0xFFFFFFFF
		template<typename out_type>
		static out_type Astr2Type(const string &strIn_)
		{
			return Convert<out_type>(strIn_);
		}

		//  ת��Ϊ���֣�����Ƿ����֣��򷵻��㡣
		// ���ֿ�ʼ�����������ַ��ģ�ֻ����ǰ������
		static int Astr2Int(const string &strIn_);
		static string Int2Astr(unsigned int nNumber_, unsigned int nDigits_=0); // nDigitsΪ����λ�����������ָ��λ��ǰ�油��
		static float Astr2Float(const string &strIn_);

		// ������ʮ�������ַ���ת��
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
		// ת���������ͣ����ַ���ת��Ϊ�����ͣ�
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