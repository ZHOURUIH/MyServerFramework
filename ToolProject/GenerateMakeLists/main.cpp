#include "Generator.h"

int main()
{
	myMap<string, myMap<string, string>> configMap = Generator::readConfig("./GenerateMakeLists_Config.txt");
	for (const auto& configPair : configMap)
	{
		// 修改CMakeLists.txt文件中的目录列表部分和源文件列表部分
		const string& cmakeListsPath = StringUtility::toFullPath(configPair.first);
		myVector<string> makeLists;
		FileUtility::openTxtFileLines(cmakeListsPath, makeLists, true);
		int dirStart = -1;
		int dirEnd = -1;
		FOR_VECTOR(makeLists)
		{
			if (makeLists[i] == "INCLUDE_DIRECTORIES(")
			{
				dirStart = i + 1;
			}
			if (dirStart != -1 && makeLists[i] == "IF(WINDOWS)")
			{
				dirEnd = i - 1;
				break;
			}
		}
		END(makeLists);

		if (dirStart == -1 || dirEnd == -1)
		{
			ERROR("CMakeLists.txt文件解析错误");
			system("pause");
			return 0;
		}

		const auto& config = configPair.second;
		myVector<string> exceptDirList;
		StringUtility::split(config.get("ExcludeDir", "").c_str(), ",", exceptDirList);
		for (string& folder : exceptDirList)
		{
			folder = StringUtility::toFullPath(folder);
			StringUtility::rightToLeft(folder);
		}
		for (const string& item : exceptDirList)
		{
			cout << "排除的路径:" << item << endl;
		}
		myVector<string> searchPathList;
		StringUtility::split(config.get("Path", "").c_str(), ",", searchPathList);
		// 当前路径也要加进去
		myVector<string> dirList{"."};
		for (const string& item : searchPathList)
		{
			Generator::findFolders(StringUtility::toFullPath(item), dirList, true);
			for (string& folder : dirList)
			{
				StringUtility::rightToLeft(folder);
			}
			Generator::removeIgnorePath(dirList, exceptDirList);
		}
		for (const string& item : dirList)
		{
			cout << "查找到的路径:" << item << endl;
		}

		// 修改目录列表
		string curFullPath = StringUtility::getFilePath(cmakeListsPath);
		cout << "CMakeLists.txt路径:" << curFullPath << endl;
		makeLists.erase(dirStart, dirEnd - dirStart + 1);
		FOR_VECTOR(dirList)
		{
			string relativePath = StringUtility::toRelativePath(dirList[i], curFullPath);
			if (relativePath.empty())
			{
				relativePath = ".";
			}
			makeLists.insert(dirStart + i, relativePath + ";");
		}
		END(dirList);
		string finalFile = StringUtility::stringsToString(makeLists.data(), makeLists.size(), "\r\n");
		finalFile = StringUtility::ANSIToUTF8(finalFile.c_str(), true);
		FileUtility::writeFileSimple(cmakeListsPath, finalFile);
	}
	return 0;
}