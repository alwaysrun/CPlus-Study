#pragma once
#ifndef XUGD_APR_DIR_H
#define XUGD_APR_DIR_H
#include "xugd.apr.h"

namespace xugd{ namespace apr{

	XAPRAPI_DECLARE(void) XDirMake(const char *pDir_);
	XAPRAPI_DECLARE(void) XDirRemove(const char *pDir_);
//////////////////////////////////////////////////////////////////////////
} // apr
} // xugd
#endif // XUGD_APR_DIR_H