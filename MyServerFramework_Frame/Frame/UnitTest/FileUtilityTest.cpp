#include "FrameHeader.h"

// ============================================================
// FileUtilityTest — FileUtility 全方位测试
// 覆盖：
//   createFolder / isDirExist / isFileExist
//   writeFile / openFile / openTxtFile / openTxtFileLines
//   writeFileSimple / openBinaryFile
//   copyFile / moveFile / renameFile / deleteFile
//   findFiles / findFolders
//   copyFolder / deleteFolder
//   generateMD5 / generateFileMD5
//   getFileSize
//   validPath
// ============================================================

#define FUT_FILE_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("FileUtilityTest FAILED: ") + (msg)); } } while(0)

// 测试临时目录根（相对路径，确保权限可写）
static const string kTestRoot = "FileUtilityTest_Tmp";

// 辅助：清理测试目录（每轮测试开始前调用）
void FileUtilityTest::cleanupTestRoot()
{
    if (isDirExist(kTestRoot))
    {
        deleteFolder(kTestRoot);
    }
}

// ============================================================
// validPath
// ============================================================
void FileUtilityTest::testValidPath()
{
    // 无结尾斜杠 → 加上 /
    string p1 = "some/path";
    validPath(p1);
    FUT_FILE_ASSERT(!p1.empty() && p1.back() == '/', "validPath appended /");

    // 已有 / → 不变
    string p2 = "another/path/";
    const string before = p2;
    validPath(p2);
    FUT_FILE_ASSERT(p2 == before, "validPath unchanged when / already present");

    // 空字符串 → 不崩溃
    string p3 = "";
    validPath(p3);
    // 结果合法（不崩溃即可）
    FUT_FILE_ASSERT(true, "validPath empty no crash");

}

// ============================================================
// createFolder / isDirExist / deleteFolder
// ============================================================
void FileUtilityTest::testCreateAndDeleteFolder()
{
    cleanupTestRoot();

    // 创建根目录
    bool r1 = createFolder(kTestRoot);
    FUT_FILE_ASSERT(r1, "createFolder root success");
    FUT_FILE_ASSERT(isDirExist(kTestRoot), "isDirExist root true");

    // 不存在的目录
    FUT_FILE_ASSERT(!isDirExist(kTestRoot + "/nonexistent_dir_xyz"), "isDirExist nonexist false");

    // 创建子目录
    const string sub = kTestRoot + "/subdir";
    FUT_FILE_ASSERT(createFolder(sub), "createFolder subdir success");
    FUT_FILE_ASSERT(isDirExist(sub), "isDirExist subdir true");

    // 再次创建同名目录（不崩溃，通常返回 false 或 true，行为由实现决定）
    createFolder(sub); // 不崩溃即可

    // 创建深层目录
    const string deep = kTestRoot + "/a/b/c";
    createFolder(kTestRoot + "/a");
    createFolder(kTestRoot + "/a/b");
    FUT_FILE_ASSERT(createFolder(deep), "createFolder deep success");
    FUT_FILE_ASSERT(isDirExist(deep), "isDirExist deep true");

    // deleteFolder
    deleteFolder(kTestRoot + "/a");
    FUT_FILE_ASSERT(!isDirExist(kTestRoot + "/a/b"), "deleteFolder removed child");

}

// ============================================================
// writeFile / isFileExist / getFileSize / deleteFile
// ============================================================
void FileUtilityTest::testWriteAndDeleteFile()
{
    const string filePath = kTestRoot + "/test_write.txt";

    // 写文件
    const string content = "Hello, FileUtility!\nLine2\nLine3";
    bool rWrite = writeFile(filePath, content);
    FUT_FILE_ASSERT(rWrite, "writeFile success");
    FUT_FILE_ASSERT(isFileExist(filePath), "isFileExist true after write");

    // getFileSize
    const int sz = getFileSize(filePath);
    FUT_FILE_ASSERT(sz == (int)content.length(), "getFileSize matches content length");

    // 不存在的文件
    FUT_FILE_ASSERT(!isFileExist(kTestRoot + "/nonexistent_file_xyz.txt"), "isFileExist false nonexist");
    FUT_FILE_ASSERT(getFileSize(kTestRoot + "/nonexistent_file_xyz.txt") == 0, "getFileSize nonexist 0");

    // 追加写
    const string append1 = "\nAppended";
    writeFile(filePath, append1, true);
    const int sz2 = getFileSize(filePath);
    FUT_FILE_ASSERT(sz2 == (int)(content.length() + append1.length()), "writeFile append size correct");

    // deleteFile
    FUT_FILE_ASSERT(deleteFile(filePath), "deleteFile success");
    FUT_FILE_ASSERT(!isFileExist(filePath), "isFileExist false after delete");

    // deleteFile 不存在的文件
    FUT_FILE_ASSERT(!deleteFile(filePath), "deleteFile nonexist false");

}

// ============================================================
// writeFileSimple / openTxtFile
// ============================================================
void FileUtilityTest::testWriteSimpleAndOpen()
{
    const string filePath = kTestRoot + "/test_simple.txt";
    const string content = "Simple write test content 123";

    bool rWrite = writeFileSimple(filePath, content);
    FUT_FILE_ASSERT(rWrite, "writeFileSimple success");
    FUT_FILE_ASSERT(isFileExist(filePath), "isFileExist after writeFileSimple");

    // openTxtFile (返回 string 版本)
    const string readBack = openTxtFile(filePath, false);
    FUT_FILE_ASSERT(readBack == content, "openTxtFile content matches");

    // openTxtFile (output string 版本)
    string readBack2;
    bool rOpen2 = openTxtFile(filePath, readBack2, false);
    FUT_FILE_ASSERT(rOpen2, "openTxtFile output-string true");
    FUT_FILE_ASSERT(readBack2 == content, "openTxtFile output-string content matches");

    // 不存在的文件
    const string notExist = kTestRoot + "/no_such_file_abc.txt";
    const string r3 = openTxtFile(notExist, false, false);
    FUT_FILE_ASSERT(r3.empty(), "openTxtFile notExist returns empty");

    string r4;
    bool rOpen4 = openTxtFile(notExist, r4, false, false);
    FUT_FILE_ASSERT(!rOpen4, "openTxtFile notExist output-string false");

    // 追加写
    writeFileSimple(filePath, string("\nLine2"), true);
    const string r5 = openTxtFile(filePath, false);
    FUT_FILE_ASSERT(r5 == content + "\nLine2", "writeFileSimple append correct");

    deleteFile(filePath);
}

// ============================================================
// openFile (binary) / openBinaryFile
// ============================================================
void FileUtilityTest::testOpenBinaryFile()
{
    const string filePath = kTestRoot + "/test_binary.bin";

    // 写入二进制数据
    const char binData[] = { 0x01, 0x02, 0x03, 0x04, 0x05, (char)0xAB, (char)0xCD, (char)0xEF };
    const int binLen = (int)sizeof(binData);
    bool rWrite = writeFile(filePath, binData, binLen);
    FUT_FILE_ASSERT(rWrite, "writeFile binary success");
    FUT_FILE_ASSERT(getFileSize(filePath) == binLen, "binary file size correct");

    // openBinaryFile
    FileContent fc;
    bool rOpen = openBinaryFile(filePath, &fc);
    FUT_FILE_ASSERT(rOpen, "openBinaryFile success");
    FUT_FILE_ASSERT(fc.mFileSize == binLen, "openBinaryFile size matches");
    FUT_FILE_ASSERT(memcmp(fc.mBuffer, binData, binLen) == 0, "openBinaryFile content matches");

    // openFile with addZero
    FileContent fc2;
    bool rOpen2 = openFile(filePath, &fc2, true);
    FUT_FILE_ASSERT(rOpen2, "openFile addZero success");
    FUT_FILE_ASSERT(fc2.mFileSize == binLen + 1, "openFile addZero size matches original");

    // 不存在
    FileContent fc3;
    bool rOpen3 = openBinaryFile(kTestRoot + "/no_such_bin.bin", &fc3, false);
    FUT_FILE_ASSERT(!rOpen3, "openBinaryFile notExist false");

    deleteFile(filePath);
}

// ============================================================
// openTxtFileLines
// ============================================================
void FileUtilityTest::testOpenTxtFileLines()
{
    const string filePath = kTestRoot + "/test_lines.txt";
    const string content = "Line1\nLine2\nLine3\nLine4\nLine5";
    writeFile(filePath, content);

    Vector<string> lines;
    bool rOpen = openTxtFileLines(filePath, lines, false);
    FUT_FILE_ASSERT(rOpen, "openTxtFileLines success");
    FUT_FILE_ASSERT(lines.size() == 5, "openTxtFileLines 5 lines");
    FUT_FILE_ASSERT(lines[0] == "Line1", "openTxtFileLines line0==Line1");
    FUT_FILE_ASSERT(lines[4] == "Line5", "openTxtFileLines line4==Line5");

    // 空文件
    const string emptyFile = kTestRoot + "/empty.txt";
    writeFile(emptyFile, "");
    Vector<string> emptyLines;
    openTxtFileLines(emptyFile, emptyLines, false);
    FUT_FILE_ASSERT(emptyLines.size() == 0 || emptyLines.size() == 1, "openTxtFileLines empty file");

    // 不存在的文件
    Vector<string> lines2;
    bool rOpen2 = openTxtFileLines(kTestRoot + "/no_such_lines.txt", lines2, false, false);
    FUT_FILE_ASSERT(!rOpen2, "openTxtFileLines notExist false");

    // 单行文件
    const string singleFile = kTestRoot + "/single.txt";
    writeFile(singleFile, "OnlyOneLine");
    Vector<string> singleLines;
    openTxtFileLines(singleFile, singleLines, false);
    FUT_FILE_ASSERT(singleLines.size() == 1, "openTxtFileLines single line count==1");
    FUT_FILE_ASSERT(singleLines[0] == "OnlyOneLine", "openTxtFileLines single line content");

    deleteFile(filePath);
    deleteFile(emptyFile);
    deleteFile(singleFile);
}

// ============================================================
// copyFile
// ============================================================
void FileUtilityTest::testCopyFile()
{
    const string src = kTestRoot + "/copy_src.txt";
    const string dst = kTestRoot + "/copy_dst.txt";
    const string content = "Copy file test content 9876";
    writeFile(src, content);

    // 正常拷贝
    bool rCopy = copyFile(src, dst);
    FUT_FILE_ASSERT(rCopy, "copyFile success");
    FUT_FILE_ASSERT(isFileExist(dst), "copyFile dst exists");
    const string readDst = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst == content, "copyFile content matches");

    // 修改 src 后 dst 不变（是独立副本）
    writeFile(src, "Modified");
    const string readDst2 = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst2 == content, "copyFile dst independent from src");

    // overWrite = true：覆盖已存在目标
    const string src2 = kTestRoot + "/copy_src2.txt";
    writeFile(src2, "New content");
    bool rCopy2 = copyFile(src2, dst, true);
    FUT_FILE_ASSERT(rCopy2, "copyFile overWrite true success");
    const string readDst3 = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst3 == "New content", "copyFile overWrite updated content");

    // overWrite = false：目标存在时不覆盖，框架语义返回 true（视为成功，未做覆盖）
    const string src3 = kTestRoot + "/copy_src3.txt";
    writeFile(src3, "Should not overwrite");
    bool rCopy3 = copyFile(src3, dst, false);
    FUT_FILE_ASSERT(rCopy3, "copyFile overWrite false returns true (no-op success)");
    const string readDst4 = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst4 == "New content", "copyFile overWrite false dst unchanged");

    // 拷贝不存在的文件
    bool rCopy4 = copyFile(kTestRoot + "/no_src.txt", kTestRoot + "/no_dst.txt");
    FUT_FILE_ASSERT(!rCopy4, "copyFile notExist src returns false");

    deleteFile(src);
    deleteFile(src2);
    deleteFile(src3);
    deleteFile(dst);
}

// ============================================================
// moveFile / renameFile
// ============================================================
void FileUtilityTest::testMoveAndRenameFile()
{
    const string src = kTestRoot + "/move_src.txt";
    const string dst = kTestRoot + "/move_dst.txt";
    const string content = "Move file test content";
    writeFile(src, content);

    // moveFile
    bool rMove = moveFile(src, dst);
    FUT_FILE_ASSERT(rMove, "moveFile success");
    FUT_FILE_ASSERT(!isFileExist(src), "moveFile src gone");
    FUT_FILE_ASSERT(isFileExist(dst), "moveFile dst exists");
    const string readDst = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst == content, "moveFile content correct");

    // moveFile - 目标已存在，forceCover=true
    writeFile(src, "New move content");
    bool rMove2 = moveFile(src, dst, true);
    FUT_FILE_ASSERT(rMove2, "moveFile forceCover true success");
    const string readDst2 = openTxtFile(dst, false);
    FUT_FILE_ASSERT(readDst2 == "New move content", "moveFile forceCover content updated");

    // renameFile
    const string renamed = kTestRoot + "/renamed.txt";
    bool rRename = renameFile(dst, renamed);
    FUT_FILE_ASSERT(rRename, "renameFile success");
    FUT_FILE_ASSERT(isFileExist(renamed), "renameFile exists");
    FUT_FILE_ASSERT(!isFileExist(dst), "renameFile src gone");

    deleteFile(renamed);
}

// ============================================================
// copyFolder / renameFolder
// ============================================================
void FileUtilityTest::testCopyAndRenameFolder()
{
    const string srcDir = kTestRoot + "/folder_src";
    const string dstDir = kTestRoot + "/folder_dst";
    createFolder(srcDir);

    // 在源目录中创建一些文件
    writeFile(srcDir + "/file1.txt", "file1 content");
    writeFile(srcDir + "/file2.txt", "file2 content");
    createFolder(srcDir + "/sub");
    writeFile(srcDir + "/sub/file3.txt", "sub file3 content");

    // copyFolder
    bool rCopy = copyFolder(srcDir + "/", dstDir + "/");
    FUT_FILE_ASSERT(rCopy, "copyFolder success");
    FUT_FILE_ASSERT(isDirExist(dstDir), "copyFolder dstDir exists");
    FUT_FILE_ASSERT(isFileExist(dstDir + "/file1.txt"), "copyFolder file1 copied");
    FUT_FILE_ASSERT(isFileExist(dstDir + "/file2.txt"), "copyFolder file2 copied");
    FUT_FILE_ASSERT(isFileExist(dstDir + "/sub/file3.txt"), "copyFolder sub/file3 copied");

    // 验证内容一致
    const string r1 = openTxtFile(dstDir + "/file1.txt", false);
    FUT_FILE_ASSERT(r1 == "file1 content", "copyFolder file1 content correct");
    const string r3 = openTxtFile(dstDir + "/sub/file3.txt", false);
    FUT_FILE_ASSERT(r3 == "sub file3 content", "copyFolder sub/file3 content correct");

    // 修改 src 不影响 dst
    writeFile(srcDir + "/file1.txt", "modified");
    const string r2 = openTxtFile(dstDir + "/file1.txt", false);
    FUT_FILE_ASSERT(r2 == "file1 content", "copyFolder dst independent");

    // renameFolder
    const string renamedDir = kTestRoot + "/folder_renamed";
    bool rRename = renameFolder(dstDir, renamedDir);
    FUT_FILE_ASSERT(rRename, "renameFolder success");
    FUT_FILE_ASSERT(isDirExist(renamedDir), "renameFolder new exists");
    FUT_FILE_ASSERT(!isDirExist(dstDir), "renameFolder old gone");
    FUT_FILE_ASSERT(isFileExist(renamedDir + "/file1.txt"), "renameFolder file1 accessible");

    deleteFolder(srcDir);
    deleteFolder(renamedDir);
}

// ============================================================
// findFiles
// ============================================================
void FileUtilityTest::testFindFiles()
{
    const string dir = kTestRoot + "/findfiles_test";
    createFolder(dir);
    createFolder(dir + "/sub1");
    createFolder(dir + "/sub2");

    // 创建文件
    writeFile(dir + "/a.txt", "a");
    writeFile(dir + "/b.txt", "b");
    writeFile(dir + "/c.log", "c");
    writeFile(dir + "/sub1/d.txt", "d");
    writeFile(dir + "/sub1/e.log", "e");
    writeFile(dir + "/sub2/f.txt", "f");
    writeFile(dir + "/sub2/g.dat", "g");

    // 递归查找所有文件
    {
        Vector<string> files;
        findFiles(dir, files);
        FUT_FILE_ASSERT(files.size() == 7, "findFiles recursive all 7");
    }

    // 查找 .txt 文件（递归）
    {
        Vector<string> files;
        const string pattern = ".txt";
        findFiles(dir, files, pattern, true);
        FUT_FILE_ASSERT(files.size() == 4, "findFiles recursive .txt 4");
    }

    // 查找 .log 文件（递归）
    {
        Vector<string> files;
        const string pattern = ".log";
        findFiles(dir, files, pattern, true);
        FUT_FILE_ASSERT(files.size() == 2, "findFiles recursive .log 2");
    }

    // 查找 .txt 文件（非递归，仅顶层）
    {
        Vector<string> files;
        const string pattern = ".txt";
        findFiles(dir, files, pattern, false);
        FUT_FILE_ASSERT(files.size() == 2, "findFiles nonrecursive .txt 2");
    }

    // 空目录
    {
        const string emptyDir = kTestRoot + "/findfiles_empty";
        createFolder(emptyDir);
        Vector<string> files;
        findFiles(emptyDir, files);
        FUT_FILE_ASSERT(files.size() == 0, "findFiles empty dir 0");
        deleteFolder(emptyDir);
    }

    deleteFolder(dir);
}

// ============================================================
// findFolders
// ============================================================
void FileUtilityTest::testFindFolders()
{
    const string dir = kTestRoot + "/findfolders_test";
    createFolder(dir);
    createFolder(dir + "/subA");
    createFolder(dir + "/subB");
    createFolder(dir + "/subA/subAA");
    writeFile(dir + "/file.txt", "x");

    // 非递归：只找顶层子目录
    {
        Vector<string> folders;
        findFolders(dir, folders, false);
        FUT_FILE_ASSERT(folders.size() == 2, "findFolders nonrecursive 2");
    }

    // 递归：所有子目录
    {
        Vector<string> folders;
        findFolders(dir, folders, true);
        FUT_FILE_ASSERT(folders.size() == 3, "findFolders recursive 3");
    }

    // 空目录
    {
        const string emptyDir = kTestRoot + "/findfolders_empty";
        createFolder(emptyDir);
        Vector<string> folders;
        findFolders(emptyDir, folders, false);
        FUT_FILE_ASSERT(folders.size() == 0, "findFolders empty dir 0");
        deleteFolder(emptyDir);
    }

    deleteFolder(dir);
}

// ============================================================
// generateMD5 (内存数据)
// ============================================================
void FileUtilityTest::testGenerateMD5Memory()
{
    // 空数据
    const string md5Empty = generateMD5("", false);
    FUT_FILE_ASSERT(md5Empty.length() == 32, "generateMD5 empty length==32");
    FUT_FILE_ASSERT(md5Empty == "d41d8cd98f00b204e9800998ecf8427e", "generateMD5 empty correct lowercase");

    // 大写版本
    const string md5EmptyUpper = generateMD5("", true);
    FUT_FILE_ASSERT(md5EmptyUpper.length() == 32, "generateMD5 empty upper length==32");
    FUT_FILE_ASSERT(md5EmptyUpper == "D41D8CD98F00B204E9800998ECF8427E", "generateMD5 empty correct uppercase");

    // 已知字符串 "abc"
    const string md5Abc = generateMD5("abc", false);
    FUT_FILE_ASSERT(md5Abc == "900150983cd24fb0d6963f7d28e17f72", "generateMD5 abc correct");

    // 已知字符串 "Hello World"
    const string md5Hello = generateMD5("Hello World", false);
    FUT_FILE_ASSERT(md5Hello == "b10a8db164e0754105b7a99be72e3fe5", "generateMD5 Hello World correct");

    // 大小写差异：同一数据大小写 MD5 值相同（除了大小写区分）
    const string md5Lower = generateMD5("test123", false);
    const string md5Upper = generateMD5("test123", true);
    FUT_FILE_ASSERT(md5Lower.length() == 32, "generateMD5 test123 lower length==32");
    FUT_FILE_ASSERT(md5Upper.length() == 32, "generateMD5 test123 upper length==32");
    // 大写版等于小写版的 toupper
    string expectedUpper = md5Lower;
    for (char& c : expectedUpper)
    {
        c = (char)toupper((unsigned char)c);
    }
    FUT_FILE_ASSERT(md5Upper == expectedUpper, "generateMD5 upper matches toupper(lower)");

    // 不同内容产生不同 MD5
    const string md5A = generateMD5("contentA", false);
    const string md5B = generateMD5("contentB", false);
    FUT_FILE_ASSERT(md5A != md5B, "generateMD5 different content different hash");

    // buffer 版本
    const char* rawData = "raw buffer data";
    const int rawLen = (int)strlen(rawData);
    const string md5Raw = generateMD5(rawData, rawLen, false);
    const string md5StrVersion = generateMD5(string("raw buffer data"), false);
    FUT_FILE_ASSERT(md5Raw == md5StrVersion, "generateMD5 buffer and string versions match");

}

// ============================================================
// generateFileMD5
// ============================================================
void FileUtilityTest::testGenerateFileMD5()
{
    const string filePath = kTestRoot + "/md5_test.txt";
    const string content = "Test content for MD5";
    writeFile(filePath, content);

    // 文件 MD5 应与内存版本一致
    const string fileMD5 = generateFileMD5(filePath, false);
    const string memMD5 = generateMD5(content, false);
    FUT_FILE_ASSERT(fileMD5.length() == 32, "generateFileMD5 length==32");
    FUT_FILE_ASSERT(fileMD5 == memMD5, "generateFileMD5 matches generateMD5 for same content");

    // 大写版本
    const string fileMD5Upper = generateFileMD5(filePath, true);
    FUT_FILE_ASSERT(fileMD5Upper.length() == 32, "generateFileMD5 upper length==32");
    string expectedUpper = fileMD5;
    for (char& c : expectedUpper)
    {
        c = (char)toupper((unsigned char)c);
    }
    FUT_FILE_ASSERT(fileMD5Upper == expectedUpper, "generateFileMD5 upper matches toupper(lower)");

    // 修改文件内容后 MD5 变化
    writeFile(filePath, "Different content!");
    const string fileMD5New = generateFileMD5(filePath, false);
    FUT_FILE_ASSERT(fileMD5New != fileMD5, "generateFileMD5 different content different hash");

    // 空文件
    const string emptyFile = kTestRoot + "/empty_md5.txt";
    writeFile(emptyFile, "");
    const string emptyMD5 = generateFileMD5(emptyFile, false);
    FUT_FILE_ASSERT(emptyMD5 == "d41d8cd98f00b204e9800998ecf8427e", "generateFileMD5 empty file correct");

    // 不存在的文件（返回空字符串或 error，不崩溃）
    const string noFileMD5 = generateFileMD5(kTestRoot + "/no_such_file.txt", false);
    FUT_FILE_ASSERT(true, "generateFileMD5 nonexist no crash");

    deleteFile(filePath);
    deleteFile(emptyFile);
}

// ============================================================
// getFileSize（扩展测试）
// ============================================================
void FileUtilityTest::testGetFileSizeExtended()
{
    // 0 字节
    const string f0 = kTestRoot + "/size_0.bin";
    writeFile(f0, (char*)nullptr, 0);
    // 空文件或极小文件
    writeFile(f0, "");
    const int s0 = getFileSize(f0);
    FUT_FILE_ASSERT(s0 == 0, "getFileSize empty file == 0");

    // 1 字节
    const string f1 = kTestRoot + "/size_1.bin";
    const char c1 = 'X';
    writeFile(f1, &c1, 1);
    FUT_FILE_ASSERT(getFileSize(f1) == 1, "getFileSize 1 byte == 1");

    // 100 字节
    const string f100 = kTestRoot + "/size_100.bin";
    char buf100[100];
    memset(buf100, 'A', 100);
    writeFile(f100, buf100, 100);
    FUT_FILE_ASSERT(getFileSize(f100) == 100, "getFileSize 100 bytes == 100");

    // 1024 字节
    const string f1k = kTestRoot + "/size_1k.bin";
    char buf1k[1024];
    memset(buf1k, 'B', 1024);
    writeFile(f1k, buf1k, 1024);
    FUT_FILE_ASSERT(getFileSize(f1k) == 1024, "getFileSize 1KB == 1024");

    deleteFile(f0);
    deleteFile(f1);
    deleteFile(f100);
    deleteFile(f1k);
}

// ============================================================
// deleteEmptyFolder
// ============================================================
void FileUtilityTest::testDeleteEmptyFolder()
{
    // 空目录：应成功删除
    const string emptyDir = kTestRoot + "/delete_empty";
    createFolder(emptyDir);
    FUT_FILE_ASSERT(isDirExist(emptyDir), "deleteEmptyFolder dir created");
    bool r = deleteEmptyFolder(emptyDir);
    FUT_FILE_ASSERT(r, "deleteEmptyFolder empty dir success");
    FUT_FILE_ASSERT(!isDirExist(emptyDir), "deleteEmptyFolder dir removed");

    // 非空目录：应返回 false（或不删除）
    const string nonEmptyDir = kTestRoot + "/delete_nonempty";
    createFolder(nonEmptyDir);
    writeFile(nonEmptyDir + "/file.txt", "x");
    bool r2 = deleteEmptyFolder(nonEmptyDir);
    FUT_FILE_ASSERT(!r2, "deleteEmptyFolder nonempty returns false");
    FUT_FILE_ASSERT(isDirExist(nonEmptyDir), "deleteEmptyFolder nonempty dir still exists");
    deleteFolder(nonEmptyDir);

}

// ============================================================
// 综合场景：写入→读取→拷贝→比较→删除
// ============================================================
void FileUtilityTest::testComprehensiveScenario()
{
    cleanupTestRoot();

    const string workDir = kTestRoot + "/scenario";
    createFolder(workDir);

    // 创建多个文件
    for (int i = 0; i < 10; ++i)
    {
        const string name = workDir + "/data_" + IToS(i) + ".txt";
        const string content = "Content for file " + IToS(i) + " with some padding text 12345";
        writeFile(name, content);
    }

    // 查找所有文件
    Vector<string> allFiles;
    findFiles(workDir, allFiles);
    FUT_FILE_ASSERT(allFiles.size() == 10, "scenario: 10 files created");

    // 计算每个文件 MD5 并确认不同文件 MD5 不同（内容不同）
    Vector<string> md5List;
    for (int i = 0; i < (int)allFiles.size(); ++i)
    {
        md5List.add(generateFileMD5(allFiles[i], false));
    }
    // 确认 MD5 列表中没有重复（每个文件内容唯一）
    bool allUnique = true;
    for (int i = 0; i < (int)md5List.size(); ++i)
    {
        for (int j = i + 1; j < (int)md5List.size(); ++j)
        {
            if (md5List[i] == md5List[j])
            {
                allUnique = false;
            }
        }
    }
    FUT_FILE_ASSERT(allUnique, "scenario: all MD5s unique");

    // 创建备份目录并拷贝
    const string backupDir = kTestRoot + "/scenario_backup";
    createFolder(backupDir);
    for (int i = 0; i < (int)allFiles.size(); ++i)
    {
        const string fileName = getFileName(allFiles[i]);
        copyFile(allFiles[i], backupDir + "/" + fileName);
    }

    // 校验备份 MD5 与原始一致（用文件真实名称，不依赖索引）
    for (int i = 0; i < (int)allFiles.size(); ++i)
    {
        const string fileName = getFileName(allFiles[i]);
        const string md5Src = generateFileMD5(allFiles[i], false);
        const string md5Dst = generateFileMD5(backupDir + "/" + fileName, false);
        FUT_FILE_ASSERT(md5Src == md5Dst, "scenario: backup MD5 matches original");
    }

    // 删除原始目录
    deleteFolder(workDir);
    FUT_FILE_ASSERT(!isDirExist(workDir), "scenario: original dir deleted");

    // 备份目录仍然存在
    FUT_FILE_ASSERT(isDirExist(backupDir), "scenario: backup dir still exists");

    // 从备份恢复
    createFolder(workDir);
    copyFolder(backupDir + "/", workDir + "/");
    Vector<string> restoredFiles;
    findFiles(workDir, restoredFiles);
    FUT_FILE_ASSERT(restoredFiles.size() == 10, "scenario: restored 10 files");

    deleteFolder(workDir);
    deleteFolder(backupDir);
}

// ============================================================
// 大量小文件 stress test
// ============================================================
void FileUtilityTest::testManySmallFiles()
{
    const string dir = kTestRoot + "/stress";
    createFolder(dir);

    const int count = 50;

    // 创建 50 个小文件
    FOR(count)
    {
        const string name = dir + "/file_" + IToS(i) + ".dat";
        const string data = "data_" + IToS(i * 100);
        writeFile(name, data);
    }

    // 验证全部存在
    FOR(count)
    {
        const string name = dir + "/file_" + IToS(i) + ".dat";
        FUT_FILE_ASSERT(isFileExist(name), "stress: file exists");
    }

    // 验证内容
    FOR(count)
    {
        const string name = dir + "/file_" + IToS(i) + ".dat";
        const string expected = "data_" + IToS(i * 100);
        const string readBack = openTxtFile(name, false);
        FUT_FILE_ASSERT(readBack == expected, "stress: file content correct");
    }

    // 查找所有文件
    Vector<string> files;
    findFiles(dir, files);
    FUT_FILE_ASSERT(files.size() == (int)count, "stress: findFiles count correct");

    // 逐一删除
    FOR(count)
    {
        deleteFile(dir + "/file_" + IToS(i) + ".dat");
    }

    Vector<string> filesAfter;
    findFiles(dir, filesAfter);
    FUT_FILE_ASSERT(filesAfter.isEmpty(), "stress: all files deleted");

    deleteFolder(dir);
}

// ============================================================
// 多轮写入追加测试
// ============================================================
void FileUtilityTest::testMultipleAppends()
{
    const string filePath = kTestRoot + "/append_test.txt";
    deleteFile(filePath); // 确保不存在

    // 10 次追加写入
    FOR(10)
    {
        writeFile(filePath, "Line " + IToS(i) + "\n", true);
    }

    Vector<string> lines;
    openTxtFileLines(filePath, lines, false);
    FUT_FILE_ASSERT(lines.size() == 10, "multipleAppends: 10 lines");
    FUT_FILE_ASSERT(lines[0] == "Line 0", "multipleAppends: line0 correct");
    FUT_FILE_ASSERT(lines[9] == "Line 9", "multipleAppends: line9 correct");

    // 覆盖写
    writeFile(filePath, string("Overwritten"), false);
    const string content = openTxtFile(filePath, false);
    FUT_FILE_ASSERT(content == "Overwritten", "multipleAppends: overwrite correct");

    deleteFile(filePath);
}

// ============================================================
// 文件名包含空格和特殊字符
// ============================================================
void FileUtilityTest::testFileWithSpecialName()
{
    // 含空格
    const string spaceFile = kTestRoot + "/file with spaces.txt";
    const string spaceContent = "content with spaces in filename";
    bool rWrite = writeFile(spaceFile, spaceContent);
    FUT_FILE_ASSERT(rWrite, "writeFile with spaces success");
    FUT_FILE_ASSERT(isFileExist(spaceFile), "isFileExist with spaces true");
    const string readSpace = openTxtFile(spaceFile, false);
    FUT_FILE_ASSERT(readSpace == spaceContent, "openTxtFile with spaces content correct");
    deleteFile(spaceFile);

    // 含下划线和数字
    const string numFile = kTestRoot + "/file_123_abc.txt";
    writeFile(numFile, "numeric file");
    FUT_FILE_ASSERT(isFileExist(numFile), "isFileExist numeric name true");
    deleteFile(numFile);

}

// ============================================================
// 二进制数据完整性验证
// ============================================================
void FileUtilityTest::testBinaryDataIntegrity()
{
    const string filePath = kTestRoot + "/binary_integrity.bin";

    // 写入所有可能的字节值 0~255
    char allBytes[256];
    FOR(256)
    {
        allBytes[i] = (char)i;
    }
    writeFile(filePath, allBytes, 256);

    // 读取并验证
    FileContent fc;
    openBinaryFile(filePath, &fc);
    FUT_FILE_ASSERT(fc.mFileSize == 256, "binaryIntegrity: size == 256");
    bool allMatch = true;
    FOR(256)
    {
        if (fc.mBuffer[i] != allBytes[i])
        {
            allMatch = false;
        }
    }
    FUT_FILE_ASSERT(allMatch, "binaryIntegrity: all 256 bytes match");

    deleteFile(filePath);
}

// ============================================================
// 主入口
// ============================================================
void FileUtilityTest::test()
{
    // 初始化测试目录
    cleanupTestRoot();
    createFolder(kTestRoot);

    testValidPath();
    testCreateAndDeleteFolder();
    testWriteAndDeleteFile();
    testWriteSimpleAndOpen();
    testOpenBinaryFile();
    testOpenTxtFileLines();
    testCopyFile();
    testMoveAndRenameFile();
    testCopyAndRenameFolder();
    testFindFiles();
    testFindFolders();
    testGenerateMD5Memory();
    testGenerateFileMD5();
    testGetFileSizeExtended();
    testDeleteEmptyFolder();
    testComprehensiveScenario();
    testManySmallFiles();
    testMultipleAppends();
    testFileWithSpecialName();
    testBinaryDataIntegrity();

    // 清理测试目录
    cleanupTestRoot();
}
