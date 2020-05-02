#pragma once
#ifndef XUGD_CLIB_STR_H
#define XUGD_CLIB_STR_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.str.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: �����ַ������ַ�ָ������ӿ���
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <vector>

// �Ƽ�ʹ��<boost/algorithm/string.hpp>�е�string_algo��������(boost::fxxx)��
// ���ں���fxxx��
//		�漰����Сд�Ƚ϶���Ӧ��ifxxx��
//		�漰���޸ĵĶ���fxxx_copy�����޸�ֵ������ֱ���޸ģ�
//		�����ν�ʣ��Ƚ�����������fxxx_if������copy�����Ϊfxxx_copy_if��
// ��Сдת����to_upper��to_lower��
// ��ȡ��trim��trim_left��trim_right��
// �жϰ�����starts_with��ends_with��contains��
// ��ȣ�equals��
// �����жϣ�bool all(Input, Pred)��unspecified is_any_of(Set)��nspecified is_from_range(From, To);;
// ����жϣ�is_classified��is_space��is_alnum ��is_alpha��is_cntrl ��is_digit ��is_graph 
//			is_lower ��is_print ��is_punct ��is_upper ��is_xdigit ��
// ɾ����erase_all ��erase_first��erase_last ��erase_nth��ɾ��ָ���Ǹ����� erase_range 
//		��ɾ��ָ�������ַ���erase_head ��erase_tail 
// �滻��replace_first ��replace_last ��replace_nth ��replace_all
//		replace_head ��replace_tail 
// ���ң�find��find_first��find_last��find_nth��find_head��find_tail��find_token

namespace xugd{	namespace clib{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// �Ƴ�ָ���ַ�����Trimed="\\/"���Ƴ�����·���ָ���
	XCLIBAPI_DECLARE(void) XWstrTrim(wstring &strInput_, const wchar_t *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XWstrTrimStart(wstring &strInput_, const wchar_t *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XWstrTrimEnd(wstring &strInput_, const wchar_t *pTrimed_=NULL);

	// �Ƚ�
	typedef bool (*XWstrCmpFun)(wchar_t fr_, wchar_t se_);
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_);
	
	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_);
	
	XCLIBAPI_DECLARE(bool) XWstrContain(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrEqual(const wstring &strFirst_, const wstring &strSecond_, bool bIngoreCase_=false);

	// ��ȡ�Ӵ������Len<0����˵����β����ȡ�������ȡͷ��
	XCLIBAPI_DECLARE(wstring) XWstrSubStr(const wstring &strInput_, int nLen_);

	// ����ָ������n�������Ӵ����ҵ����ص�n���Ӵ���λ�ã����򷵻�wstring::npos
	XCLIBAPI_DECLARE(wstring::size_type) XAstrFindNth(const wstring &strInput_, const wstring &strTofind_, int nNth_, wstring::size_type nStart_=0);
	
	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XWptrCountEqual(const wchar_t *pFirst_, const wchar_t *pSecond_, size_t nCount_, bool bIgnorCase_=false);

	//////////////////////////////////////////////////////////////////////////
	// �Ƴ�ָ���ַ�����Trimed="\\/"���Ƴ�����·���ָ�����Ĭ���Ƴ��հ�
	XCLIBAPI_DECLARE(void) XAstrTrim(string &strInput_, const char *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XAstrTrimStart(string &strInput_, const char *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XAstrTrimEnd(string &strInput_, const char *pTrimed_=NULL);

	// �Ƚ�
	typedef bool (*XAstrCmpFun)(char fr_, char se_);
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_);

	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_);

	XCLIBAPI_DECLARE(bool) XAstrContain(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrEqual(const string &strFirst_, const string &strSecond_, bool bIngoreCase_=false);

	// ת��
	XCLIBAPI_DECLARE(string) XAstrToLower(const string &strInput_);
	XCLIBAPI_DECLARE(string) XAstrToUpper(const string &strInput_);
	
	// �ָ���strSeperators��������ַ�������һ�ַ���Ϊ�ָ���
	XCLIBAPI_DECLARE(std::vector<string>) XAstrSplit(const string &strInput_, const char *strSeperators_, bool bDropEmpty_);
	XCLIBAPI_DECLARE(string) XAstrJoin(const std::vector<string> &strInput_, const char *strSeperator_);

	XCLIBAPI_DECLARE(string) XAstrReplaceFirst(const string &strInput_, const string &strOld_, const string &strNew_);
	XCLIBAPI_DECLARE(string) XAstrReplaceAll(const string &strInput_, const string &strOld_, const string &strNew_);

	// ��ȡ�Ӵ������Len<0����˵����β����ȡ�������ȡͷ��
	XCLIBAPI_DECLARE(string) XAstrSubStr(const string &strInput_, int nLen_);

	// ����ָ������n�������Ӵ����ҵ����ص�n���Ӵ���λ�ã����򷵻�string::npos
	XCLIBAPI_DECLARE(string::size_type) XAstrFindNth(const string &strInput_, const string &strTofind_, int nNth_, string::size_type nStart_=0);

	//////////////////////////////////////////////////////////////////////////
	// PChar
	XCLIBAPI_DECLARE(bool) XAptrEqual(const char *pFirst_, const char *pSecond_, bool bIgnorCase_=false);
	XCLIBAPI_DECLARE(bool) XAptrCountEqual(const char *pFirst_, const char *pSecond_, size_t nCount_, bool bIgnorCase_=false);

	// ���Ҳ�������NULL�����򷵻��Ӵ���ʼλ��
	XCLIBAPI_DECLARE(const char*) XAptrFindFirst(const char *strInput_, const char *strSub_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(const char*) XAptrFindLast(const char *strInput_, const char *strSub_, bool bIngoreCase_=false);

	// ��ȡĩβָ�����ȵ��Ӵ��������Ӵ���ʼλ�ã����ָ�����ȳ����ַ������ȣ��򷵻��ַ�������
	//XCLIBAPI_DECLARE(const char*) XAptrGetTail(const char *strInput_, int nCount_);
} //clib
} // xugd
#endif // XUGD_CLIB_STR_H