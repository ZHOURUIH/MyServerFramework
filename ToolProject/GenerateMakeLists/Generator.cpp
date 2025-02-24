#include "Generator.h"

myMap<string, myMap<string, string>> Generator::readConfig(const string& configPath)
{
	myVector<string> lines;
	openTxtFileLines(configPath, lines, false);
	myMap<string, myMap<string, string>> configList;
	string curCMakeFile;
	FOR_VECTOR(lines)
	{
		myVector<string> params;
		split(lines[i].c_str(), "=", params);
		if (params.size() != 2)
		{
			continue;
		}
		if (params[0] == "CMakeListsPath")
		{
			curCMakeFile = params[1];
			configList.insert(curCMakeFile, myMap<string, string>());
			continue;
		}

		configList[curCMakeFile].insert(params[0], params[1]);
	}
	END(lines);
	return configList;
}

void Generator::removeIgnorePath(myVector<string>& pathList, const myVector<string>& ignoreList)
{
	uint ignoreCount = ignoreList.size();
	for (int i = 0; i < (int)pathList.size(); ++i)
	{
		FOR_J(ignoreCount)
		{
			if (startWith(pathList[i], ignoreList[j].c_str()))
			{
				pathList.erase(pathList.begin() + i);
				--i;
				break;
			}
		}
	}
}