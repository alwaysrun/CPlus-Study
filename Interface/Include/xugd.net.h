#pragma once
#ifndef XUGD_CLIB_NET_H
#define XUGD_CLIB_NET_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.net.h
// @DESCRIPTION: ����������ز�������
// @VERSION: 1.0.20.506
//
// @RECORD:
//	��д��Xugd��2020-05-06����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include <string>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// �������
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