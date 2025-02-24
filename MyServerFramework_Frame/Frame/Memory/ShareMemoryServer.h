#pragma once

#include "FrameDefine.h"
#ifdef WINDOWS
#include <aclapi.h>

#include "Array.h"

#pragma warning(disable:4996)

static constexpr int ERROR_INVALID_CMDCODE = 0xE00001FF;	// 已经存在完全一样的共享內存
static constexpr int ERROR_NO_MAPFILE = 0xE00002FF;			// 未分配共享內存文件
static constexpr int ERROR_INVALID_CFMCODE = 0xE00003FF;	// 校验码不匹配

enum class MEMORY_STATE : byte
{
	NONE,
	CLIENT_HAS_WRITE,
	CLIENT_HAS_READ,
	SERVER_HAS_WRITE,
	SERVER_HAS_READ,
};

#pragma pack(1)
struct DATA_HEADER
{
	static constexpr int mUserDataLength = 64;
	int mCmd = 0;								// 命令码
	int mDataSize = 0;							// 数据区长度
	MEMORY_STATE mFlag = MEMORY_STATE::NONE;	// 数据区状态,0表示无状态,1表示客户端数据已写入,2表示客户端已读取数据,3表示服务器已写入,4表示服务器已读取
	MyString<mUserDataLength> mUserData;		// 64个字节的自定义数据
	DATA_HEADER()
	{
		mUserData.zero();
	}
};
typedef DWORD(WINAPI *PSetEntriesInAcl)(unsigned long, PEXPLICIT_ACCESS, PACL, PACL*);

#pragma pack()

// 共享內存服务端
class MICRO_LEGEND_FRAME_API ShareMemoryServer
{
public:
	virtual ~ShareMemoryServer() { destory(); }
	void destory();
	bool create(const char* mapName, const int size);
	void* getBuffer() const { return mFileMapBuffer; }
	int getMappingSize() const { return mMappingSize; }
	// 返回值为读取的数据长度,小于0表示读取错误,文件头需要填写命令码,数据长度
	int readCmdData(DATA_HEADER& header, void* outBuffer, const int bufferSize);
	// 需要填写文件头中的命令码,数据长度
	bool writeCmdData(const DATA_HEADER& header, const void* buffer);
protected:
	bool isWinNTLater();
protected:
	PSetEntriesInAcl mSetEntriesInAcl = nullptr;
	const char* mFileName = nullptr;			// 映射文件名
	const char* mMapName = nullptr;				// 內存文件名
	void* mFileMapBuffer = nullptr;				// 缓冲区指针
	HANDLE mFileMap = nullptr;					// 內存文件句柄
	HANDLE mFile = nullptr;						// 映射文件句柄
	int mMappingSize = 0;						// 缓冲区大小
	int mLastError = 0;							// 错误码
	bool mCreateFlag = false;					// 是否已创建共享內存
};
#endif