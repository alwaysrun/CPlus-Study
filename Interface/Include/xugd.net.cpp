#include "xugd.net.h"
#include "xugd.exception.h"
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace xugd{	namespace clib{
//using namespace std;
//////////////////////////////////////////////////////////////////////////
// ÍøÂç²Ù×÷
//////////////////////////////////////////////////////////////////////////
std::string XuNet::peerAddress(SOCKET sock_, int *pPort_) {
	struct sockaddr_in addr;
	int addrLen = sizeof(addr);
	if (getpeername(sock_, (struct sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
		int nError = WSAGetLastError();
		throw XuException("getpeername fail", nError);
	}

	char szBuff[INET6_ADDRSTRLEN] = { 0 };
	if (inet_ntop(addr.sin_family, &addr.sin_addr, szBuff, INET6_ADDRSTRLEN) == nullptr) {
		int nError = WSAGetLastError();
		throw XuException("inet_ntop fail", nError);
	}
	if(nullptr!= pPort_)
		*pPort_ = ntohs(addr.sin_port);

	return szBuff;
}

std::string XuNet::localAddress(SOCKET sock_, int *pPort_) {
	struct sockaddr_in addr;
	int addrLen = sizeof(addr);
	if (getsockname(sock_, (struct sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
		int nError = WSAGetLastError();
		throw XuException("getpeername fail", nError);
	}

	char szBuff[INET6_ADDRSTRLEN] = { 0 };
	if (inet_ntop(addr.sin_family, &addr.sin_addr, szBuff, INET6_ADDRSTRLEN) == nullptr) {
		int nError = WSAGetLastError();
		throw XuException("inet_ntop fail", nError);
	}
	if (nullptr != pPort_)
		*pPort_ = ntohs(addr.sin_port);

	return szBuff;
}

void XuNet::initSocket() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		int nError = WSAGetLastError();
		throw XuException("WSAStartup fail", nError);
	}
}

void XuNet::uninitSocket() {
	WSACleanup();
}

} //clib
} // xugd