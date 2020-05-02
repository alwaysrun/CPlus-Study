#pragma once
#ifndef XUGD_CLIB_RANDOM_H
#define XUGD_CLIB_RANDOM_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.random.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: ���������������
// @VERSION: 1.0.15.907
//
// @RECORD:
//	��д��Xugd��2015-09-07����ʼ�汾��
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>

// ʹ��<boost/random.hpp>��mt19937���ɵ������
namespace xugd{	namespace clib{	
	class XCLIBCLASS_DECLARE XRandom{
	public: 
		// ����[Min, Max]��������
		static int GetInt(int nMin_=0, int nMax_=100);

		// ���������true��false
		static bool GetBool();

		// ������[32, 126]ASCII����ɵ�����ַ���
		static std::string GetString(int nLen_);

		static void GetBuff(unsigned char *pByte_, int nLen_);

		// ����[-128, 127]��ɵ��ֽ����У���ͨ��string.data��ȡ
		static std::string GetBuff(int nLen_);
	};
} //clib
} // xugd
#endif //XUGD_CLIB_RANDOM_H