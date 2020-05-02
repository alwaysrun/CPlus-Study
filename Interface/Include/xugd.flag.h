#pragma once
#ifndef XUGD_CLIB_FLAG_H
#define XUGD_CLIB_FLAG_H
//////////////////////////////////////////////////////////////////////////
// @FILE: xugd.flag.h
// @LIB: xugd.clib.lib/xugd.clib_d.lib
// @DESCRIPTION: 定义标志位操作类
// @VERSION: 1.0.15.907
//
// @RECORD:
//	编写：Xugd，2015-09-07，初始版本。
//////////////////////////////////////////////////////////////////////////
#include "xugd.common.h"

namespace xugd{	namespace clib{
	class XCLIBCLASS_DECLARE XFlag 
	{
	public: 

		// 是否包含检测的位数
		template<typename T>
		static bool Check(T tFlag_, T toCheck_)
		{
			return (toCheck_!=0) && ((tFlag_&toCheck_)==toCheck_);
		}

		// 是否包含检测的数中的任一位或多位
		template<typename T>
		static bool CheckAny(T tFlag_, T toCheck_)
		{
			return (0!=(tFlag_&toCheck_));
		}

		// 设定位
		template<typename T>
		static void Set(T &tFlag_, T toSet_)
		{
			tFlag_ = (T)(tFlag_ | toSet_);
		}

		// 移除位
		template<typename T>
		static void Clear(T tFlag_, T toClear_)
		{
			tFlag_ = (T)(tFlag_ & (~toClear_));
		}

		// 异或
		template<typename T>
		static T Diff(T tFirst_, T tSecond_)
		{
			return (T)(tFirst_ ^ tSecond_);
		}
	};
} //clib
} // xugd
#endif //XUGD_CLIB_FLAG_H