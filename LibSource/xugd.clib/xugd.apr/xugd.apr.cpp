#include "xugd.apr.h"
#include "../xugd.clib/xugd.common.h"

#include "apr_pools.h"

#pragma comment(lib, "libapr.lib")
#pragma comment(lib, "xugd.clib.lib")

namespace xugd{ namespace apr{
	static bool s_aprInit = false;
	apr_pool_t* g_xugd_aprPool = NULL;

	static void ToInitApr()
	{
		apr_status_t rv;
		if(AprFail(rv=apr_initialize()))
			throw AprException("apr_initialize", rv);

		if(CxNotNull(g_xugd_aprPool))
		{
			apr_pool_destroy(g_xugd_aprPool);
			g_xugd_aprPool = NULL;
		}
		if(AprFail(rv=apr_pool_create(&g_xugd_aprPool, NULL)))
			throw AprException("apr_pool_create", rv);
	}

	void AprInitCheck()
	{
		if(!s_aprInit)
		{
			ToInitApr();
			s_aprInit = true;
		}
	}

	void AprDllStart()
	{
		try 
		{
			AprInitCheck();
		}
		catch(AprException&){}
	}

	void AprDllStop()
	{
		if(s_aprInit)
		{
			if(CxNotNull(g_xugd_aprPool))
			{
				apr_pool_destroy(g_xugd_aprPool);
				g_xugd_aprPool = NULL;
			}

			apr_terminate();
		}
	}
} // apr
} // xugd