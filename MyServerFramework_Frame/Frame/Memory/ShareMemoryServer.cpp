#include "FrameHeader.h"

#ifdef WINDOWS
bool ShareMemoryServer::isWinNTLater()
{
	OSVERSIONINFOA Ver;
	bool bAbleVersion = false;
	Ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	if (GetVersionExA(&Ver))
	{
		// 是否NT4.0以上操作系统
		if (Ver.dwPlatformId == VER_PLATFORM_WIN32_NT && Ver.dwMajorVersion >= 4)
		{
			bAbleVersion = true;
		}
	}
	else
	{
		mLastError = GetLastError();
	}
	return bAbleVersion;
}

void ShareMemoryServer::destory()
{
	if (mFileMapBuffer != nullptr)
	{
		UnmapViewOfFile(mFileMapBuffer);
		mFileMapBuffer = nullptr;
	}
	if (mFileMap != nullptr)
	{
		CloseHandle(mFileMap);
		mFileMap = nullptr;
	}
	if (mFile != nullptr && mFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(mFile);
		mFile = nullptr;
	}
}

bool ShareMemoryServer::create(const char* mapName, const int size)
{
	if (mCreateFlag)
	{
		destory();
	}

	mFileName = nullptr;
	mMapName = mapName;
	mMappingSize = size;
	if (mFileName)
	{
		mFile = CreateFileA(mFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	}
	else
	{
		// 在页面文件中创建共享內存
		mFile = INVALID_HANDLE_VALUE;
	}
	if (isWinNTLater())
	{
		// Set DACL
		constexpr int NUM_ACES = 2;   // number if ACEs int DACL
		// evryone -- read
		// creator -- full access
		PSID pEveryoneSID = nullptr; // everyone群組SID
		PSID pCreatorSID = nullptr; // creator群組SID
		PACL pFileMapACL = nullptr; // 準備新內存文件的DACL
		EXPLICIT_ACCESS ea[NUM_ACES]; // 外部訪問結構 
		bool hasError = false;
		FOR_ONCE
		{
			// 以下創建SID
			SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
			SID_IDENTIFIER_AUTHORITY SIDAuthCreator = SECURITY_CREATOR_SID_AUTHORITY;
			// Evryone
			if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID))
			{
				hasError = true;
				break;
			}
			// Creator
			if (!AllocateAndInitializeSid(&SIDAuthCreator, 1, SECURITY_CREATOR_OWNER_RID, 0, 0, 0, 0, 0, 0, 0, &pCreatorSID))
			{
				hasError = true;
				break;
			}
			// 填充ACE
			ZeroMemory(&ea, NUM_ACES * sizeof(EXPLICIT_ACCESS));
			// S-1-1-0 evryone, 唯讀權限
			EXPLICIT_ACCESS& access0 = ea[0];
			access0.grfAccessPermissions = GENERIC_READ | GENERIC_WRITE;
			access0.grfAccessMode = SET_ACCESS;
			access0.grfInheritance = NO_INHERITANCE;
			access0.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			access0.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			access0.Trustee.ptstrName = (LPTSTR)pEveryoneSID;
			// S-1-3-0 creator owner, 完全權限
			EXPLICIT_ACCESS& access1 = ea[1];
			access1.grfAccessPermissions = STANDARD_RIGHTS_ALL;
			access1.grfAccessMode = SET_ACCESS;
			access1.grfInheritance = NO_INHERITANCE;
			access1.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			access1.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			access1.Trustee.ptstrName = (LPTSTR)pCreatorSID;
			// 創建並填充ACL
			if (mSetEntriesInAcl == nullptr)
			{
				if (HINSTANCE hLib = LoadLibraryA("Advapi32.dll"))
				{
					mSetEntriesInAcl = (PSetEntriesInAcl)GetProcAddress(hLib, "SetEntriesInAclA");
					FreeLibrary(hLib);
				}
			}
			if (mSetEntriesInAcl == nullptr || mSetEntriesInAcl(NUM_ACES, ea, nullptr, &pFileMapACL) != ERROR_SUCCESS)
			{
				hasError = true;
				break;
			}
			// 创建並初始化內存文件的SD
			const PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (pSD == nullptr)
			{
				hasError = true;
				break;
			}
			if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
			{
				hasError = true;
				break;
			}
			// 添加ACL到SD中去
			if (!SetSecurityDescriptorDacl(pSD, TRUE, pFileMapACL, FALSE))   // not a default DACL 
			{
				hasError = true;
				break;
			}
			// 設置SA
			SECURITY_ATTRIBUTES saFileMap; // 內存文件的SA
			saFileMap.nLength = sizeof(SECURITY_ATTRIBUTES);
			saFileMap.bInheritHandle = FALSE;
			saFileMap.lpSecurityDescriptor = pSD;
			// 創建共享內存文件
			if (mFile == nullptr)
			{
				break;
			}
			mFileMap = CreateFileMappingA(mFile, &saFileMap, PAGE_READWRITE, 0, mMappingSize, mMapName);
			if (mFileMap != nullptr)
			{
				break;
			}
			mFileMap = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, mMapName);
			if (mFileMap == nullptr)
			{
				mLastError = GetLastError();
				destory();
				break;
			}
		}
		if (pFileMapACL != nullptr)
		{
			LocalFree(pFileMapACL);
			pFileMapACL = nullptr;
		}
		if (pEveryoneSID != nullptr)
		{
			FreeSid(pEveryoneSID);
			pEveryoneSID = nullptr;
		}
		if (pCreatorSID != nullptr)
		{
			FreeSid(pCreatorSID);
			pCreatorSID = nullptr;
		}
		if (hasError)
		{
			mLastError = GetLastError();
			return false;
		}
	}
	else
	{
		// 創建共享內存文件
		if (mFile)
		{
			mFileMap = CreateFileMappingA(mFile, nullptr, PAGE_READWRITE, 0, mMappingSize, mMapName);
			if (mFileMap == nullptr)
			{
				mLastError = GetLastError();
				destory();
				return false;
			}
		}
	}
	// 映射文件指針到用戶
	if (mFileMap)
	{
		mFileMapBuffer = MapViewOfFile(mFileMap, FILE_MAP_ALL_ACCESS, 0, 0, mMappingSize);
		if (mFileMapBuffer == nullptr)
		{
			mLastError = GetLastError();
			destory();
			return false;
		}
	}
	mCreateFlag = true;
	return true;
}

// 讀取命令數據
int ShareMemoryServer::readCmdData(DATA_HEADER& header, void* outBuffer, const int bufferSize)
{
	auto* pHeader = static_cast<DATA_HEADER*>(mFileMapBuffer);
	if (pHeader == nullptr)
	{
		mLastError = ERROR_NO_MAPFILE;
		SetLastError(mLastError);
		return -1;
	}
	// 如果当前状态不是客户端已写入,则表示没有数据可以读取
	if (pHeader->mFlag != MEMORY_STATE::CLIENT_HAS_WRITE)
	{
		return 0;
	}
	if (pHeader->mCmd == 0)
	{
		mLastError = ERROR_INVALID_CMDCODE;
		SetLastError(mLastError);
		return -1;
	}
	if (pHeader->mDataSize > header.mDataSize)
	{
		mLastError = ERROR_BUFFER_OVERFLOW;
		SetLastError(mLastError);
		return -1;
	}
	if (pHeader->mCmd != header.mCmd)
	{
		mLastError = ERROR_INVALID_CMDCODE;
		SetLastError(mLastError);
		return -1;
	}
	ZeroMemory(outBuffer, header.mDataSize);
	// 数据存入缓冲区
	MEMCPY(&header, sizeof(header), pHeader, sizeof(DATA_HEADER));
	MEMCPY(outBuffer, bufferSize, (char*)pHeader + sizeof(DATA_HEADER), pHeader->mDataSize);
	// 设置为服务器已读取
	pHeader->mFlag = MEMORY_STATE::SERVER_HAS_READ;
	return pHeader->mDataSize;
}

bool ShareMemoryServer::writeCmdData(const DATA_HEADER& header, const void* buffer)
{
	// 检验数据的合理性
	if (mFileMapBuffer == nullptr)
	{
		mLastError = ERROR_NO_MAPFILE;
		SetLastError(mLastError);
		return false;
	}
	if (header.mCmd == 0)
	{
		mLastError = ERROR_INVALID_CMDCODE;
		SetLastError(mLastError);
		return false;
	}
	if (header.mDataSize > 0 && buffer == nullptr)
	{
		mLastError = ERROR_INVALID_USER_BUFFER;
		SetLastError(mLastError);
		return false;
	}
	if (header.mDataSize + sizeof(DATA_HEADER) > mMappingSize)
	{
		mLastError = ERROR_BUFFER_OVERFLOW;
		SetLastError(mLastError);
		return false;
	}
	// 填充数据结构
	ZeroMemory(mFileMapBuffer, mMappingSize);
	MEMCPY(mFileMapBuffer, mMappingSize, &header, sizeof(DATA_HEADER));
	// 数据块
	auto* pData = static_cast<DATA_HEADER*>(mFileMapBuffer);
	MEMCPY((char*)pData + sizeof(DATA_HEADER), mMappingSize - sizeof(DATA_HEADER), buffer, header.mDataSize);
	// 设置为服务器已写入
	pData->mFlag = MEMORY_STATE::SERVER_HAS_WRITE;
	return true;
}

#endif
