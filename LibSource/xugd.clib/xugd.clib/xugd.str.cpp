#include "xugd.str.h"
#include "string.h"
#include <boost/algorithm/string.hpp>

namespace xugd{	namespace clib{
using namespace boost;
	//////////////////////////////////////////////////////////////////////////
	// wchar_t & wstring
	XCLIBAPI_DECLARE(void) XWstrTrim(wstring &strInput_, const wchar_t *pTrimed_)
	{
		if(CxWstrNullOrEmpty(pTrimed_))
			trim(strInput_);
		else
			trim_if(strInput_, is_any_of(pTrimed_));
	}
	XCLIBAPI_DECLARE(void) XWstrTrimStart(wstring &strInput_, const wchar_t *pTrimed_)
	{
		if(CxWstrNullOrEmpty(pTrimed_))
			trim_left(strInput_);
		else
			trim_left_if(strInput_, is_any_of(pTrimed_));
	}
	XCLIBAPI_DECLARE(void) XWstrTrimEnd(wstring &strInput_, const wchar_t *pTrimed_)
	{
		if(CxWstrNullOrEmpty(pTrimed_))
			trim_right(strInput_);
		else
			trim_right_if(strInput_, is_any_of(pTrimed_));
	}

	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_)
	{
		if(bIngoreCase_)
		{
			return iends_with(strInput_, strTest_);
		}
		else
		{
			return ends_with(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XWstrEndWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_)
	{
		return ends_with(strInput_, strTest_, cmp_);
	}

	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_)
	{
		if(bIngoreCase_)
		{
			return istarts_with(strInput_, strTest_);
		}
		else
		{
			return starts_with(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XWstrStartWith(const wstring &strInput_, const wstring &strTest_, XWstrCmpFun cmp_)
	{
		return starts_with(strInput_, strTest_, cmp_);
	}

	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XWstrContain(const wstring &strInput_, const wstring &strTest_, bool bIngoreCase_){
		if(bIngoreCase_){
			return icontains(strInput_, strTest_);
		}
		else {
			return contains(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XWstrEqual(const wstring &strFirst_, const wstring &strSecond_, bool bIngoreCase_){
		if(bIngoreCase_){
			return iequals(strFirst_, strSecond_);
		}
		else {
			return equals(strFirst_, strSecond_);
		}
	}

	XCLIBAPI_DECLARE(wstring) XWstrSubStr(const wstring &strInput_, int nLen_)
	{
		int nStart = 0;
		if(nLen_ < 0){
			nStart = strInput_.length() + nLen_;
			if(nStart<=0)
				return strInput_;
			nLen_ = -nLen_;
		}

		return strInput_.substr(nStart, nLen_);
	}

	XCLIBAPI_DECLARE(wstring::size_type) XAstrFindNth(const wstring &strInput_, const wstring &strTofind_, int nNth_, wstring::size_type nStart_){
		if(nNth_<=0)
			return wstring::npos;

		int nIndex = 0;
		wstring::size_type nPos=nStart_-1;
		while(nIndex<nNth_){
			nPos = strInput_.find(strTofind_, nPos+1);
			if(wstring::npos == nPos)
				break;

			++nIndex;
		}

		return nPos;
	}

	XCLIBAPI_DECLARE(bool) XWptrCountEqual(const wchar_t *pFirst_, const wchar_t *pSecond_, size_t nCount_, bool bIgnorCase_)
	{
		if(bIgnorCase_)
			return _wcsnicmp(pFirst_, pSecond_, nCount_)==0;
		else
			return wcsncmp(pFirst_, pSecond_,nCount_)==0;
	}

	//////////////////////////////////////////////////////////////////////////
	// char & string
	XCLIBAPI_DECLARE(void) XAstrTrim(string &strInput_, const char *pTrimed_)
	{
		if(CxAstrNullOrEmpty(pTrimed_))
			trim(strInput_);
		else
			trim_if(strInput_, is_any_of(pTrimed_));
	}
	XCLIBAPI_DECLARE(void) XAstrTrimStart(string &strInput_, const char *pTrimed_)
	{
		if(CxAstrNullOrEmpty(pTrimed_))
			trim_left(strInput_);
		else
			trim_left_if(strInput_, is_any_of(pTrimed_));
	}
	XCLIBAPI_DECLARE(void) XAstrTrimEnd(string &strInput_, const char *pTrimed_)
	{
		if(CxAstrNullOrEmpty(pTrimed_))
			trim_right(strInput_);
		else
			trim_right_if(strInput_, is_any_of(pTrimed_));
	}

	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, bool bIngoreCase_)
	{
		if(bIngoreCase_)
		{
			return iends_with(strInput_, strTest_);
		}
		else
		{
			return ends_with(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XAstrEndWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_)
	{
		return ends_with(strInput_, strTest_, cmp_);
	}

	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, bool bIngoreCase_)
	{
		if(bIngoreCase_)
		{
			return istarts_with(strInput_, strTest_);
		}
		else
		{
			return starts_with(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XAstrStartWith(const string &strInput_, const string &strTest_, XAstrCmpFun cmp_)
	{
		return starts_with(strInput_, strTest_, cmp_);
	}

	//////////////////////////////////////////////////////////////////////////
	XCLIBAPI_DECLARE(bool) XAstrContain(const string &strInput_, const string &strTest_, bool bIngoreCase_){
		if(bIngoreCase_){
			return icontains(strInput_, strTest_);
		}
		else {
			return contains(strInput_, strTest_);
		}
	}
	XCLIBAPI_DECLARE(bool) XAstrEqual(const string &strFirst_, const string &strSecond_, bool bIngoreCase_){
		if(bIngoreCase_){
			return iequals(strFirst_, strSecond_);
		}
		else {
			return equals(strFirst_, strSecond_);
		}
	}


	XCLIBAPI_DECLARE(string) XAstrToLower(const string &strInput_){
		return to_lower_copy(strInput_);
	}
	XCLIBAPI_DECLARE(string) XAstrToUpper(const string &strInput_){
		return to_upper_copy(strInput_);
	}

	XCLIBAPI_DECLARE(std::vector<string>) XAstrSplit(const string &strInput_, const char *strSeperators_, bool bDropEmpty_){
		vector<string> vecAll;
		split(vecAll, strInput_, is_any_of(strSeperators_), token_compress_off); // bDropEmpty_?token_compress_on:token_compress_off);
		if(bDropEmpty_)
		{
			vector<string>::iterator itEmpty = remove(vecAll.begin(), vecAll.end(), "");
			vecAll.erase(itEmpty, vecAll.end());
		}

		return vecAll;
	}
	XCLIBAPI_DECLARE(string) XAstrJoin(const std::vector<string> &strInput_, const char *strSeperator_){
		return join(strInput_, strSeperator_);
	}

	XCLIBAPI_DECLARE(string) XAstrReplaceFirst(const string &strInput_, const string &strOld_, const string &strNew_)
	{
		string strResult;
		replace_first_copy(back_inserter(strResult), strInput_, strOld_, strNew_);
		return strResult;
	}
	XCLIBAPI_DECLARE(string) XAstrReplaceAll(const string &strInput_, const string &strOld_, const string &strNew_)
	{
		string strResult;
		ireplace_all_copy(back_inserter(strResult), strInput_, strOld_, strNew_);
		return strResult;
	}

	XCLIBAPI_DECLARE(string) XAstrSubStr(const string &strInput_, int nLen_)
	{
		int nStart = 0;
		if(nLen_ < 0){
			nStart = strInput_.length() + nLen_;
			if(nStart<=0)
				return strInput_;
			nLen_ = -nLen_;
		}

		return strInput_.substr(nStart, nLen_);
	}

	XCLIBAPI_DECLARE(string::size_type) XAstrFindNth(const string &strInput_, const string &strTofind_, int nNth_, string::size_type nStart_){
		if(nNth_<=0)
			return string::npos;

		int nIndex = 0;
		string::size_type nPos=nStart_-1;
		while(nIndex<nNth_){
			nPos = strInput_.find(strTofind_, nPos+1);
			if(string::npos == nPos)
				break;

			++nIndex;
		}

		return nPos;
	}

	XCLIBAPI_DECLARE(bool) XAptrEqual(const char *pFirst_, const char *pSecond_, bool bIgnorCase_){
		if(bIgnorCase_)
			return _stricmp(pFirst_, pSecond_)==0;
		else
			return strcmp(pFirst_, pSecond_)==0;
	}

	XCLIBAPI_DECLARE(bool) XAptrCountEqual(const char *pFirst_, const char *pSecond_, size_t nCount_, bool bIgnorCase_)
	{
		if(bIgnorCase_)
			return _strnicmp(pFirst_, pSecond_, nCount_)==0;
		else
			return strncmp(pFirst_, pSecond_,nCount_)==0;
	}

	XCLIBAPI_DECLARE(const char*) XAptrFindFirst(const char *strInput_, const char *strSub_, bool bIngoreCase_){
		boost::iterator_range<const char*> fRange;
		if(bIngoreCase_)
			fRange = ifind_first(strInput_, strSub_);
		else
			fRange = find_first(strInput_, strSub_);

		if(fRange.empty()) return NULL;
		return fRange.begin();
	}
	XCLIBAPI_DECLARE(const char*) XAptrFindLast(const char *strInput_, const char *strSub_, bool bIngoreCase_){
		boost::iterator_range<const char*> fRange;
		if(bIngoreCase_)
			fRange = ifind_last(strInput_, strSub_);
		else
			fRange = find_last(strInput_, strSub_);
		
		if(fRange.empty()) return NULL;
		return fRange.begin();
	}

	//XCLIBAPI_DECLARE(const char*) XAptrGetTail(const char *strInput_, int nCount_){
	//	auto fRange = find_tail(strInput_, nCount_);
	//	return fRange.begin();
	//}
} //clib
} // xugd