// ConsoleOut.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ConsoleOut.h"
#include <vector>
#include <algorithm>
using namespace std;

bool funCmpB(const string &str_)
{
	return str_ == "b";
}

#define GET_ARRAY_LEN(array)    (sizeof(array) / sizeof(array[0]))

template <class T>
int getArrayLen(T& array)  //使用模板定义一个函数getArrayLen,该函数将返回数组array的长度
{ 
	return (sizeof(array) / sizeof(array[0]));
}

void RemoveIfTest()
{
	vector<string> vecAll;
	vecAll.push_back("a");
	vecAll.push_back("b");
	vecAll.push_back("c");
	vecAll.push_back("b");
	vecAll.push_back("d");
	vecAll.push_back("e");

	vector<string>::iterator itRem = remove_if(vecAll.begin(), vecAll.end(), funCmpB);
	int nCount = vecAll.size();
	vecAll.erase(itRem, vecAll.end());
	nCount = vecAll.size();
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestReplace();
	//char *pArray = new char[5];
	//int nCount = GET_ARRAY_LEN(pArray);
	//nCount = 0;
	//nCount = getArrayLen(pArray);
	//nCount = 1;

	//GlooxConnectTest();

	//RemoveIfTest();

	// Clib
	// XDateTimeTest();
	// posix_time::time_from_string()
	// PtreeTest();
	// TestConfig(L"File.xml");
	// XTimerTest();
	// XThreadTest();
	//XConditionTest();
	//TestQueue();
	//XConditionTest();
	 //TestXLogFile();
	// XRandomTest();

	//FilePathTest();
	// FileFileTest();
	// TestSharePtr();
	// TestString();
	 //TestConverted();
	// TestSubString();
	// TestPtreeXml();
	 //TestStrFind();

	 //ostringstream strAAA;
	 //wostringstream strBuffer;
	 //string strAtest = " astring ";
	 //strBuffer<<"None";
	 //strAAA<<"None";
	 //strBuffer<<strAtest;
	 //strAAA<<strAtest;
	 ////char *chNone = NULL;
	 ////strBuffer<<chNone;
	 //auto strOut = strBuffer.str();
	 //strAAA<<L" wstr ";

	cout<<"Press to close";
	getchar();
	return 0;
}