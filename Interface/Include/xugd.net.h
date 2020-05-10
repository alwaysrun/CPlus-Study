#pragma once
#ifndef XUGD_CLIB_NET_H
#define XUGD_CLIB_NET_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.net.h
// @DESCRIPTION: 定义网络相关操作与类
// @VERSION: 1.0.20.506
//
// @RECORD:
//	编写：Xugd，2020-05-06，初始版本。
//////////////////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include <string>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// 网络操作
	//////////////////////////////////////////////////////////////////////////
	class XuNet{
	public:
		static void initSocket();
		static void uninitSocket();

		static std::string peerAddress(SOCKET sock_, int *pPort_ = nullptr);
		static std::string localAddress(SOCKET sock_, int *pPort_ = nullptr);

	private:
	};

} //clib
} // xugd

#endif	//XUGD_CLIB_NET_H