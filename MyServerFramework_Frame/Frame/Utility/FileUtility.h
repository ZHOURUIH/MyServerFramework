#pragma once

#include "MathUtility.h"

namespace FileUtility
{
	void MICRO_LEGEND_FRAME_API validPath(string& path);
	void MICRO_LEGEND_FRAME_API findFiles(const string& path, Vector<string>& files, const string* patterns, int patternCount, bool recursive = true);
	void findFiles(const string& path, Vector<string>& files) { findFiles(path, files, nullptr, 0, true); }
	void findFiles(const string& path, Vector<string>& files, const string& patterns, bool recursive = true)
	{
		findFiles(path, files, &patterns, 1, recursive);
	}
#ifdef LINUX
	bool MICRO_LEGEND_FRAME_API isDirectory(const string& path);
#endif
	void MICRO_LEGEND_FRAME_API findFolders(const string& path, Vector<string>& folders, bool recursive = false);
	void MICRO_LEGEND_FRAME_API deleteFolder(const string& path);
	bool MICRO_LEGEND_FRAME_API deleteEmptyFolder(const string& path);
	bool deleteFile(const string& path) { return remove(path.c_str()) == 0; }
	bool MICRO_LEGEND_FRAME_API isFileExist(const string& fullPath);
	bool MICRO_LEGEND_FRAME_API isDirExist(const string& fullPath);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	bool MICRO_LEGEND_FRAME_API copyFile(const string& sourceFile, const string& destFile, bool overWrite = true);
	// 将文件夹sourcePath拷贝到destPath,sourcePath和destPath都是带以/结尾,overWrite指定当目标文件已经存在时是否要覆盖文件
	bool MICRO_LEGEND_FRAME_API copyFolder(const string& sourcePath, const string& destPath, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	bool MICRO_LEGEND_FRAME_API createFolder(const string& path);
	// 设置文件的访问权限
	bool MICRO_LEGEND_FRAME_API setFileAccess(const string& fileName, int mode);
	// 设置文件夹中所有文件的访问权限
	bool MICRO_LEGEND_FRAME_API setDirAccess(const string& path, int mode);
	bool MICRO_LEGEND_FRAME_API writeFile(const string& filePath, const char* buffer, int length, bool append = false);
	bool writeFile(const string& filePath, const string& text, bool append = false) { return writeFile(filePath, text.c_str(), (int)text.length(), append); }
	bool MICRO_LEGEND_FRAME_API writeFileSimple(const string& fileName, const char* buffer, int writeCount, bool append = false);
	bool writeFileSimple(const string& fileName, const string& text, bool append = false) { return writeFileSimple(fileName, text.c_str(), (int)text.length(), append); }
	bool MICRO_LEGEND_FRAME_API openFile(const string& filePath, FileContent* fileContent, bool addZero, bool showError = true);
	string MICRO_LEGEND_FRAME_API openTxtFile(const string& filePath, bool utf8ToANSI, bool showError = true);
	bool MICRO_LEGEND_FRAME_API openTxtFile(const string& filePath, string& fileContent, bool utf8ToANSI, bool showError = true);
	bool MICRO_LEGEND_FRAME_API openTxtFileLines(const string& filePath, Vector<string>& fileLines, bool utf8ToANSI, bool showError = true);
	bool openBinaryFile(const string& filePath, FileContent* fileContent, bool showError = true) { return openFile(filePath, fileContent, false, showError); }
	bool MICRO_LEGEND_FRAME_API moveFile(const string& fileName, const string& newName, bool forceCover = false);
	bool MICRO_LEGEND_FRAME_API renameFile(const string& fileName, const string& newName, bool forceCover = false);
	bool MICRO_LEGEND_FRAME_API renameFolder(const string& folderName, const string& newName, bool forceCover = false);
	string MICRO_LEGEND_FRAME_API generateFileMD5(const string& fileName);
	string MICRO_LEGEND_FRAME_API generateFileMD5(const string& fileName, char* buffer, int bufferSize);
	string MICRO_LEGEND_FRAME_API generateFileMD5(const char* buffer, int bufferSize);
	int MICRO_LEGEND_FRAME_API getFileSize(const string& filePath);
}

using FileUtility::writeFile;
using FileUtility::openTxtFile;
using FileUtility::findFiles;
using FileUtility::deleteFile;
using FileUtility::openBinaryFile;
using FileUtility::openTxtFileLines;
using FileUtility::generateFileMD5;