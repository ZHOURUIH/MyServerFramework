#ifdef WINDOWS
#include "FrameHeader.h"

#define LFIT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("LogFileInfoTest FAILED: ") + (msg)); } } while(0)

void LogFileInfoTest::test()
{
	testDefaultState();
	testGenerateLogFileNameFormat();
	testGenerateErrorFileNameFormat();
	testDifferentGUIDsProduceDifferentPaths();
	testGUIDPathBucketing();
	testLogVsErrorFileNameDiffers();
}

// 1. 默认状态: 文件名为空,写入长度为0,GUID为0
void LogFileInfoTest::testDefaultState()
{
	LogFileInfo info;
	LFIT_ASSERT(info.mFileName.empty(), "default fileName should be empty");
	LFIT_ASSERT(info.mWriteLength == 0, "default writeLength should be 0");
	LFIT_ASSERT(info.mPlayerGUID == 0, "default playerGUID should be 0");
}

// 2. generateLogFileName 格式验证
void LogFileInfoTest::testGenerateLogFileNameFormat()
{
	LogFileInfo info;
	info.mPlayerGUID = 12345;
	info.generateLogFileName();

	// 应包含关键组成部分
	const string& name = info.mFileName;

	// 必须包含 "Log_" 前缀
	LFIT_ASSERT(name.find("Log_") != string::npos, "log fileName should contain 'Log_'");

	// 必须包含 ".txt" 后缀
	LFIT_ASSERT(name.size() >= 4 && name.substr(name.size() - 4) == ".txt", "log fileName should end with .txt");

	// 必须包含GUID (12345)
	LFIT_ASSERT(name.find("12345") != string::npos, "log fileName should contain playerGUID");

	// 必须包含分桶路径 (GUID/100 = 123)
	LFIT_ASSERT(name.find("123") != string::npos, "log fileName should contain GUID bucket");
}

// 3. generateErrorFileName 格式验证
void LogFileInfoTest::testGenerateErrorFileNameFormat()
{
	LogFileInfo info;
	info.mPlayerGUID = 67890;
	info.generateErrorFileName();

	const string& name = info.mFileName;

	// Error_ 前缀
	LFIT_ASSERT(name.find("Error_") != string::npos, "error fileName should contain 'Error_'");

	// .txt 后缀
	LFIT_ASSERT(name.size() >= 4 && name.substr(name.size() - 4) == ".txt", "error fileName should end with .txt");

	// 包含GUID
	LFIT_ASSERT(name.find("67890") != string::npos, "error fileName should contain playerGUID");

	// 包含分桶路径 (67890/100 = 678)
	LFIT_ASSERT(name.find("678") != string::npos, "error fileName should contain GUID bucket");
}

// 4. 不同GUID产生不同文件名
void LogFileInfoTest::testDifferentGUIDsProduceDifferentPaths()
{
	LogFileInfo infoA;
	LogFileInfo  infoB;
	infoA.mPlayerGUID = 1000;
	infoB.mPlayerGUID = 2000;

	infoA.generateLogFileName();
	infoB.generateLogFileName();

	// 即使在同一毫秒生成,GUID不同则完整路径应不同
	LFIT_ASSERT(infoA.mFileName != infoB.mFileName, "different GUIDs should produce different file names");
}

// 5. GUID分桶: 同一桶内GUID前缀相同,但子目录不同
void LogFileInfoTest::testGUIDPathBucketing()
{
	// 1001 和 1099 都在 bucket 10 里
	LogFileInfo info1;
	LogFileInfo info2;
	info1.mPlayerGUID = 1001;
	info2.mPlayerGUID = 1099;

	info1.generateLogFileName();
	info2.generateLogFileName();

	// 两者都应该包含 "10/" (1001/100=10, 1099/100=10)
	// 但完整路径应该不同（因为GUID不同）
	string bucketPrefix = LLToS((llong)10) + "/";
	LFIT_ASSERT(info1.mFileName.find(bucketPrefix) != string::npos, "GUID 1001 should be in bucket 10");
	LFIT_ASSERT(info2.mFileName.find(bucketPrefix) != string::npos, "GUID 1099 should be in bucket 10");
	LFIT_ASSERT(info1.mFileName != info2.mFileName, "same bucket but different GUIDs => different paths");

	// 跨桶测试: 1001 在桶10, 2001 在桶20
	LogFileInfo info3;
	info3.mPlayerGUID = 2001;
	info3.generateLogFileName();

	string bucket20 = LLToS((llong)20) + "/";
	LFIT_ASSERT(info3.mFileName.find(bucket20) != string::npos, "GUID 2001 should be in bucket 20");
}

// 6. Log 和 Error 的文件名必须不同
void LogFileInfoTest::testLogVsErrorFileNameDiffers()
{
	LogFileInfo logInfo;
	LogFileInfo errInfo;
	logInfo.mPlayerGUID = 5555;
	errInfo.mPlayerGUID = 5555;  // 相同GUID

	logInfo.generateLogFileName();
	errInfo.generateErrorFileName();

	// 一个包含 Log_, 另一个包含 Error_
	LFIT_ASSERT(logInfo.mFileName.find("Log_") != string::npos, "log name has 'Log_'");
	LFIT_ASSERT(errInfo.mFileName.find("Error_") != string::npos, "error name has 'Error_'");

	// 完整文件名必须不同
	LFIT_ASSERT(logInfo.mFileName != errInfo.mFileName, "log and error file names must differ");
}
#endif
