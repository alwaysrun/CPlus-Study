#pragma once
#ifndef XUGD_CLIB_EXCEPTION_H
#define XUGD_CLIB_EXCEPTION_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.exception.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义异常基类与常用异常
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include <exception>
#include <string>
#include <sstream>

namespace xugd{	namespace clib{
	using namespace std;
	//////////////////////////////////////////////////////////////////////////
	// 所有异常的基类
	//////////////////////////////////////////////////////////////////////////
	class XException:public std::exception{
	public:
		XException(const char* szMsg_, int nError_)
			: std::exception(szMsg_), m_nError(nError_)
		{ }
		virtual ~XException(void) {}

		//////////////////////////////////////////////////////////////////////////
		// 获取错误信息
		const char* GetMsg(void) const
		{
			return exception::what();
		}

		//////////////////////////////////////////////////////////////////////////
		// 获取错误号
		int GetErrCode(void) const
		{
			return m_nError;
		}

		//////////////////////////////////////////////////////////////////////////
		// 获取错误码对应的信息（返回空字符串）
		virtual string GetErrInfo(void) const
		{
			return "";
		}

		//重载what，与ToString输出相同
		virtual const char *what() const
		{
			return ToString();
		}


		//////////////////////////////////////////////////////////////////////////
		// 获取打印信息：Msg(ErrInfo[ErrCode])
		// 返回的是内部静态数组，无需释放，所以不是线程安全的。
		const char* ToString(void) const
		{
			static char szPrint[500] = {0};
			sprintf_s(szPrint, 500, "%s(%s[%d])", exception::what(), GetErrInfo(), m_nError);
			return szPrint;
		}

	private:
		int m_nError;
	};

	class XSizeException:public XException
	{
	public:
		XSizeException(const char* szMsg_, int nSize_)
			:XException(szMsg_, 1462)
		{
			m_nSize = nSize_;
		}

		int Size() const 
		{
			return m_nSize;
		}

		string GetErrInfo(void) const
		{
			ostringstream outStr;
			outStr<<"Size at least "<<m_nSize;
			return outStr.str();
		}

	private:
		int m_nSize;
	};

	//////////////////////////////////////////////////////////////////////////
	//	System Error
	// ERROR_INVALID_FUNCTION		1
	// ERROR_FILE_NOT_FOUND			2
	// ERROR_PATH_NOT_FOUND			3
	// ERROR_TOO_MANY_OPEN_FILES	4
	// ERROR_ACCESS_DENIED			5
	// ERROR_INVALID_HANDLE			6
	// ERROR_NOT_ENOUGH_MEMORY		8
	// ERROR_INVALID_BLOCK			9
	// ERROR_BAD_FORMAT				11
	// ERROR_INVALID_DATA			13
	// ERROR_OUTOFMEMORY			14
	// ERROR_INVALID_DRIVE			15
	// ERROR_NO_MORE_FILES			18
	// ERROR_WRITE_PROTECT			19
	// ERROR_NOT_READY				21
	// ERROR_BAD_COMMAND			22
	// ERROR_CRC					23
	// ERROR_BAD_LENGTH				24
	// ERROR_WRITE_FAULT			29
	// ERROR_READ_FAULT				30
	// ERROR_WRONG_DISK				34
	// ERROR_HANDLE_EOF				38
	// ERROR_NOT_SUPPORTED			50
	// ERROR_DEV_NOT_EXIST			55
	// ERROR_FILE_EXISTS			80
	// ERROR_CANNOT_MAKE			82	(File or dir cannot create)
	// ERROR_INVALID_PASSWORD		86
	// ERROR_INVALID_PARAMETER		87
	// ERROR_DRIVE_LOCKED			108
	// ERROR_OPEN_FAILED			110
	// ERROR_BUFFER_OVERFLOW		111
	// ERROR_CALL_NOT_IMPLEMENTED	120
	// ERROR_INSUFFICIENT_BUFFER	122
	// ERROR_INVALID_NAME			123
	// ERROR_PROC_NOT_FOUND			127	(procedure could not found)
	// ERROR_DIR_NOT_ROOT			144
	// ERROR_DIR_NOT_EMPTY			145
	// ERROR_NOT_LOCKED				158
	// ERROR_BAD_PATHNAME			161
	// ERROR_LOCK_FAILED			164
	// ERROR_BUSY					170
	// ERROR_ALREADY_EXISTS			183
	// ERROR_LOCKED					212
	// ERROR_BAD_FILE_TYPE          222
	// ERROR_FILE_TOO_LARGE         223
	// ERROR_NO_DATA				232	
	// ERROR_MORE_DATA				234
	// WAIT_TIMEOUT					258
	// ERROR_NO_MORE_ITEMS			259
	// ERROR_CANNOT_COPY			266
	// ERROR_DIRECTORY				267 (Dir name invalid)
	// ERROR_NOT_OWNER				288
	// ERROR_INVALID_ADDRESS		487
	// ERROR_INVALID_MESSAGE		1002
	// ERROR_CAN_NOT_COMPLETE		1003
	// ERROR_INVALID_FLAGS			1004
	// ERROR_FILE_INVALID			1006
	// ERROR_NO_TOKEN				1008
	// ERROR_BADDB					1009 (registry database corrupt)
	// ERROR_BADKEY					1010 (registry key is invalid)
	// 
	// ERROR_SERVICE_ALREADY_RUNNING	1056
	// ERROR_SERVICE_DISABLED			1058
	// ERROR_SERVICE_DOES_NOT_EXIST		1060
	// ERROR_SERVICE_NOT_ACTIVE			1062
	// ERROR_SERVICE_EXISTS				1073
	// ERROR_DUPLICATE_SERVICE_NAME		1078 (The name is already in use as either a service name or a service display name.)
	// 
	// ERROR_DATABASE_DOES_NOT_EXIST	1065
	// 
	// ERROR_INVALID_BLOCK_LENGTH		1106

	// ERROR_OLD_WIN_VERSION            1150

	// ERROR_BAD_DLL_ENTRYPOINT		609 
	// ERROR_DLL_INIT_FAILED		1114
	// ERROR_INVALID_DLL			1154
	// ERROR_DLL_NOT_FOUND			1157
	// 
	// ERROR_NOT_FOUND			1168
	// ERROR_NO_MATCH			1169
	// ERROR_BAD_DEVICE			1200
	// ERROR_BAD_PROVIDER		1204
	// 
	// ERROR_INVALID_GROUPNAME		1209
	// ERROR_INVALID_DOMAINNAME		1212
	// ERROR_INVALID_SERVICENAME	1213
	// ERROR_INVALID_NETNAME		1214
	// 
	// ERROR_NO_NETWORK				1222
	// ERROR_CANCELLED				1223
	// ERROR_CONNECTION_INVALID		1229
	// ERROR_CONNECTION_ACTIVE		1230
	// ERROR_RETRY					1237
	// ERROR_INCORRECT_ADDRESS		1241
	// ERROR_ALREADY_REGISTERED		1242
	// ERROR_SERVICE_NOT_FOUND		1243
	// ERROR_NOT_AUTHENTICATED		1244
	// ERROR_NOT_LOGGED_ON			1245
	// ERROR_ALREADY_INITIALIZED	1247
	// ERROR_NO_MORE_DEVICES		1248
	// ERROR_RECOVERY_FAILURE		1279
	// ERROR_INVALID_OWNER			1307
	// ERROR_USER_EXISTS			1316
	// ERROR_NO_SUCH_USER			1317
	// ERROR_GROUP_EXISTS			1318
	// ERROR_NO_SUCH_GROUP			1319	
	// ERROR_WRONG_PASSWORD			1323
	// ERROR_LOGON_FAILURE			1326
	// ERROR_PASSWORD_EXPIRED		1330
	// ERROR_ACCOUNT_DISABLED		1331
	// 
	// ERROR_NO_SUCH_DOMAIN			1355
	// ERROR_DOMAIN_EXISTS			1356
	// ERROR_TOKEN_ALREADY_IN_USE	1375
	// ERROR_FILE_CORRUPT			1392
	// ERROR_DISK_CORRUPT			1393
	// 
	// ERROR_INVALID_INDEX			1413
	// ERROR_CLIPBOARD_NOT_OPEN		1418
	// ERROR_TIMEOUT				1460
	// ERROR_INCORRECT_SIZE			1462
	// 
	// ERROR_DATATYPE_MISMATCH		1629
	// ERROR_UNSUPPORTED_TYPE		1630
	// ERROR_CREATE_FAILED			1631
	// ERROR_PRODUCT_VERSION		1638
	// 
	// ERROR_ACCOUNT_EXPIRED		1793
	// ERROR_UNKNOWN_PORT			1796
	// ERROR_INVALID_DATATYPE		1804
	// ERROR_INVALID_TIME			1901
	// ERROR_PASSWORD_MUST_CHANGE	1907
	// ERROR_ACCOUNT_LOCKED_OUT		1909
	// 
	// ERROR_BAD_USERNAME			2202
	// ERROR_NOT_CONNECTED			2250
	// ERROR_ACTIVE_CONNECTIONS		2402
	// ERROR_DEVICE_IN_USE			2404
	// 
	// ERROR_EMPTY				4306
	// ERROR_NOT_EMPTY			4307
	// ERROR_OBJECT_NOT_FOUND	4312
	// ERROR_DATABASE_FAILURE	4313
	// ERROR_DATABASE_FULL		4314
	// 
	// ERROR_INVALID_OPERATION		4317
	// ERROR_DEVICE_NOT_AVAILABLE	4319
	// ERROR_REQUEST_REFUSED		4320
	// 
	// ERROR_DEPENDENCY_NOT_FOUND		5002
	// ERROR_DEPENDENCY_ALREADY_EXISTS	5003
	// ERROR_RESOURCE_NOT_AVAILABLE		5006
	// ERROR_RESOURCE_NOT_FOUND			5007
	// ERROR_OBJECT_ALREADY_EXISTS		5010
	// 
	// ERROR_INVALID_STATE			5023
	// 
	// ERROR_ENCRYPTION_FAILED		6000
	// ERROR_DECRYPTION_FAILED		6001
	// ERROR_FILE_ENCRYPTED			6002
	// ERROR_NO_EFS					6004 (The required encryption driver is not loaded for this system)
	// ERROR_WRONG_EFS				6005 (The file was encrypted with a different encryption driver than is currently loaded)
	// ERROR_NO_USER_KEYS			6006
	// ERROR_FILE_NOT_ENCRYPTED		6007
	// ERROR_NOT_EXPORT_FORMAT		6008
	// ERROR_FILE_READ_ONLY			6009
} //clib
} // xugd

#endif	//XUGD_CLIB_EXCEPTION_H