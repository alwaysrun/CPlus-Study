#pragma once
#ifndef XUGD_XMPP_H
#define XUGD_XMPP_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.xmpp.h
// @LIB: xugd.xmpp.lib/xugd.xmpp_d.lib
// @DESCRIPTION: 定义连接即时通讯（XMPP）服务端（如openfire等）的客户端库
//	#ifdef _DEBUG
//	#pragma comment(lib, "xugd.xmpp_d.lib")
//	#else
//	#pragma comment(lib, "xugd.xmpp.lib")
//	#endif
// @VERSION: 1.0.15.929
//
// @RECORD:
//	编写：Xugd，2015-09-29，初始版本。
//	添加：Xugd，2017-3-6，添加获取启动时Stream头信息接口。
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "xugd.log.h"

namespace xugd{ namespace xmpp{

#ifdef	XUGD_XMPP_EXPORT
#define XXMPPAPI_DECLARE(_rv)	__declspec(dllexport) _rv __stdcall
#else
#define XXMPPAPI_DECLARE(_rv)	__declspec(dllimport) _rv __stdcall
#endif

#ifdef __cplusplus  
	extern "C" {  
#endif 
		typedef enum 
		{
			XmppRes_Success,              
			XmppRes_StreamError,          
			XmppRes_StreamVersionError,   
			XmppRes_StreamClosed,         
			XmppRes_ProxyAuthRequired,    
			XmppRes_ProxyAuthFailed,      
			XmppRes_ProxyNoSupportedAuth, 
			XmppRes_IoError,              
			XmppRes_ParseError,           
			XmppRes_ConnectionRefused,
			XmppRes_DnsError,             
			XmppRes_OutOfMemory,          
			XmppRes_NoSupportedAuth,      
			XmppRes_TlsFailed,            
			XmppRes_TlsNotAvailable,     
			XmppRes_CompressionFailed,    
			XmppRes_AuthenticationFailed, 
			XmppRes_UserDisconnected,     
			XmppRes_NotConnected,
			// Below is added
			XmppRes_ParamIsNULL,
			XmppRes_Timeout,
			XmppRes_NotFound,	// UserJid not found(nod login)
			XmppRes_HasLogin,	// User has login
			XmppRes_NoPong,		// Noresponse of Ping.
		}XmppResult_t;

		typedef enum 
		{
			XmppPre_Available,    // The entity is online. 
			XmppPre_Chat,         // The entity is 'available for chat'. 
			XmppPre_Away,         // The entity is away. 
			XmppPre_DND,          // The entity is DND (Do Not Disturb). 
			XmppPre_XA,           // The entity is XA (eXtended Away). 
			XmppPre_Unavailable,  // The entity is offline. 
			XmppPre_Probe,        // This is a presence probe. 
			XmppPre_Error,        // This is a presence error. 
			XmppPre_Invalid       // The stanza is invalid. 
		}XmppPresenceType_t;

		typedef enum 
		{
			XmppMsg_Chat           =  1,        // A chat message.
			XmppMsg_Error          =  2,        // An error message.
			XmppMsg_Groupchat      =  4,        // A groupchat message.
			XmppMsg_Headline       =  8,        // A headline message.
			XmppMsg_Normal         = 16,        // A normal message.
			XmppMsg_Invalid        = 32         // The message stanza is invalid.
		}XmppMsgType_t;

		typedef void (*PXmppOnLogin)(const char *strUserJid_);
		typedef void (*PXmppOnClose)(const char *strUserJid_, XmppResult_t euErr_);
		typedef void (*PXmppOnPresence)(const char *strUserJid_, const char *strFriendJid_, XmppPresenceType_t euType_);
		typedef void (*PXmppOnMsgReceive)(const char *strUserJid_, const char *strFrom_, const char *pUtf8_, int nCount_, XmppMsgType_t euType_);

		//////////////////////////////////////////////////////////////////////////
		// jid（要全部使用小写）: name@domain

		// strLogFile: if is NULL, use default(glXmpp)
		XXMPPAPI_DECLARE(XmppResult_t) XmppLogin(const char *strUserJid_, const char *strPsw_, 
			const char *strIp_, int nPort_, const wchar_t *strLogFile_);

		XXMPPAPI_DECLARE(void) XmppClose(const char *strUserJid_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppGetStartStream(const char *strUserJid_, char *strStream_, int nSize_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppGetRemoteAddress(const char *strUserJid_, char *strAddress_, int nSize_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppGetLocalAddress(const char *strUserJid_, char *strAddress_, int nSize_);

		// nLevel_的取值范围为XLogFile::LogLevel
		XXMPPAPI_DECLARE(XmppResult_t) XmppSetLogLevel(const char *strUserJid_, int nLevel_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppSendMsg(const char *strUserJid_, const char *strToJid_, const char *strUtf8Msg_);
		XXMPPAPI_DECLARE(XmppResult_t) XmppSendByteMsg(const char *strUserJid_, const char *strToJid_, const char *pUtf8_, int nCount_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppSubscribe(const char *strUserJid_, const char *strFriendJid_);
		XXMPPAPI_DECLARE(XmppResult_t) XmppUnsubscribe(const char *strUserJid_, const char *strFriendJid_);

		XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackLogin(const char *strUserJid_, PXmppOnLogin pfunLogin_);
		XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackClose(const char *strUserJid_, PXmppOnClose pfunClose_);
		XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackPresence(const char *strUserJid_, PXmppOnPresence pfunPresence_);
		XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackMsgReceive(const char *strUserJid_, PXmppOnMsgReceive pfunMsgReceive_);

#ifdef __cplusplus  
	}  
#endif
	
	//////////////////////////////////////////////////////////////////////////
	XXMPPAPI_DECLARE(XmppResult_t) XmppLoginDebug(const char *strUserJid_, const char *strPsw_, 
		const char *strIp_, int nPort_, const wchar_t *strLogFile_,
		xugd::clib::XLogFile::PFunGetLogCallback pFun_, bool bDenyLogFile_=true);

} // xmpp
} // xugd

#endif // XUGD_XMPP_H