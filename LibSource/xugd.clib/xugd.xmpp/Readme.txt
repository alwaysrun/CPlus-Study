gloox 1.0.20:
1. gloox.h(713) add: ConnXugdTimeOut,	// Remember add ErrorInfo of xugd.xmpp 
2. connectiontcpclient.cpp(137) add: ConnectionTCPClient::recv()
		if( !dataAvailable( timeout ) )
		{
		  m_recvMutex.unlock();
		  return ConnXugdTimeOut;
		}

	(172)recv
    m_buf[size] = '\0';

	if(m_cancel)// add by xugd
		return ConnNotConnected;

3.util.h(112) add: ForEach - for-cirlce
		unsigned int classAddr = (unsigned int)(*it);
		if( classAddr==0xfeeefeee )
			  continue;

4.client.cpp(130) add: ~Client() set to 0 after delete.
5.connectiontcpbase.cpp add:
	(81)~ConnectionTCPBase: m_handler=0;
	(168) cleanup:
	  m_state = StateDisconnected;
	  m_cancel = true;
	  m_totalBytesIn = 0;
	  m_totalBytesOut = 0;