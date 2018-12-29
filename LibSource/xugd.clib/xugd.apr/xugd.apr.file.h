#pragma once
#ifndef XUGD_APR_FILE_H
#define XUGD_APR_FILE_H
#include "xugd.apr.h"
#include "apr_file_io.h"

namespace xugd{ namespace apr{
	typedef enum
	{
		FOpenRead = 0x00001,  // Open for reading
		FOpenWrite = 0x00002,  // Open for writing
		FOpenCreate = 0x00004,  // Create if not exist
		FOpenAppend = 0x00008,  // Append to the end of
		FOpenTruncate = 0x00010,  // Open and truncate to 0 length
		FOpenEXCL = 0x00040,  // Open should fail if FOpenCreate and file exists.
		FOpenBuffered = 0x00080,  // Open for buffered I/O
		FOpenDelOnClose = 0x00100,  // Delete after close

		FCreateNew = FOpenCreate|FOpenEXCL,
		FCreateAlways = FOpenCreate|FOpenTruncate,
	}FOpenMode;

	typedef enum {
		FSeekSet = SEEK_SET,
		FSeekCur = SEEK_CUR,
		FSeekEnd = SEEK_END,
	}FSeekMode;

//////////////////////////////////////////////////////////////////////////
	class XAPRCLASS_DECLARE XFileInfo
	{
	public:
		XFileInfo();
		~XFileInfo();

		void Open(const char* szFile_, FOpenMode euType_);
		void Close();

		const char* GetFileName() const
		{
			return m_szName;
		}

		apr_size_t Write(const void *pBuffer_, apr_size_t nBytes_);
		apr_size_t Read(void *pBuffer_, apr_size_t nBytes_);

		//////////////////////////////////////////////////////////////////////////
		// 读写一行数据（最大不能超过1024*8）
		char* Gets();	// 读取的数据存放在apr_pool中，不要去释放
		void Puts(const char *pLine_);

		void Flush();

		apr_off_t Seek(FSeekMode euMode_, apr_off_t nOffset_);
		apr_off_t Tell();

		void Trunc(apr_off_t nOffset_);

		//////////////////////////////////////////////////////////////////////////
		// Same as printf, The extensions are:
		//  - %%pA takes a struct in_addr *, and prints it as a.b.c.d
		//  - %%pI takes an apr_sockaddr_t * and prints it as a.b.c.d:port or [ipv6-address]:port
		//  - %%pT takes an apr_os_thread_t * and prints it in decimal,('0' is printed if !APR_HAS_THREADS)
		//  - %%pt takes an apr_os_thread_t * and prints it in hexadecimal,('0' is printed if !APR_HAS_THREADS)
		//  - %%pm takes an apr_status_t * and prints the appropriate error,string (from apr_strerror) corresponding to that error code.
		//  - %%pp takes a void * and outputs it in hex
		//  - %%pB takes a apr_uint32_t * as bytes and outputs it's apr_strfsize
		//  - %%pF same as above, but takes a apr_off_t *
		//  - %%pS same as above, but takes a apr_size_t *
		int Printf(const char *format, ...);

	private:
		apr_pool_t *m_filePool;
		apr_file_t *m_fHandle;
		char m_szName[MAX_PATH];
	};

//////////////////////////////////////////////////////////////////////////
} // apr
} // xugd
#endif	// XUGD_APR_FILE_H