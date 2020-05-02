#pragma once
#ifndef XUGD_CLIB_TIME_H
#define XUGD_CLIB_TIME_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.time.h
// @DESCRIPTION: 定义时间相关操作与类
// @VERSION: 1.0.20.502
//
// @RECORD:
//	编写：Xugd，2020-05-02，初始版本。
//////////////////////////////////////////////////////////////////////////
#include <string>

namespace xugd{	namespace clib{
	//////////////////////////////////////////////////////////////////////////
	// 时间操作
	//////////////////////////////////////////////////////////////////////////
	class XuTime{
	public:
		// 获取时间（本地或系统）字符串(2020-05-02 14:40:31.015)
		static std::string getLocalTime(bool bInlcudeMill = true);
		static std::string getSystemTime(bool bInlcudeMill = true);

	private:
	};

} //clib
} // xugd

#endif	//XUGD_CLIB_TIME_H