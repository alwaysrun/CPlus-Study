#pragma once
#ifndef XUGD_CLIB_RANDOM_H
#define XUGD_CLIB_RANDOM_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.random.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义随机数生成类
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"
#include <string>

// 使用<boost/random.hpp>中mt19937生成的随机数
namespace xugd{	namespace clib{	
	class XCLIBCLASS_DECLARE XRandom{
	public: 
		// 返回[Min, Max]间的随机数
		static int GetInt(int nMin_=0, int nMax_=100);

		// 生成随机的true与false
		static bool GetBool();

		// 生成由[32, 126]ASCII码组成的随机字符串
		static std::string GetString(int nLen_);

		static void GetBuff(unsigned char *pByte_, int nLen_);

		// 生成[-128, 127]组成的字节序列，可通过string.data获取
		static std::string GetBuff(int nLen_);
	};
} //clib
} // xugd
#endif //XUGD_CLIB_RANDOM_H