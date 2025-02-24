#include "FrameHeader.h"

void FrameConfigSystem::init()
{
	readConfig(parseConfig(FrameDefine::CONFIG_PATH + "FrameConfig.txt"));
}

void FrameConfigSystem::readConfig(const Vector<pair<string, string>>& valueList)
{
	for (const auto& iterValue : valueList)
	{
		const string& paramName = iterValue.first;
		const string& paramValue = iterValue.second;
		if (paramName == "TCP_PORT")
		{
			mTCPPort = SToI(paramValue);
		}
		else if (paramName == "UDP_PORT")
		{
			mUDPPort = SToI(paramValue);
		}
		else if (paramName == "WEB_SOCKET_PORT")
		{
			mWebSocketPort = SToI(paramValue);
		}
		else if (paramName == "HTTP_PORT")
		{
			mHttpPort = SToI(paramValue);
		}
		else if (paramName == "HEART_BEAT_TIME_OUT")
		{
			mHeartBeatTimeout = SToF(paramValue);
		}
		else if (paramName == "BACK_LOG")
		{
			mBacklog = SToI(paramValue);
		}
		else if (paramName == "SHOW_COMMAND_DEBUG_INFO")
		{
			mShowCommandDebugLog = SToI(paramValue);
		}
		else if (paramName == "OUTPUT_NET_LOG")
		{
			mOutputNetLog = SToI(paramValue);
		}
		else if (paramName == "MYSQL_PORT_NUMBER")
		{
			mMySQLPortNumber = SToI(paramValue);
		}
		else if (paramName == "DOMAIN_NAME")
		{
			mDomainName = paramValue;
		}
		else if (paramName == "SERVER_NAME")
		{
			mServerName = paramValue;
		}
		else if (paramName == "MYSQL_HOST")
		{
			mMySQLHost = paramValue;
		}
		else if (paramName == "MYSQL_USER")
		{
			mMySQLUser = paramValue;
		}
		else if (paramName == "MYSQL_PASSWORD")
		{
			mMySQLPassword = paramValue;
		}
		else if (paramName == "MYSQL_DATABASE")
		{
			mMySQLDataBase = paramValue;
		}
	}
}

Vector<pair<string, string>> FrameConfigSystem::parseConfig(const string& fileName)
{
	Vector<string> lineList;
	openTxtFileLines(fileName, lineList, false);

	// 每行最多只允许2048个字节
	MyString<2048> tempBuffer;
	// 将数据字符串拆分出来,放入valueList中
	Vector<pair<string, string>> valueList;
	for (const string& lineString : lineList)
	{
		// 首先去掉所有的空格和制表符
		int curLen = 0;
		FOR_I((int)lineString.length())
		{
			if (lineString[i] != ' ' && lineString[i] != '\t' && lineString[i] != '\r' && lineString[i] != '\n')
			{
				if (curLen >= tempBuffer.size())
				{
					break;
				}
				tempBuffer[curLen++] = lineString[i];
			}
		}
		tempBuffer[curLen] = 0;
		const string newString(tempBuffer.str());
		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && !startWith(newString, "//"))
		{
			ArrayList<2, string> valueArray;
			if (splitFull(newString, "=", valueArray))
			{
				valueList.emplace_back(valueArray[0], valueArray[1]);
			}
		}
	}
	return valueList;
}