#include "ConsoleOut.h"
#include "macros.h"
#include "client.h"
#include "messagesessionhandler.h"
#include "messageeventhandler.h"
#include "messageeventfilter.h"
#include "chatstatehandler.h"
#include "chatstatefilter.h"
#include "connectionlistener.h"
#include "disco.h"
#include "message.h"
#include "gloox.h"
#include "lastactivity.h"
#include "loghandler.h"
#include "logsink.h"
#include "eventhandler.h"
#include "connectiontcpclient.h"
#include "connectionsocks5proxy.h"
#include "connectionhttpproxy.h"
#include "messagehandler.h"

#include "xugd.time.h"

//#ifdef _DEBUG
//#pragma comment(lib, "gloox-1.0_d.lib")
//#else
//#pragma comment(lib, "gloox-1.0.lib")
//#endif
//
using namespace gloox;
using namespace xugd::clib;

class CEventHandler:public EventHandler
{
public:
	CEventHandler();
	virtual ~CEventHandler();
	virtual void handleEvent(const Event& event);
	void increaceHeartBeatCount(void);
	UINT getHeartBeatCount(){return m_nCount;}
private:
	void decreaceHeartBeatCount(void);
	UINT m_nCount;
};

//
class MessageTest : public MessageSessionHandler, ConnectionListener, LogHandler,
	MessageEventHandler, MessageHandler, ChatStateHandler
{
public:
	MessageTest() : m_session( 0 ), m_messageEventFilter( 0 ), m_chatStateFilter( 0 ) {}

	virtual ~MessageTest() {}

	void start()
	{

		JID jid( "cretest@cti-202.net" );
		j = new Client( jid, "xgdxgd" );
		j->registerConnectionListener( this );
		j->registerMessageSessionHandler( this, 0 );
		j->disco()->setVersion( "messageTest", GLOOX_VERSION );
		j->disco()->setIdentity( "client", "bot" );
		j->disco()->addFeature( XMLNS_CHAT_STATES );
		//j->setTls(TLSDisabled);
		j->setServer("192.168.1.202");
		j->setResource("IM");
		//j->setPresence()
		//StringList ca;
		//ca.push_back( "/path/to/cacert.crt" );
		//j->setCACerts( ca );

		j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

		//
		// this code connects to a jabber server through a SOCKS5 proxy
		//
		//       ConnectionSOCKS5Proxy* conn = new ConnectionSOCKS5Proxy( j,
		//                                   new ConnectionTCP( j->logInstance(),
		//                                                      "sockshost", 1080 ),
		//                                   j->logInstance(), "example.net" );
		//       conn->setProxyAuth( "socksuser", "sockspwd" );
		//       j->setConnectionImpl( conn );

		//
		// this code connects to a jabber server through a HTTP proxy through a SOCKS5 proxy
		//
		//       ConnectionTCP* conn0 = new ConnectionTCP( j->logInstance(), "old", 1080 );
		//       ConnectionSOCKS5Proxy* conn1 = new ConnectionSOCKS5Proxy( conn0, j->logInstance(), "old", 8080 );
		//       conn1->setProxyAuth( "socksuser", "sockspwd" );
		//       ConnectionHTTPProxy* conn2 = new ConnectionHTTPProxy( j, conn1, j->logInstance(), "jabber.cc" );
		//       conn2->setProxyAuth( "httpuser", "httppwd" );
		//       j->setConnectionImpl( conn2 );


		if( j->connect( false ) )
		{
			ConnectionError ce = ConnNoError;
			CEventHandler *pEvtHandler = new CEventHandler();

			int nCount = 0;

			while( ce == ConnNoError )
			{
				ce = j->recv(30000000);
				printf( "Received ce: %d\n", ce );
				
				if(ce == ConnXugdTimeOut){
					j->xmppPing(JID("cti-202.net"), pEvtHandler);
					ce = ConnNoError;
				}
			}
			//printf( "ce: %d\n", ce );
		}

		delete( j );
	}

	virtual void onConnect()
	{
		printf( "connected!!!\n" );
	}

	virtual void onDisconnect( ConnectionError e )
	{
		printf( "message_test: disconnected: %d\n", e );
		if( e == ConnAuthenticationFailed )
			printf( "auth failed. reason: %d\n", j->authError() );
	}

	virtual bool onTLSConnect( const CertInfo& info )
	{
		time_t from( info.date_from );
		time_t to( info.date_to );

		printf( "\n[onTLSConnect]-status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
			"from: %s\nto: %s\n",
			info.status, info.issuer.c_str(), info.server.c_str(),
			info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
			info.compression.c_str(), ctime( &from ), ctime( &to ) );
		return true;
	}

	virtual void handleMessage( const Message& msg, MessageSession * /*session*/ )
	{
		printf( "\n[handleMessage]-type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
			msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str() );

		std::string re = "You said:\n> " + msg.body() + "\nI like that statement.";
		std::string sub;
		if( !msg.subject().empty() )
			sub = "Re: " +  msg.subject();

//		m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
//#if defined( WIN32 ) || defined( _WIN32 )
//		Sleep( 1000 );
//#else
//		sleep( 1 );
//#endif
//		m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
//		m_chatStateFilter->setChatState( ChatStateComposing );
#if defined( WIN32 ) || defined( _WIN32 )
		Sleep( 2000 );
#else
		sleep( 2 );
#endif
		m_session->send( re, sub );

		if( msg.body() == "quit" )
			j->disconnect();
	}

	virtual void handleMessageEvent( const JID& from, MessageEventType event )
	{
		printf( "\n[handleMessageEvent]-received event: %d from: %s\n", event, from.full().c_str() );
	}

	virtual void handleChatState( const JID& from, ChatStateType state )
	{
		printf( "\n[handleChatState]-received state: %d from: %s\n", state, from.full().c_str() );
	}

	virtual void handleMessageSession( MessageSession *session )
	{
		printf( "\n[handleMessageSession]-got new session\n");
		// this example can handle only one session. so we get rid of the old session
		//j->disposeMessageSession( m_session );
		if(m_session != session){
			string strTarget = session->target().bare();
			m_session = session;
			m_session->registerMessageHandler( this );
		}
		//m_messageEventFilter = new MessageEventFilter( m_session );
		//m_messageEventFilter->registerMessageEventHandler( this );
		//m_chatStateFilter = new ChatStateFilter( m_session );
		//m_chatStateFilter->registerChatStateHandler( this );
	}

	virtual void handleLog( LogLevel level, LogArea area, const std::string& message )
	{
		XDateNow dtNow;
		printf("[log]-%s: level: %d, area: %d, %s\n", dtNow.ToAString(true), level, area, message.c_str() );
	}

private:
	Client *j;
	MessageSession *m_session;
	MessageEventFilter *m_messageEventFilter;
	ChatStateFilter *m_chatStateFilter;
};

CEventHandler::CEventHandler():m_nCount(0)
{
}

CEventHandler::~CEventHandler()
{
}

void CEventHandler::increaceHeartBeatCount()
{
	m_nCount++;
	return;
}

void CEventHandler::decreaceHeartBeatCount()
{
	if (m_nCount > 0)
	{
		m_nCount--;
	}
	return;
}

void CEventHandler::handleEvent(const Event& event)
{
	std::string sEvent;
	switch (event.eventType())
	{
	case Event::PingPing:	//! 收到PING消息
		sEvent = "PingPing";
		break;
	case Event::PingPong:	//! 收到返回PONG消息,心跳累计次数减1
		sEvent = "PingPong";
		decreaceHeartBeatCount();
		break;
	case Event::PingError:	//! 
		sEvent = "PingError";
		break;
	default:
		break;
	}

	//TRACE("handleEvent:-------------%s\n", sEvent.c_str());
	return;
}

void GlooxConnectTest(){
	MessageTest mTest;
	mTest.start();
}
