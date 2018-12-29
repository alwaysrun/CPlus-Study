#include "ConsoleOut.h"

#include <fstream>
#include <queue>
//#include <boost/timer.hpp>
//#include <boost/progress.hpp>
//#include <boost/pool/object_pool.hpp>
//#include <boost/operators.hpp>
//#include <boost/exception/all.hpp>
//#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/any.hpp>
//#include <boost/variant.hpp>
//#include <boost/bimap.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
//#include <boost/thread.hpp>
//#include <boost/asio.hpp>

#include "../../xugd.clib/xugd.time.h"
#include "../../xugd.clib/xugd.config.h"
#include "../../xugd.clib/xugd.str.h"
#include "../../xugd.clib/xugd.file.h"
#include "../../xugd.clib/xugd.flag.h"
#include "../../xugd.clib/xugd.thread.h"
#include "../../xugd.clib/xugd.log.h"
#include "../../xugd.clib/xugd.random.h"
#include "../../xugd.clib/xugd.convert.h"

#ifdef _DEBUG
#pragma comment(lib, "xugd.clib_d.lib")
#else
#pragma comment(lib, "xugd.clib.lib")
#endif

using namespace xugd::clib;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::filesystem;
//using namespace boost::asio;

void TestReplace()
{
	string strOri = "\\d12\\d\\\\";
	string strResult = XAstrReplaceAll(strOri, "\\", "\\\\");
	string strNone = strResult;
}

void TestStrFind(){
	using namespace boost;
	char *strAll = "a11ab22ab33aab44a";
	vector<string> vecAll = XAstrSplit(strAll, "a", false);
	vector<string> vecValid = XAstrSplit(strAll, "a", true);

	iterator_range<const char*> fRange;
	fRange = find_first(strAll, "ab");
	auto fLast = find_last(strAll, "ab");
	auto fNth = find_nth(strAll, "ab", 1);
	auto fNone = find_first(strAll, "none");

	auto fHead = find_head(strAll, 4);
	auto fHeadErr = find_head(strAll, 100);
	auto fEnd = find_tail(strAll, 4);
	auto fEndErr = find_tail(strAll, 100);

	string strA = vecAll[0];
}

void TestPtreeXml(){
	using namespace boost::property_tree;
	string strXml = 
		"<result row_count=\"2\">"
		//"<row row_id=\"1\">"
		//  "<uuid>2777cc24-2fcd-11e5-ab8c-51038ec3c579</uuid>"
		//  "<direction>inbound</direction>"
		//  "<created>2015-07-22 01:23:08</created>"
		//"</row>"
		//"<row row_id=\"2\">"
		//  "<uuid>28b015ce-2fcd-11e5-ab9a-51038ec3c579</uuid>"
		//  "<direction>outbound</direction>"
		//  "<created>2015-07-22 01:23:10</created>"
		//"</row>"
		"<row row_id=\"1\"/>"
		"<row row_id=\"2\"/>"
		//"<row>3</row>"
		//"<row>4</row>"
		"</result>";

	XSimpleXmlA sipXml(strXml);
	auto mulText = sipXml.GetMultiAttr("result.row", "row_id");

	//stringstream inStr;
	//inStr<<strXml;
	//ptree ptRead;
	//read_xml(inStr, ptRead);

	////string strRow = ptRead.get<string>("result.row.<xmlattr>.row_id");
	//auto rngRow = ptRead.get_child("result").equal_range("row");
	//for(auto rIt=rngRow.first ; rIt!=rngRow.second ; ++rIt){
	////auto rngRow = ptRead.get_child("result.row");
	////for(auto rIt=rngRow.begin() ; rIt!=rngRow.end() ; ++rIt){
	//	auto rSub = *rIt;
	//	string strKey = rIt->first;
	//	auto strValue = (rIt->second).get<string>("");
	//}
}

string GetEventPhoneExt(const string& strChName_){
	string::size_type nPos = strChName_.find('@');
	if(string::npos == nPos)
		return "";
	// sofia/internal/sip:1013@192.168.0.83:5060
	string::size_type nStart = strChName_.rfind(':', nPos);
	if(string::npos != nStart){
		++nStart;
		return strChName_.substr(nStart, nPos-nStart);
	}

	// sofia/internal/1006@192.168.0.204
	nStart = strChName_.rfind('/', nPos);
	if(string::npos != nStart){
		++nStart;
		return strChName_.substr(nStart, nPos-nStart);
	}

	return "";
}
void TestSubString(){
	string strOne = "sofia/internal/sip:1013@192.168.0.83:5060";
	string strTwo = "sofia/internal/1006@192.168.0.204";

	string strResult = GetEventPhoneExt(strOne);
	strResult = GetEventPhoneExt(strTwo);
}

void TestConverted(){

	float fCount = XConvert::Astr2Float("12.324");
	float fSecond = XConvert::Astr2Float(".23");
	float fThird = XConvert::Astr2Float("df12.34");
	float fFour = XConvert::Astr2Float("23.43fsfds");
	float fFive = XConvert::Astr2Float("0012.343");

	int nCount = XConvert::Astr2Int("01");
	string strHex = XConvert::Int2AstrHex(nCount);
	int nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;

	nCount = XConvert::Astr2Int("-1");
	strHex = XConvert::Int2AstrHex(nCount);
	nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;

	nCount = XConvert::Astr2Int("+012");
	strHex = XConvert::Int2AstrHex(nCount);
	nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;

	nCount = XConvert::Astr2Int("-23.5");
	strHex = XConvert::Int2AstrHex(nCount);
	nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;

	nCount = XConvert::Astr2Int("0x12");
	strHex = XConvert::Int2AstrHex(nCount);
	nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;

	nCount = XConvert::Astr2Int("confer lis");
	strHex = XConvert::Int2AstrHex(nCount);
	nResult = XConvert::AstrHex2Int(strHex);
	cout<<nCount<<endl;


	// Big number convert
	string strLong = "4294967295";
	unsigned int resInt;
	long long resLong;
	for(int i=0 ; i<5 ; ++i){
		resLong = XConvert::Astr2Type<long long>(strLong);
		resInt = XConvert::Astr2Type<unsigned int>(strLong);

		strLong += XConvert::Type2Astr(i);
	}
}

void TestString(){
	string strResult = "name|system|uuid|type|contact|status|state|max_no_answer|wrap_up_time|reject_delay_time|busy_delay_time|no_answer_delay_time|last_bridge_start|last_bridge_end|last_offered_call|last_status_change|no_answer_count|calls_answered|talk_time|ready_time\n"
		"1006|single_box||callback|[call_timeout=60]user/1006|Available|Waiting|3|10|10|10|0|1432033374|1432033376|1432034239|1432709855|0|0|0|0\n"
		"+OK";
	string::size_type nStart=strResult.find("1006");
	nStart = XAstrFindNth(strResult, "|", 5, nStart);
	if(string::npos == nStart)
		return ;
	++nStart;
	string::size_type nEnd = strResult.find("|", nStart);
	if(string::npos == nEnd)
		return ;
	string strStatus = strResult.substr(nStart, nEnd-nStart);
}

class TestShareData{
public:
	string strInfo;
};
typedef tr1::shared_ptr<TestShareData> TestShareDataPrt;
void TestSharePtr(){
	TestShareDataPrt ptrData(new TestShareData);
	TestShareDataPrt ptrNone(NULL);
	if(ptrNone){
		bool bTest = false;
	}
	if(ptrData){
		bool bTest = true;
	}
}

void XRandomTest()
{
	for(int i=0 ; i<10 ; ++i)
	{
		int nBuffLen = XRandom::GetInt(10, 100);
		string strRand = XRandom::GetBuff(nBuffLen);
		string strHex = XConvert::Bytes2AHex(strRand);
		string strOut = XConvert::AHex2Bytes(strHex);
		assert(strRand == strOut);

		string strBase64 = XConvert::Bytes2Base64Str(strRand);
		string strBinary = XConvert::Base64Str2Bytes(strBase64);
		assert(strRand == strBinary);
	}

	for(int j=0 ; j<20 ; ++j){
		int nFalse = 0, nTrue = 0;
		for(int i=0; i<1000 ; ++i)
		{
			if(XRandom::GetBool())
				++nTrue;
			else
				++nFalse;
			// cout<<XRandom::GetInt()<<" \t";
		}
		cout<<endl;
		cout<<"True: "<<nTrue<<", False: "<<nFalse<<endl;
	}
	//for(int i=0 ; i<50 ; ++i)
	//{
	//	cout<<XRandom::GetString(XRandom::GetInt(10, 50))<<endl;
	//}
}

void TestXLogFile()
{
	wstring strConfig(L"LogFile.xml");
	XLogConfig logConfig;
	if(!logConfig.Read(strConfig))
	{
		logConfig.InitHourLog(L"Test");
		logConfig.Write(strConfig);
	}

	int nIndex = 0;
	XLogFile logFile(logConfig);
	wostringstream strOut;
	strOut<<L"Large"<<endl;
	strOut<<L"打算范德萨发 "<<endl;
	strOut<<"My test only"<<endl;
	strOut<<"普通的字符串（窄字符）"<<endl;
	wcout<<strOut.str();
	logFile.WriteFormater(strOut, XLogFile::LogInfo);
	string strFormat = "sofia/external/%s@192.168.6.90";
	strFormat = XAstrReplaceFirst(strFormat, "%s", "%1%");
	logFile.Called("%s", strFormat.c_str());
	string strInfo = "123";
	string strCmd = (boost::format(strFormat)%strInfo).str();

	while(true)
	{
		cout<<++nIndex<<endl;
		logFile.Called(L"Called %d", nIndex);
		logFile.Info(L"Info %d", nIndex);
		logFile.Notice(L"Notice %d", nIndex);
		logFile.Warn(L"Warn %d", nIndex);
		logFile.Error(L"Error %d", nIndex);

		XThread::Sleep(0, 0, 10);
	}

}

void TestQueue()
{
	queue<string> quStrs;
	quStrs.push("first");
	quStrs.push("\n");
	quStrs.push("Two");
	string str("\nThree\n");
	quStrs.push(str);
	while(!quStrs.empty())
	{
		cout<<quStrs.front();
		quStrs.pop();
	}
}

XMutex g_locker;
XAutoEvent g_event;
void GetCondition(void *parm_)
{
	string str((char*)parm_);
	for(int i=0 ;i<20; i++)
	{
		{
			if(g_event.TimeWait(0, 0, 2))
			{
				cout<<i<<": wait, "<<str<<endl;
			}
			else
			{
				cout<<i<<": timeout, "<<str<<endl;
			}
			//if(g_event.TimeWait(1))
			//{
			//	cout<<i<<"again: wait"<<endl;
			//}
			//else
			//{
			//	cout<<i<<"again: timeout"<<endl;
			//}
		}
	}
}

void SetCondition(void *)
{
	g_event.Notify();
	cout<<"!Notified"<<endl;
	XThread::Sleep(5000);
	for(int i=0 ;i<10; i++)
	{
		g_event.Notify();
		g_event.Notify();
		XThread::Sleep(1000);
	}
}

void XConditionTest()
{
	g_event.Notify();
	XThread::Start(SetCondition, NULL);

	cout<<"!To wait Event"<<endl;
	g_event.Wait();
	cout<<"!Event waited"<<endl;

	XThread::Start(GetCondition, "one");
	//XThread::Start(GetCondition, "two");
	XThread::Start(SetCondition, NULL);

	getchar();
}

void MyThread(void *pInfo_)
{
	string str((char*)pInfo_);
	for(int i=1; i<10 ; )
	{
		{
			XMutexScopLock mt(g_locker);
			cout<<str;
			cout<<i<<endl;
			XThread::Sleep(100);
		}

		// XThread::Sleep(10);
		++i;
	}
}

void XThreadTest()
{
	cout<<"XThreadTest Start"<<endl;

	//XThread::Start(MyThread, "First:");
	//XThread::Start(MyThread, "Second:");
	//XThread::Start(MyThread, "three:");
	//XThread::Start(MyThread, "four:");
	//XThread::Start(MyThread, "five:");

	XThread xThr(MyThread);
	xThr.Start("None:");
	XThread::Sleep(5);
	xThr.Stop();
	xThr.Start("None:");
	XThread::Sleep(1);
	xThr.Start("None:");

	// XThread::Sleep(1000);
	getchar();
}

void MyTimerCallback(const XTimer& xTimer_, void *pParm_)
{
	ptime ptNow = posix_time::microsec_clock::local_time();
	cout<<ptNow<<endl;
}

void XTimerTest()
{
	posix_time::ptime ptNow = posix_time::second_clock::local_time();
	auto nextClock = posix_time::time_duration(ptNow.time_of_day().hours()+1, 0, 0);
	//for(int i=0 ;i<24 ; ++i)
	//{
	//	nextClock = posix_time::time_duration(nextClock.hours()+1, 0, 0);
	//	int nHoure = nextClock.hours();
	//	cout<<nextClock<<endl;
	//}
	posix_time::ptime ptExpire = posix_time::ptime(ptNow.date(), nextClock);
	cout<<ptExpire<<endl;


	XClockTimer myTimer;
	myTimer.Start(MyTimerCallback, NULL);
	cout<<"ClockTimer started"<<endl;
	XThread::Sleep(0, 1, 0);
	myTimer.Stop();
	XThread::Sleep(0, 0, 2);
	myTimer.Start(MyTimerCallback, NULL);
	cout<<"ClockTimer restarted"<<endl;

	XThread::Sleep(0, 2, 0);
	myTimer.Stop();
	XThread::Sleep(0, 0, 5);
	myTimer.Start(MyTimerCallback, NULL);
	cout<<"ClockTimer restarted"<<endl;


	cout<<"press to end:"<<endl;
	getchar();

	cout<<"ClockTimer destroy"<<endl;
	myTimer.Stop();
}

void FileFileTest()
{
	//path ptDir = L"D:\\Tmp\\first\\second";
	//bool bRet = create_directory(ptDir);
	//bool bRet = create_directories(ptDir);

	//long lSize = file_size(ptDir);
	// last_write_time(ptDir);

	//remove(ptDir);
	// remove_all(L"D:\\Tmp\\first");

	wstring strPath = L"F:\\应用软件";
	wstring strFile = L"out.txt";
	wostringstream strOut;
	auto allFile = XDir::EnumFiles(L"F:\\应用软件");
	// wcout<<strPath<<endl;
	for(auto it=allFile.begin() ; it!=allFile.end(); ++it)
	{
		strOut.str(L"");
		strOut<<(it->IsDir?L"Dir: ":L"File:");
		strOut<<it->Name;
		XFile::AppendText(strFile, strOut.str());
	}
}

void FilePathTest()
{
	boost::filesystem::path ptTest(L"e:/Projects/SH Cre\\xugd.clib\\File.xml");
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<"native:"<<ptTest.native()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;
	cout<<"ext :"<<ptTest.extension()<<endl;
	cout<<"stem:"<<ptTest.stem()<<endl;
	cout<<"Dir:"<<ptTest.relative_path()<<endl;
	//ptTest += L"AddOne";
	ptTest /=L"Two";
	wcout<<"Added:"<<ptTest<<endl;

	cout<<endl;
	ptTest = L"SH Cre\\xugd.clib\\File.xml";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<"Replace:"<<ptTest.replace_extension(L"new")<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"Replace:"<<ptTest.replace_extension(L"")<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"Replace:"<<ptTest.replace_extension(L"tre")<<endl;
	wcout<<"Full:"<<ptTest<<endl;

	cout<<endl;
	ptTest = L"\\SH Cre\\xugd.clib\\File.xml";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;

	cout<<endl;
	ptTest = L"SH Cre\\xugd.clib\\File.xml\\";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<"Dir:"<<ptTest.relative_path()<<endl;

	cout<<endl;
	ptTest = L"\\SH Cre\\xugd.clib\\File.xml\\";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl; 
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;

	cout<<endl;
	ptTest = L"e:/";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;
	cout<<"ext :"<<ptTest.extension()<<endl;
	cout<<"stem:"<<ptTest.stem()<<endl;
	cout<<"Dir:"<<ptTest.relative_path()<<endl;

	cout<<endl;
	ptTest = L"e:/f1";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;
	cout<<"ext :"<<ptTest.extension()<<endl;
	cout<<"stem:"<<ptTest.stem()<<endl;

	cout<<endl;
	ptTest = L"e:";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;
	cout<<"ext :"<<ptTest.extension()<<endl;
	cout<<"stem:"<<ptTest.stem()<<endl;
	ptTest /="f1";
	cout<<"Slash:"<<ptTest<<endl;
	ptTest +="\\";
	cout<<"Append:"<<ptTest<<endl;

	cout<<endl;
	ptTest = L"e";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;

	cout<<endl;
	ptTest = L"\\e";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;

	cout<<endl;
	ptTest = L"e\\";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;

	cout<<endl;
	ptTest = L"\\e\\";
	wcout<<"Full:"<<ptTest<<endl;
	cout<<"HasRootDir:"<<ptTest.has_root_directory()
		<<" ,HasRootName:"<<ptTest.has_root_name()
		<<" ,HasRootPath:"<<ptTest.has_root_path()
		<<" ,HasRelative:"<<ptTest.has_relative_path()
		<<" ,HasParent:"<<ptTest.has_parent_path()<<endl;
	cout<<"root_Name:"<<ptTest.root_name()<<", Path:"<<ptTest.root_path()<<endl;
	cout<<ptTest.is_complete()<<endl;
	cout<<"parent:"<<ptTest.parent_path()<<endl;
	cout<<"file :"<<ptTest.filename()<<endl;

	// Remove test
	cout<<endl;
	ptTest = L"e:/xugd.clib\\second\\File.xml";
	wcout<<"Normal:"<<ptTest.normalize()<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	wcout<<"RemoveLeaf:"<<ptTest.remove_leaf()<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	wcout<<"Remove:"<<ptTest.remove_filename()<<endl;

	cout<<endl;
	ptTest = L"e:/xugd.clib\\second\\File.xml\\";
	wcout<<"Normal:"<<ptTest.normalize()<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	wcout<<"RemoveLeaf:"<<ptTest.remove_leaf()<<endl;
	wcout<<"Full:"<<ptTest<<endl;
	wcout<<"Remove:"<<ptTest.remove_filename()<<endl;

	cout<<endl;
	cout<<initial_path()<<endl;
	//create_
}

void XDateTimeTest()
{
	auto tmZone = _timezone;

	XDateTime dtTime;
	dtTime.FromEpoch1970(1432048884, 0);
	string strTime = dtTime.ToAString(true);

	// 2015-8-3 10:
	dtTime.FromEpoch1970(1438569998, 123);
	strTime = dtTime.ToAString(true);


	XDateNow tNow;
	XDateTime dt;
	while(true)
	{
		tNow.Now();
		wostringstream strOut;
		strOut<<tNow;
		wistringstream strIn(strOut.str());
		strIn>>dt;

		wcout << tNow << L" : " << dt;

		getchar();
	}
}

void PtreeTest()
{
	property_tree::ptree ptChild;
	ostringstream outChild;
	ptChild.put("<xmlattr>.Flag", 1234);
	ptChild.put("File", "MyFile");
	write_xml(outChild, ptChild);

	property_tree::ptree ptParent;
	ostringstream outParent;
	ptParent.put("None", "None");
	write_xml(outParent, ptParent);

	ptParent.add_child("Child", ptChild);
	outParent.str("");
	write_xml(outParent, ptParent);
	auto ptBegin=ptParent.begin();
	auto ptFirst=ptBegin->first;
	auto ptSecond=ptBegin->second;

	ptChild=ptParent.get_child("Child");
	outChild.str("");
	write_xml(outChild, ptChild);

	int Flag = ptChild.get<int>("<xmlattr>.Flag");
}

struct FileConfig
{
	int nFlag;
	bool toSet;
	wstring strFile;
	XDateTime dtSet;

	void ToConfig(XConFile &conFile_)
	{
		conFile_.SetAttr(L"", L"Flag", nFlag);
		conFile_.SetAttr(L"", L"Set", toSet);
		conFile_.SetText(L"File", strFile);
		conFile_.SetAttr(L"", L"SetTime", dtSet);
	}
	void FromConfig(const XConFile &conFile_)
	{
		nFlag=conFile_.GetAttr(L"", L"Flag", 0);
		toSet=conFile_.GetAttr(L"", L"Set", false);
		strFile=conFile_.GetText(L"File", L"");
		dtSet=conFile_.GetAttrTime(L"", L"SetTime");
	}
};
struct FileParent
{
	wstring strNone;
	FileConfig stChild;

	void ToConfig(XConFile &conFile_)
	{
		conFile_.SetText(L"None", strNone);

		XConFile conChild;
		stChild.ToConfig(conChild);
		conFile_.AddChildConfig(conChild, L"Child");
	}
	void FromConfig(const XConFile &conFile_)
	{
		strNone = conFile_.GetText(L"None", L"");

		XConFile conChild;
		conFile_.GetChildConfig(conChild, L"Child");
		stChild.FromConfig(conChild);
	}
};
void WriteConfig(const wstring &strFile_, FileParent &fCon);
void TestConfig(const wstring &strFile_)
{
	FileParent fParent;
	try 
	{
		XConFile conParent;
		conParent.Read(strFile_);
		fParent.FromConfig(conParent);
		wcout<<fParent.strNone<<endl;
		cout<<"Read OK"<<endl;
	}
	catch(std::exception &ex)
	{
		fParent.strNone = L"Nothing";
		fParent.stChild.nFlag = 1234;
		fParent.stChild.strFile = L"File啊我饿";
		fParent.stChild.dtSet.Now();

		WriteConfig(strFile_, fParent);
	}
}

void WriteConfig(const wstring &strFile_, FileParent &fCon)
{
	try 
	{
		XConFile conFile;
		fCon.ToConfig(conFile);
		conFile.Write(strFile_);
	}
	catch(std::exception &ex)
	{

	}
}