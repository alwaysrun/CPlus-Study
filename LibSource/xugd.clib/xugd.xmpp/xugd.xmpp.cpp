#include "stdafx.h"
#include "xugd.xmpp.h"
#include <sstream>

#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "gloox.h"
#include "client.h"
#include "disco.h"
#include "message.h"
#include "eventhandler.h"
#include "loghandler.h"
#include "messagehandler.h"
#include "messagesessionhandler.h"
#include "rosterlistener.h"
#include "connectionlistener.h"
#include "connectiontcpclient.h"
#include "messagesession.h"
#include "rostermanager.h"

#include "xugd.log.h"
#include "xugd.thread.h"
#include "xugd.str.h"

#ifdef _DEBUG
#pragma comment(lib, "xugd.clib_d.lib")
#else
#pragma comment(lib, "xugd.clib.lib")
#endif

namespace xugd{ namespace xmpp{
	namespace{
		using namespace gloox;
		using namespace xugd::clib;

		const char* GetErrorInfo(ConnectionError euError_){
			static const char* ErrorInfo[]={
				"NoError",              
				"StreamError",          
				"StreamVersionError",   
				"StreamClosed",         
				"ProxyAuthRequired",    
				"ProxyAuthFailed",      
				"ProxyNoSupportedAuth", 
				"IoError",              
				"ParseError",           
				"ConnectionRefused",    
				"DnsError",             
				"OutOfMemory",          
				"NoSupportedAuth",      
				"TlsFailed",            
				"TlsNotAvailable",     
				"CompressionFailed",    
				"AuthenticationFailed", 
				"UserDisconnected",     
				"NotConnected",
				"RevTimeout",
			};

			if(euError_<ConnNoError || euError_>ConnNotConnected)
				euError_ = ConnNotConnected;
			return ErrorInfo[euError_];
		}

		const char* GetSubscribeType(SubscriptionType euType_){
			static const char* SubType[]={
				"None",             
				"NoneOut",      
				"NoneIn",       
				"NoneOutIn",    
				"To",           
				"ToIn",         
				"From",         
				"FromOut",      
				"Both"
			};

			if(euType_<S10nNone || euType_>S10nBoth)
				euType_ = S10nNone;
			return SubType[euType_];
		}

		const char* GetPresenceType(Presence::PresenceType euType_){
			static const char* PreInfo[]={
				"Available",      
				"Chat",           
				"Away",           
				"DND",            
				"XA",             
				"Unavailable",    
				"Probe",          
				"Error",          
				"Invalid"
			};

			if(euType_<Presence::Available || euType_>Presence::Invalid)
				euType_ = Presence::Invalid;
			return PreInfo[euType_];
		}
		
		class XPingHandler:public EventHandler
		{
		public:
			XPingHandler():m_nCount(0)
			{ }
			virtual ~XPingHandler()
			{ }

			virtual void handleEvent(const Event& event)
			{
				//std::string sEvent;
				switch (event.eventType())
				{
				case Event::PingPing:	//! 收到PING消息
					//sEvent = "PingPing";
					break;
				case Event::PingPong:	//! 收到返回PONG消息,心跳累计次数减1
					//sEvent = "PingPong";
					//DecreaceCount();
					ResetCount();
					break;
				case Event::PingError:	//! 
					//sEvent = "PingError";
					break;
				default:
					break;
				}

				//TRACE("handleEvent:-------------%s\n", sEvent.c_str());
				return;
			}

			// Ret: false, too many time of noResponse
			bool IncreaseCount(void){
				if(++m_nCount > 3)
					return false;	// Too many Ping time, may disonnect.

				return true;
			}

			void ResetCount(){
				m_nCount = 0;
			}

		private:
			int		m_nCount;

			void DecreaceCount(void)
			{
				if(m_nCount>0)
					--m_nCount;
			}
		};

		class XMessageClient : public MessageSessionHandler, public MessageHandler, 
			public RosterListener, ConnectionListener, LogHandler
		{
		public:
			XMessageClient(const string &strName_, const string &strPsw_, const wchar_t *pLogFile_) {
				Init(strName_, strPsw_, pLogFile_);

				m_pClient->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
			}

			void SetLogCallback(XLogFile::PFunGetLogCallback pFun_, bool bDenyLogFile_){				
				m_logFile.SetGetLogCallback(pFun_, bDenyLogFile_);
			}

			void Init(const string &strName_, const string &strPsw_, const wchar_t *pLogFile_){
				m_bIsStart = false;
				m_bIsStopped = false;
				m_pOnLogin = NULL;
				m_pOnClose = NULL;
				m_pOnPresence = NULL;
				m_pOnMsgReceive = NULL;

				m_strName = strName_;
				m_bLogDebug = false;
				m_nLogLevel = (int)XLogFile::LogDefault;
				m_conError = ConnNotConnected;

				// Logger
				if(!CxWstrNullOrEmpty(pLogFile_)){
					m_logFile.Open(pLogFile_);
					m_logFile.Version("xugd.xmpp", "17.309(g1.0.20)");
				}

				// Client
				JID jid(m_strName);
				m_pClient = new Client(jid, strPsw_);
				m_pClient->registerConnectionListener(this);
				m_pClient->registerMessageSessionHandler(this);
				// m_pClient->registerMessageHandler(this);
				m_pClient->rosterManager()->registerRosterListener(this);
				m_pClient->disco()->setVersion("xmpp", GLOOX_VERSION);
				//m_pClient->disco()->setIdentity("client", "bot");
				//m_pClient->disco()->addFeature( XMLNS_CHAT_STATES );
				m_pClient->setResource("netim");
				//m_pClient->setPresence(Presence::Available, 10);
				m_pClient->setTls(TLSDisabled);
			}

			virtual ~XMessageClient() {
				m_bIsStart = false;
				m_bIsStopped = true;
				m_pOnLogin = NULL;
				m_pOnPresence = NULL;
				m_pOnMsgReceive = NULL;
				m_pOnClose = NULL;
				m_logFile.SetGetLogCallback(NULL);

				if(CxNotNull(m_pClient)){
					if(IsConnected()){
						m_pClient->disconnect();
					}

					m_pClient->logInstance().removeLogHandler(this);
					m_pClient->rosterManager()->removeRosterListener();
					m_pClient->registerMessageSessionHandler(NULL);
					m_pClient->removeConnectionListener(this);

					XThread::Sleep(1050); // receive max-wait is 1000
					//delete m_pClient;
					m_pClient = NULL;
				}
				m_logFile.Info("XMessageClient destructor");
				m_logFile.Close();

				XThread::Sleep(50); 
			}

			void SetLogLevel(int nLevel_){
				if(CxIsEqual(nLevel_, m_nLogLevel))
					return;

				if(m_logFile.SetLogLevel((XLogFile::LogLevel)nLevel_)){
					m_nLogLevel = nLevel_;

					bool bDebug = nLevel_ > XLogFile::LogInfo;
					if(CxIsEqual(m_bLogDebug, bDebug))
						return;

					m_bLogDebug = bDebug;

					// if reset, can not connect server
					//if(CxNotNull(m_pClient)){
					//	m_pClient->logInstance().removeLogHandler(this);
					//	m_pClient->logInstance().registerLogHandler(m_bLogDebug?LogLevelDebug:LogLevelWarning, LogAreaAll, this);
					//}
				}
			}

			void SetCallbackLogin(PXmppOnLogin pfunLogin_){
				m_logFile.Called("%s %s Callback-Login", m_strName.c_str(), 
					CxIsNull(pfunLogin_)?"Remove":"Set");

				m_pOnLogin = pfunLogin_;
			}
			void SetCallbackClose(PXmppOnClose pfunClose_){
				m_logFile.Called("%s %s Callback-Close", m_strName.c_str(), 
					CxIsNull(pfunClose_)?"Remove":"Set");

				m_pOnClose = pfunClose_;
			}
			void SetCallbackPresence(PXmppOnPresence pfunPresence_){
				m_logFile.Called("%s %s Callback-Presence", m_strName.c_str(), 
					CxIsNull(pfunPresence_)?"Remove":"Set");

				m_pOnPresence = pfunPresence_;
			}
			void SetCallbackMsgReceive(PXmppOnMsgReceive pfunMsgReceive_){
				m_logFile.Called("%s %s Callback-MsgReceive", m_strName.c_str(), 
					CxIsNull(pfunMsgReceive_)?"Remove":"Set");

				m_pOnMsgReceive = pfunMsgReceive_;
			}

			XmppResult_t start(const string strIp_, int nPort_)
			{
				m_logFile.Called("start(%s:%d)", strIp_.c_str(), nPort_);
				if(IsConnected())
					m_pClient->disconnect();

				m_pClient->setServer(strIp_);
				m_pClient->setPort(nPort_);

				m_strServer = strIp_;
				m_nPort = nPort_;

				m_bCheckedDomain = false;
				m_bIsStopped = false;
				m_conError = ConnNoError;
				if(!m_pClient->connect(false))
				{
					if(CxIsEqual(m_conError, ConnNoError))
						m_conError = ConnNotConnected;
				}
				else{ // Connected
					XThread::Start(XMessageClient::ToReceive, this);
					if(!m_evtLogin.TimeWait(0, 0, 30)){
						m_logFile.Error("%s login %s time-out(30s)", m_strName.c_str(), strIp_.c_str());
						return XmppRes_Timeout;
					}
				}

				if(CxIsEqual(m_conError, ConnNoError)){
					m_bIsStart = true;
					m_logFile.Info("%s login %s: Success", m_strName.c_str(), strIp_.c_str());

					//XThread::Sleep(20);
				}
				else{
					m_bIsStart = false;
					m_logFile.Error("%s login %s: %s[%d]", m_strName.c_str(), strIp_.c_str(), GetErrorInfo(m_conError), m_conError);
				}
				return (XmppResult_t)m_conError;
			}

			void stop(){
				m_logFile.Called("stop(): %s disconnect", m_strName.c_str());

				m_bIsStart = false;
				if(IsConnected())
				{
					m_pClient->disconnect();
					m_bIsStopped = true;
					m_logFile.Debug("Stopped");
				}
			}

			bool IsStopped()
			{
				return m_bIsStopped;
			}

			bool IsConnected(){
				ConnectionBase* pConn = m_pClient->connectionImpl();
				return (CxNotNull(pConn) && (pConn->state()>= StateConnecting));
			}

			string GetRemoteAddress(){
				ostringstream strOut;
				strOut<<m_pClient->server()<<":"<<m_pClient->port();
				return strOut.str();
			}

			string GetStartStream(int nSize_)
			{
				if(nSize_ > m_strStartMsg.length())
					return m_strStartMsg;

				return m_strStartMsg.substr(0, nSize_-1);
			}

			string GetLocalAddress(){
				ConnectionBase *pConn = m_pClient->connectionImpl();
				if(CxIsNull(pConn))
					return "";

				ostringstream strOut;
				strOut<<pConn->localInterface()<<":"<<pConn->localPort();
				return strOut.str();
			}

			void subscribe(const string &strFriend_){
				if(m_bIsStart){
					if(IsRoster(strFriend_))
					{
						m_logFile.Notice("%s roster %s has existed, just return", 
							m_strName.c_str(), strFriend_.c_str());
					}
					else
					{
						XMutexScopLock lker(m_synDataSend);
						m_pClient->rosterManager()->subscribe(strFriend_);
					}
				}
			}

			void unsubscribe(const string &strFriend_){
				// m_pClient->rosterManager()->unsubscribe(strFriend_);
				if(m_bIsStart)
				{
					XMutexScopLock lker(m_synDataSend);
					m_pClient->rosterManager()->remove(strFriend_);
				}
			}

			XmppResult_t sendto(const string& strReceiver_, const string &strMsg_){
				if(!m_bIsStart)
					return (XmppResult_t)m_conError;

				//string strName = boost::to_lower_copy(strReceiver_);
				return SendBySession(strReceiver_, strMsg_);
			}

			void OnPingFailed(){
				m_logFile.Notice("%s disconnected from %s: no response of Ping", 
					m_strName.c_str(), m_strServer.c_str());

				if(m_bIsStart && CxNotNull(m_pOnClose))
					m_pOnClose(m_strName.c_str(), XmppRes_NoPong);
			}

			string GetSrvJid(){
				string::size_type nPos = m_strName.find('@');
				if(string::npos == nPos) return "";

				return m_strName.substr(nPos+1);
			}

			//////////////////////////////////////////////////////////////////////////
			// Override
			virtual void onConnect()
			{
				m_evtLogin.Notify();

				m_logFile.Notice("%s connected to %s:%d", m_strName.c_str(), m_strServer.c_str(), m_nPort);
				if(m_bIsStart && CxNotNull(m_pOnLogin))
					m_pOnLogin(m_strName.c_str());
			}

			virtual void onDisconnect(ConnectionError euErr_) { 
				bool bInvoke = m_bIsStart;
				m_bIsStart = false;
				m_bIsStopped = true;
				m_conError = euErr_;

				{
					m_evtLogin.Notify();

					m_logFile.Notice("%s disconnected from %s: %s[%d]", 
						m_strName.c_str(), m_strServer.c_str(), GetErrorInfo(euErr_), euErr_);

					if(bInvoke && CxNotNull(m_pOnClose))
						m_pOnClose(m_strName.c_str(), (XmppResult_t)euErr_);
				}

				ClearRoster();
				ClearSession();
			}

			virtual void handleLog(LogLevel level, LogArea area, const std::string& message)
			{
				XLogFile::LogLevel euLevel = XLogFile::LogInfo;
				switch(level){
				case LogLevelDebug:
					if(!m_bCheckedDomain)
					{
						if(XAstrStartWith(message, "<stream:stream"))
						{
							m_bCheckedDomain = true;
							m_strStartMsg = message;
							euLevel = XLogFile::LogNotice;
							break;
						}
					}
					if(!m_bLogDebug) return;
					euLevel = XLogFile::LogDebug;
					break;
				case LogLevelWarning:
					euLevel = XLogFile::LogWarn;
					break;
				case LogLevelError:
					euLevel = XLogFile::LogError;
					break;
				}

				m_logFile.Write(message, euLevel);
			}

			virtual void handleMessageSession(MessageSession *session)
			{
				AddSession(session);
			}

			virtual void handleMessage(const Message& stMsg_, MessageSession *pSession_)
			{
				if(m_bLogDebug)
					m_logFile.Debug("<-[Type:%d, %s->%s]: %s", stMsg_.subtype(), stMsg_.from().bare().c_str(), m_strName.c_str(), stMsg_.body().c_str());

				if(m_bIsStart && CxNotNull(m_pOnMsgReceive)){
					string strReceived = stMsg_.body();
					m_pOnMsgReceive(m_strName.c_str(), stMsg_.from().bare().c_str(), strReceived.data(), strReceived.length(), (XmppMsgType_t)stMsg_.subtype());
				}
			}

			virtual void handleRosterPresence(const RosterItem& item, const std::string& /*resource*/,
				Presence::PresenceType presence, const std::string& /*msg*/)
			{
				if(!m_bIsStart) return;

				m_logFile.Info("%s roster %s presence changed to %s", m_strName.c_str(), 
					item.jidJID().bare().c_str(), GetPresenceType(presence));

				if(presence == Presence::Available)
					AddRoster(item.jidJID().bare());
				else if(presence>Presence::Chat)
					RemoveSessionByTarget(item.jidJID().bare());

				if(CxNotNull(m_pOnPresence))
					m_pOnPresence(m_strName.c_str(), item.jidJID().bare().c_str(), (XmppPresenceType_t)presence);
			}

			virtual bool handleSubscriptionRequest(const JID& jid, const std::string& /*msg*/)
			{
				if(m_bIsStart) {
					m_logFile.Notice("%s subscribe request from %s", m_strName.c_str(), jid.bare().c_str());

					//if(!IsRoster(jid.bare()))
					{
						StartSubscribeThread(jid.bare());
					}
					//m_pClient->rosterManager()->subscribe(jid.bare());
				}
				return true;
			}

			virtual bool handleUnsubscriptionRequest(const JID& jid, const std::string& /*msg*/)
			{
				m_logFile.Notice("%s unsubscribe request from %s", m_strName.c_str(), jid.bare().c_str());
				return true;
			}

			virtual void handleNonrosterPresence(const Presence& presence)
			{
				if(m_bIsStart){
					m_logFile.Notice("handleNonrosterPresence(%s)", presence.from().bare().c_str());

					StartSubscribeThread(presence.from().bare());
					//m_pClient->rosterManager()->subscribe(presence.from().bare());
				}
			}

			virtual bool onTLSConnect(const CertInfo& info)
			{
				m_logFile.Info("%s onTLSConnect-status: %d; issuer: %s; peer: %s; protocol: %s; mac: %s; cipher: %s",
					m_strName.c_str(),
					info.status, info.issuer.c_str(), info.server.c_str(),
					info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str());
				return true;
			}

			virtual void onResourceBindError(ResourceBindError error)
			{
				m_logFile.Error("%s onResourceBindError: %d", m_strName.c_str(), error);
			}

			virtual void onSessionCreateError(SessionCreateError error)
			{
				m_logFile.Error("%s onSessionCreateError: %d", m_strName.c_str(), error);
			}

			virtual void handleItemSubscribed(const JID& jid)
			{
				m_logFile.Notice("%s subscribed-item %s", m_strName.c_str(), jid.bare().c_str());
			}

			virtual void handleItemAdded(const JID& jid)
			{
				m_logFile.Notice("%s added item %s", m_strName.c_str(), jid.bare().c_str());
			}

			virtual void handleItemUnsubscribed(const JID& jid)
			{
				m_logFile.Notice("%s unsubscribe item %s", m_strName.c_str(), jid.bare().c_str());
			}

			virtual void handleItemRemoved(const JID& jid)
			{
				m_logFile.Notice("%s removed item %s", m_strName.c_str(), jid.bare().c_str());
			}

			virtual void handleItemUpdated(const JID& jid)
			{
				m_logFile.Notice("%s updated item %s", m_strName.c_str(), jid.bare().c_str());
			}

			virtual void handleRoster(const Roster& roster)
			{
				int i=0;
				string strRoster;
				Roster::const_iterator it = roster.begin();
				for(; it != roster.end(); ++it)
				{
					if(++i>50)
					{
						strRoster += "; ...";
						break;
					}

					if(i>1)
						strRoster += "; ";
					strRoster += (*it).second->jidJID().bare();
					strRoster += ", ";
					strRoster += GetSubscribeType((*it).second->subscription());

					//if((*it).second->subscription() == S10nBoth)
					//	AddRoster((*it).second->jidJID().bare());
				}

				m_logFile.Info("%s\'s Roster Items(%d): %s", m_strName.c_str(), roster.size(), strRoster.c_str());
			}

			virtual void handleRosterError(const IQ& /*iq*/)
			{
				m_logFile.Error("%s roster-related error occurred", m_strName.c_str());
			}

			virtual void handleSelfPresence(const RosterItem& item, const std::string& resource,
				Presence::PresenceType presence, const std::string& /*msg*/)
			{
				//m_logFile.Info("%s self presence from %s/%s changed to %s", m_strName.c_str(), 
				//	item.jidJID().full().c_str(), resource.c_str(), GetPresenceType(presence));
			}

		private: // roster
			typedef std::vector<string>::iterator VecRosterIt;
			XMutex m_synRoster;
			std::vector<string> m_vecRosters;
			void AddRoster(string strUser_){
				XMutexScopLock lker(m_synRoster);
				VecRosterIt fIt = find(m_vecRosters.begin(), m_vecRosters.end(), strUser_);
				if(m_vecRosters.end() == fIt){
					m_vecRosters.push_back(strUser_);
				}
			}

			bool IsRoster(string strUser_){
				XMutexScopLock lker(m_synRoster);
				VecRosterIt fIt = find(m_vecRosters.begin(), m_vecRosters.end(), strUser_);
				return (m_vecRosters.end() != fIt);
			}

			void ClearRoster(){
				XMutexScopLock lker(m_synRoster);
				m_vecRosters.clear();
			}

			//////////////////////////////////////////////////////////////////////////			
			struct SubscribeInfo{
				Client *pClient;
				string strFriend;

				SubscribeInfo(Client *pCli_, const string &strToSub_)
				{
					pClient = pCli_;
					strFriend = strToSub_;
				}
			};
			void StartSubscribeThread(const string &strToSub_){
				SubscribeInfo *pInfo = new SubscribeInfo(m_pClient, strToSub_);
				XThread::Start(ToSubscribe, pInfo);
			}
			static void ToSubscribe(void *pParam_){
				SubscribeInfo *pInfo = (SubscribeInfo*)pParam_;
				if(CxNotNull(pInfo) && CxNotNull(pInfo->pClient)){
					pInfo->pClient->rosterManager()->subscribe(pInfo->strFriend);
				}

				delete pInfo;
			}

		private: // MessageSession
			typedef std::map<string, MessageSession*>::iterator MsgSessionIt;
			XMutex m_synDataSend;
			std::map<string, MessageSession*> m_mapSessions;

			static void ToReceive(void *pRevParam_){
				try
				{
					XMessageClient *pMsgClient=(XMessageClient*)pRevParam_;
					if(CxIsNull(pMsgClient) || CxIsNull(pMsgClient->m_pClient))
						return;

					const int RevTimeout = 30 * 1000000; // 30s
					Client *pClient = pMsgClient->m_pClient;
					ConnectionError revResult = ConnNoError;

					JID  jidSrv(pMsgClient->GetSrvJid());
					tr1::shared_ptr<XPingHandler> pHandler(new XPingHandler());
					while(ConnNoError == revResult){
						revResult = pClient->recv(RevTimeout);
						if(pMsgClient->IsStopped()) break;

						if(ConnXugdTimeOut == revResult){
							if(!pHandler->IncreaseCount()){	// Error
								pMsgClient->OnPingFailed();
								break;;
							}

							{
								pClient->xmppPing(jidSrv, pHandler.get());
							}
							revResult = ConnNoError;
						}
					} // while
				} // try
				catch(...){} // ClientBase::notifyOnDisconnect may fail as INVALID_POINTER_READ_FILL_PATTERN_feeefeee
			}

			void AddSession(MessageSession *pSession_){
				if(!m_bIsStart) {
					//if(CxNotNull(m_pClient))
					//	m_pClient->disposeMessageSession(pSession_);
					return;
				}

				XMutexScopLock lker(m_synDataSend);
				string strTarget = boost::to_lower_copy(pSession_->target().bare());
				MsgSessionIt fIt = m_mapSessions.find(strTarget);
				if(m_mapSessions.end() != fIt){
					m_pClient->disposeMessageSession(pSession_);
					return;		// has added
				}

				pSession_->registerMessageHandler(this);
				m_mapSessions[strTarget]=pSession_;
			}

			void RemoveSessionByTarget(const string &strName_){
				XMutexScopLock lker(m_synDataSend);
				MsgSessionIt fIt = m_mapSessions.find(strName_);
				if(m_mapSessions.end() != fIt){
					(fIt->second)->removeMessageHandler();
					m_pClient->disposeMessageSession(fIt->second);
					m_mapSessions.erase(fIt);
				}
			}

			void ClearSession(){
				XMutexScopLock lker(m_synDataSend);
				try
				{
					for(MsgSessionIt fIt=m_mapSessions.begin(); fIt!=m_mapSessions.end(); ++fIt){
						(fIt->second)->removeMessageHandler();
						m_pClient->disposeMessageSession(fIt->second);
					}
				}
				catch(...){}
				m_mapSessions.clear();
			}

			 XmppResult_t SendBySession(const string &strName_, const string &strMsg_){
				XMutexScopLock lker(m_synDataSend);

				MessageSession *pSession = NULL;
				MsgSessionIt fIt = m_mapSessions.find(strName_);
				if(m_mapSessions.end() != fIt)
				{
					pSession = fIt->second;
				}
				else
				{
					if(m_bIsStart)
					{
						string strFull = strName_ + "/netim";
						pSession=new MessageSession(m_pClient, strFull);
						pSession->registerMessageHandler(this);
						m_mapSessions[strName_]=pSession;
					}
				}

				if(!m_bIsStart) return (XmppResult_t)m_conError;

				pSession->send(strMsg_);
				return XmppRes_Success;
			}
			
		private:
			Client *m_pClient;
			XLogFile m_logFile;
			XAutoEvent m_evtLogin;
			ConnectionError m_conError;
			
			PXmppOnLogin m_pOnLogin;
			PXmppOnClose m_pOnClose;
			PXmppOnPresence m_pOnPresence;
			PXmppOnMsgReceive m_pOnMsgReceive;

			bool m_bCheckedDomain;
			bool m_bIsStopped;
			bool m_bIsStart;
			bool m_bLogDebug;
			int m_nLogLevel;
			int m_nPort;
			string m_strStartMsg;
			string m_strName;
			string m_strServer;
		};

		//////////////////////////////////////////////////////////////////////////
		// Globle-var
		typedef tr1::shared_ptr<XMessageClient> XMsgClientPtr;
		typedef std::map<string, XMsgClientPtr>::iterator XMsgClientIt; 
		XMutex g_synClients;
		std::map<string, XMsgClientPtr> g_mapClients;

		XMsgClientPtr GetClient(const string &strUserJid_){
			XMutexScopLock lker(g_synClients);
			XMsgClientIt fIt = g_mapClients.find(strUserJid_);
			if(g_mapClients.end() == fIt)
				return NULL;

			return fIt->second;
		}

		void AddClient(const string &strUserJid_, XMsgClientPtr pClient_){
			XMutexScopLock lker(g_synClients);
			g_mapClients[strUserJid_] = pClient_;
		}

		XMsgClientPtr RemoveClient(const string &strUserJid_){
			XMutexScopLock lker(g_synClients);
			XMsgClientIt fIt = g_mapClients.find(strUserJid_);
			if(g_mapClients.end() == fIt)
				return NULL;

			XMsgClientPtr pClient = fIt->second;
			g_mapClients.erase(fIt);
			return pClient;
		}

	}// ns:anonymous

	//////////////////////////////////////////////////////////////////////////
	XXMPPAPI_DECLARE(XmppResult_t) XmppLogin(const char *strUserJid_, const char *strPsw_, 
		const char *strIp_, int nPort_, const wchar_t *strLogFile_){
			if(CxIsNull(strUserJid_) || CxIsNull(strPsw_) || CxIsNull(strIp_))
				return XmppRes_ParamIsNULL;

			string strName = strUserJid_;
			boost::to_lower(strName);
			XMsgClientPtr pClient = GetClient(strName);
			if(!pClient){
				pClient = tr1::make_shared<XMessageClient>(strName, strPsw_, strLogFile_);
				AddClient(strName, pClient);
			}
			if(pClient->IsConnected()) // has login
				return XmppRes_HasLogin;

			// Add 
			XmppResult_t euResult = pClient->start(strIp_, nPort_);
			if(CxNotEqual(euResult, XmppRes_Success))
				return euResult;
			return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(void) XmppClose(const char *strUserJid_){
		if(CxIsNull(strUserJid_))
			return;

		XMsgClientPtr pClient = RemoveClient(strUserJid_);
		if(pClient){
			pClient->stop();
		}
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppGetStartStream(const char *strUserJid_, char *strStream_, int nSize_)
	{
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		string strStart = pClient->GetStartStream(nSize_);
		strcpy_s(strStream_, nSize_, strStart.c_str());
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppGetRemoteAddress(const char *strUserJid_, char *strAddress_, int nSize_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;
		string strRemote = pClient->GetRemoteAddress();
		strcpy_s(strAddress_, nSize_, strRemote.c_str());
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppGetLocalAddress(const char *strUserJid_, char *strAddress_, int nSize_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;
		string strLocal = pClient->GetLocalAddress();
		if(strLocal.empty()) 
			return XmppRes_NotConnected;
		strcpy_s(strAddress_, nSize_, strLocal.c_str());
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppSetLogLevel(const char *strUserJid_, int nLevel_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->SetLogLevel(nLevel_);
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppSendMsg(const char *strUserJid_, const char *strToJid_, const char *strUtf8Msg_){
		if(CxIsNull(strUserJid_) || CxIsNull(strToJid_) || CxIsNull(strUtf8Msg_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		return pClient->sendto(strToJid_, strUtf8Msg_);
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppSendByteMsg(const char *strUserJid_, const char *strToJid_, const char *pUtf8_, int nCount_){
		if(CxIsNull(strUserJid_) || CxIsNull(strToJid_) || CxIsNull(pUtf8_) || nCount_<=0)
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		string strMsg(pUtf8_, nCount_);
		return pClient->sendto(strToJid_, strMsg);
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppSubscribe(const char *strUserJid_, const char *strFriendJid_){
		if(CxIsNull(strUserJid_) || CxIsNull(strFriendJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->subscribe(strFriendJid_);
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppUnsubscribe(const char *strUserJid_, const char *strFriendJid_){
		if(CxIsNull(strUserJid_) || CxIsNull(strFriendJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->unsubscribe(strFriendJid_);
		return XmppRes_Success;
	}

	XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackLogin(const char *strUserJid_, PXmppOnLogin pfunLogin_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->SetCallbackLogin(pfunLogin_);
		return XmppRes_Success;
	}
	XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackClose(const char *strUserJid_, PXmppOnClose pfunClose_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->SetCallbackClose(pfunClose_);
		return XmppRes_Success;
	}
	XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackPresence(const char *strUserJid_, PXmppOnPresence pfunPresence_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->SetCallbackPresence(pfunPresence_);
		return XmppRes_Success;
	}
	XXMPPAPI_DECLARE(XmppResult_t) XmppSetCallbackMsgReceive(const char *strUserJid_, PXmppOnMsgReceive pfunMsgReceive_){
		if(CxIsNull(strUserJid_))
			return XmppRes_ParamIsNULL;

		XMsgClientPtr pClient = GetClient(strUserJid_);
		if(!pClient)
			return XmppRes_NotFound;

		pClient->SetCallbackMsgReceive(pfunMsgReceive_);
		return XmppRes_Success;
	}
	
	XXMPPAPI_DECLARE(XmppResult_t) XmppLoginDebug(const char *strUserJid_, const char *strPsw_, 
		const char *strIp_, int nPort_, const wchar_t *strLogFile_,
		xugd::clib::XLogFile::PFunGetLogCallback pFun_, bool bDenyLogFile_){
			if(CxIsNull(strUserJid_) || CxIsNull(strPsw_) || CxIsNull(strIp_))
				return XmppRes_ParamIsNULL;

			string strName = strUserJid_;
			boost::to_lower(strName);
			XMsgClientPtr pClient = GetClient(strName);
			if(pClient && pClient->IsConnected()) // has login
				return XmppRes_HasLogin;

			// Add 
			pClient = tr1::make_shared<XMessageClient>(strName, strPsw_, strLogFile_);
			pClient->SetLogCallback(pFun_, bDenyLogFile_);
			pClient->SetLogLevel((int)XLogFile::LogAll);

			XmppResult_t euResult = pClient->start(strIp_, nPort_);
			if(CxNotEqual(euResult, XmppRes_Success))
				return euResult;
			AddClient(strName, pClient);
			return XmppRes_Success;
	}

} // xmpp
} // xugd