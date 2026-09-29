#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API FileUtilityTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void cleanupTestRoot();
	static void testValidPath();
	static void testCreateAndDeleteFolder();
	static void testWriteAndDeleteFile();
	static void testWriteSimpleAndOpen();
	static void testOpenBinaryFile();
	static void testOpenTxtFileLines();
	static void testCopyFile();
	static void testMoveAndRenameFile();
	static void testCopyAndRenameFolder();
	static void testFindFiles();
	static void testFindFolders();
	static void testGenerateMD5Memory();
	static void testGenerateFileMD5();
	static void testGetFileSizeExtended();
	static void testDeleteEmptyFolder();
	static void testComprehensiveScenario();
	static void testManySmallFiles();
	static void testMultipleAppends();
	static void testFileWithSpecialName();
	static void testBinaryDataIntegrity();
	static void testDeep();
	static void assertFileUtility(bool condition, const char* message);
	static string tempRoot();
	static void prepareTree(const string& root);
	static void testPathAndExistence();
	static void testFindAndCopy();
	static void testReadWriteAndMD5();
	static void testDeleteHelpers();
#endif
};