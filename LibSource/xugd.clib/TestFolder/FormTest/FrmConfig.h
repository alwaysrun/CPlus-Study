#pragma once
#include "stdafx.h"
#include "xugd.common.h"
#include "xugd.config.h"
#include <string>
using namespace std;
using namespace xugd::clib;

#ifdef _DEBUG
#pragma comment(lib, "xugd.clib_d.lib")
#else
#pragma comment(lib, "xugd.clib.lib")
#endif

struct UserInfoConfig
{
	wstring strName;
	wstring strPsw;
	wstring strAddr;
	wstring strFriend;

	void ToConfig(XConFile &conFile_){
		conFile_.SetAttr(L"", L"Name", strName);
		conFile_.SetAttr(L"", L"Psw", strPsw);
		conFile_.SetAttr(L"", L"Addr", strAddr);
		conFile_.SetAttr(L"", L"Friend", strFriend);
	}
	void FromConfig(const XConFile &conFile_){
		strName=conFile_.GetAttr(L"", L"Name", L"creclient@cti-202.net");
		strPsw = conFile_.GetAttr(L"",L"Psw", L"xgdxgd");
		strAddr = conFile_.GetAttr(L"",L"Addr", L"192.168.1.202");
		strFriend = conFile_.GetAttr(L"",L"Friend",L"cretest@cti-202.net");
	}

	void Read(wstring strFile_)
	{
		try 
		{
			XConFile conFile;
			conFile.Read(strFile_);
			FromConfig(conFile);
		}
		catch(std::exception &ex)
		{
			Write(strFile_);
		}
	}

	void Write(wstring strFile_)
	{
		XConFile conFile;
		if(strName.empty())
			FromConfig(conFile);

		ToConfig(conFile);
		conFile.Write(strFile_);
	}
};