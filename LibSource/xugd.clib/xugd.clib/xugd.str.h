#pragma once
#ifndef XUGD_CLIB_STR_H
#define XUGD_CLIB_STR_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.str.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义字符串与字符指针操作接口类
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>
#include <vector>

// 推荐使用<boost/algorithm/string.hpp>中的string_algo操作函数(boost::fxxx)。
// 对于函数fxxx：
//		涉及到大小写比较都对应的ifxxx；
//		涉及到修改的都有fxxx_copy返回修改值，而非直接修改；
//		如果有谓词（比较条件）则有fxxx_if（对于copy类的则为fxxx_copy_if）
// 大小写转换：to_upper、to_lower；
// 截取：trim、trim_left、trim_right；
// 判断包含：starts_with、ends_with、contains；
// 相等：equals；
// 条件判断：bool all(Input, Pred)、unspecified is_any_of(Set)、nspecified is_from_range(From, To);;
// 类别判断：is_classified、is_space、is_alnum 、is_alpha、is_cntrl 、is_digit 、is_graph 
//			is_lower 、is_print 、is_punct 、is_upper 、is_xdigit ；
// 删除：erase_all 、erase_first、erase_last 、erase_nth（删除指定那个）、 erase_range 
//		（删除指定个数字符）erase_head 、erase_tail 
// 替换：replace_first 、replace_last 、replace_nth 、replace_all
//		replace_head 、replace_tail 
// 查找：find、find_first、find_last、find_nth、find_head、find_tail、find_token

namespace xugd{	namespace clib{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// 移除指定字符：如Trimed="\\/"则移除所有路径分隔符
	XCLIBAPI_DECLARE(void) XWstrTrim(wstring &strInput_, const wchar_t *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XWstrTrimStart(wstring &strInput_, const wchar_t *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XWstrTrimEnd(wstring &strInput_, const wchar_t *pTrimed_=NULL);

	// 比较
	typedef bool (*XWstrCmpFun)(wchar_t fr_, wchar_t se_);
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_);
	
	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_);
	
	XCLIBAPI_DECLARE(bool) XWstrContain(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XWstrEqual(const wstring &strFirst_, const wstring &strSecond_, bool bIngoreCase_=false);

	// 获取子串：如果Len<0，则说明从尾部获取；否则获取头部
	XCLIBAPI_DECLARE(wstring) XWstrSubStr(const wstring &strInput_, int nLen_);

	// 查找指定（第n个）的子串：找到返回第n个子串的位置，否则返回wstring::npos
	XCLIBAPI_DECLARE(wstring::size_type) XAstrFindNth(const wstring &strInput_, const wstring &strTofind_, int nNth_, wstring::size_type nStart_=0);
	
	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XWptrCountEqual(const wchar_t *pFirst_, const wchar_t *pSecond_, size_t nCount_, bool bIgnorCase_=false);

	//////////////////////////////////////////////////////////////////////////
	// 移除指定字符：如Trimed="\\/"则移除所有路径分隔符，默认移除空白
	XCLIBAPI_DECLARE(void) XAstrTrim(string &strInput_, const char *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XAstrTrimStart(string &strInput_, const char *pTrimed_=NULL);
	XCLIBAPI_DECLARE(void) XAstrTrimEnd(string &strInput_, const char *pTrimed_=NULL);

	// 比较
	typedef bool (*XAstrCmpFun)(char fr_, char se_);
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_);

	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_);

	XCLIBAPI_DECLARE(bool) XAstrContain(const string &strInput_, const string &strTest_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(bool) XAstrEqual(const string &strFirst_, const string &strSecond_, bool bIngoreCase_=false);

	// 转换
	XCLIBAPI_DECLARE(string) XAstrToLower(const string &strInput_);
	XCLIBAPI_DECLARE(string) XAstrToUpper(const string &strInput_);
	
	// 分割：如果strSeperators包含多个字符，则任一字符都为分隔符
	XCLIBAPI_DECLARE(std::vector<string>) XAstrSplit(const string &strInput_, const char *strSeperators_, bool bDropEmpty_);
	XCLIBAPI_DECLARE(string) XAstrJoin(const std::vector<string> &strInput_, const char *strSeperator_);

	XCLIBAPI_DECLARE(string) XAstrReplaceFirst(const string &strInput_, const string &strOld_, const string &strNew_);
	XCLIBAPI_DECLARE(string) XAstrReplaceAll(const string &strInput_, const string &strOld_, const string &strNew_);

	// 获取子串：如果Len<0，则说明从尾部获取；否则获取头部
	XCLIBAPI_DECLARE(string) XAstrSubStr(const string &strInput_, int nLen_);

	// 查找指定（第n个）的子串：找到返回第n个子串的位置，否则返回string::npos
	XCLIBAPI_DECLARE(string::size_type) XAstrFindNth(const string &strInput_, const string &strTofind_, int nNth_, string::size_type nStart_=0);

	//////////////////////////////////////////////////////////////////////////
	// PChar
	XCLIBAPI_DECLARE(bool) XAptrEqual(const char *pFirst_, const char *pSecond_, bool bIgnorCase_=false);
	XCLIBAPI_DECLARE(bool) XAptrCountEqual(const char *pFirst_, const char *pSecond_, size_t nCount_, bool bIgnorCase_=false);

	// 查找不到返回NULL，否则返回子串开始位置
	XCLIBAPI_DECLARE(const char*) XAptrFindFirst(const char *strInput_, const char *strSub_, bool bIngoreCase_=false);
	XCLIBAPI_DECLARE(const char*) XAptrFindLast(const char *strInput_, const char *strSub_, bool bIngoreCase_=false);

	// 获取末尾指定长度的子串，返回子串开始位置（如果指定长度超过字符串长度，则返回字符串本身）
	//XCLIBAPI_DECLARE(const char*) XAptrGetTail(const char *strInput_, int nCount_);
} //clib
} // xugd
#endif // XUGD_CLIB_STR_H