#ifdef WINDOWS
#include "FrameHeader.h"

void FileContentTest::assertFileContent(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("FileContentTest failed: ") + message);
	}
}

void FileContentTest::testCreateBuffer()
{
	FileContent content;
	content.createBuffer(16);
	assertFileContent(content.mBuffer != nullptr, "buffer created");
	assertFileContent(content.mFileSize == 16, "file size");
	content.mBuffer[0] = 'A';
	content.mBuffer[1] = '\0';
	assertFileContent(content.mBuffer[0] == 'A', "buffer write");
}

void FileContentTest::testBufferLifecycle()
{
	FileContent* content = new FileContent();
	content->createBuffer(8);
	assertFileContent(content->mBuffer != nullptr, "heap buffer created");
	content->mBuffer[0] = 'x';
	content->mBuffer[1] = 'y';
	content->mBuffer[2] = '\0';
	assertFileContent(content->mBuffer[0] == 'x', "heap buffer x");
	assertFileContent(content->mBuffer[1] == 'y', "heap buffer y");
	delete content;
	assertFileContent(true, "destroyed without crash");
}

void FileContentTest::testBinaryFileLoad()
{
	const string tempFile = "FileContentTest_tmp.bin";
	const char payload[] = { 'b', 'i', 'n', '0', '1' };
	assertFileContent(writeFile(tempFile, payload, 5, false), "write temp file");

	FileContent content;
	assertFileContent(openBinaryFile(tempFile, &content), "open binary file");
	assertFileContent(content.mBuffer != nullptr, "loaded buffer");
	assertFileContent(content.mFileSize == 5, "loaded size");
	assertFileContent(content.mBuffer[0] == 'b', "loaded first");
	assertFileContent(content.mBuffer[4] == '1', "loaded last");

	deleteFile(tempFile);
}

void FileContentTest::test()
{
	testCreateBuffer();
	testBufferLifecycle();
	testBinaryFileLoad();
}
#endif
