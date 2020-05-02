#pragma once
#ifndef XUGD_CLIB_PRIVATE_H
#define XUGD_CLIB_PRIVATE_H
#include "apr_pools.h"

namespace xugd{ namespace apr{
//////////////////////////////////////////////////////////////////////////
	void AprInitCheck();

	void AprDllStart();
	void AprDllStop();
	extern apr_pool_t *g_xugd_aprPool;

//////////////////////////////////////////////////////////////////////////
} // apr
} // xugd
#endif // XUGD_CLIB_PRIVATE_H