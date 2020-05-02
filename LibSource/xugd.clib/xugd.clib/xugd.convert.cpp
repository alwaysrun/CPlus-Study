#include "xugd.convert.h"
#include "xugd.exception.h"
#include <codecvt>
#include <boost/archive/iterators/base64_from_binary.hpp>  
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>    

namespace xugd{	namespace clib{
	using namespace boost::archive::iterators;

	//////////////////////////////////////////////////////////////////////////
	typedef std::codecvt<wchar_t, char, std::mbstate_t> code_type;
	static std::wstring_convert<code_type> s_utf8Conv(new code_type("CHS"));
	// convert UTF-8 string to wstring
	std::wstring XConvert::Astr2W(const std::string& str)
	{
		return s_utf8Conv.from_bytes(str);
	}
	// convert wstring to UTF-8 string
	std::string XConvert::Wstr2A(const std::wstring& str)
	{
		return s_utf8Conv.to_bytes(str);
	}

	//////////////////////////////////////////////////////////////////////////
	string XConvert::AHex2Bytes(const string &strHex_)
	{
		if(strHex_.size() % 2 != 0)
			throw XSizeException("Size must multiple of 2", strHex_.size());
		
		ostringstream strOut;
		char	szHex[] = "FF";
		for(string::const_iterator it=strHex_.begin() ; it!=strHex_.end() ; )
		{
			szHex[0] = *it++;
			szHex[1] = *it++;
			strOut<<(char)(strtoul(szHex, NULL, 16) & 0xFF);
		}

		return strOut.str();
	}

	string XConvert::WHex2Bytes(const wstring &strHex_)
	{
		if(strHex_.size() % 2 != 0)
			throw XSizeException("Size must multiple of 2", strHex_.size());

		ostringstream strOut;
		wchar_t	wzHex[] = L"FF";
		for(wstring::const_iterator it=strHex_.begin(); it!=strHex_.end(); )
		{
			wzHex[0] = *it++;
			wzHex[1] = *it++;

			strOut<<(char)(wcstoul(wzHex, NULL, 16) & 0xFF);
		}

		return strOut.str();
	}

	wstring XConvert::Bytes2WHex(const char *pByte_, int nSize_)
	{
		wostringstream outStr;
		wchar_t	wzHex[] = L"0123456789ABCDEF";
		for( int i=0 ; i<nSize_ ; ++i )
		{
			outStr<<wzHex[(pByte_[i] >> 4) & 0x0F];
			outStr<<wzHex[pByte_[i] & 0x0F];
		}
		return outStr.str();
	}
	string XConvert::Bytes2AHex(const char *pByte_, int nSize_)
	{
		ostringstream outStr;
		char	szHex[] = "0123456789ABCDEF";
		for( int i=0 ; i<nSize_ ; ++i )
		{
			outStr<<szHex[(pByte_[i] >> 4) & 0x0F];
			outStr<<szHex[pByte_[i] & 0x0F];
		}
		return outStr.str();
	}

	string XConvert::Bytes2Base64Str(const char *pByte_, int nSize_)
	{
		const std::string base64_padding[] = {"", "==","="};

		typedef base64_from_binary<transform_width<const char *, 6, 8> > ToBase64It;  
		ostringstream outStr;  
		copy(ToBase64It(pByte_) , ToBase64It(pByte_+nSize_), ostream_iterator<char>(outStr)); 

		// Fill padding
		outStr<<base64_padding[nSize_%3];

		return outStr.str();
	}

	string XConvert::Base64Str2Bytes(const string &strBase64_)
	{
		typedef transform_width<binary_from_base64<const char*>, 8, 6> FromBase64It;

		// Remove the padding characters
		int nSize = strBase64_.size();
		if(nSize && strBase64_[nSize-1]=='=')
		{
			--nSize;
			if(nSize && strBase64_[nSize-1]=='=')
				--nSize;
		}
		if(CxIsZero(nSize)) return "";

		// Transform
		ostringstream outStr;
		const char *pBase64=strBase64_.data();
		copy(FromBase64It(pBase64), FromBase64It(pBase64+nSize), ostream_iterator<char>(outStr));

		return outStr.str();
	}

	int XConvert::Astr2Int(const string &strIn_){
		stringstream strStream;
		strStream.str(strIn_);

		unsigned int nResult = 0;
		strStream>>nResult;
		return nResult;
	}

	string XConvert::Int2Astr(unsigned int nNumber_, unsigned int nDigits_){
		stringstream strStream;
		strStream<<nNumber_;
		
		string strResult = strStream.str();
		if(strResult.size() >= nDigits_)
			return strResult;

		string strPrefix(nDigits_-strResult.size(), '0');
		return strPrefix + strResult;
	}

	float XConvert::Astr2Float(const string &strIn_){
		stringstream strStream;
		strStream.str(strIn_);

		float nResult = 0.0;
		strStream>>nResult;
		return nResult;
	}

	int XConvert::AstrHex2Int(const string &strIn_){
		stringstream strStream;
		strStream.str(strIn_);

		unsigned int nResult = 0;
		strStream>>std::hex>>nResult;
		return nResult;
	}
	string XConvert::Int2AstrHex(int nHex_){
		stringstream strStream;
		strStream<<std::hex<<nHex_;

		return strStream.str();
	}

} //clib
} // xugd