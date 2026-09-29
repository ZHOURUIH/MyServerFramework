#include "FrameHeader.h"

void FrameConfigSystem::init()
{
	readConfig(parseConfig(FrameDefine::CONFIG_PATH + "FrameConfig.txt"));
}

void FrameConfigSystem::destory()
{
	DELETE_ARRAY(mEncryptKey);
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
		else if (paramName == "SSL_CERTIFICATION_FILE")
		{
			mSSLCertificationFile = paramValue;
		}
		else if (paramName == "SSL_PRIVATE_KEY_FILE")
		{
			mSSLPrivateKeyFile = paramValue;
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
		else if (paramName == "SERVER_ID")
		{
			mServerID = SToI(paramValue);
		}
		else if (paramName == "KEY_0")
		{
			mKey0 = SToI(paramValue);
		}
		else if (paramName == "KEY_1")
		{
			mKey1 = SToI(paramValue);
		}
		else if (paramName == "KEY_2")
		{
			mKey2 = SToI(paramValue);
		}
		else if (paramName == "KEY_3")
		{
			mKey3 = SToI(paramValue);
		}
		else if (paramName == "ENCRYPT_KEY")
		{
			mEncryptKeyLength = (int)paramValue.size() / 2;
			if (!isPow2(mEncryptKeyLength))
			{
				ERROR("网络密钥的长度必须是2的n次方,当前长度为:" + IToS(mEncryptKeyLength));
			}
			mEncryptKey = new byte[mEncryptKeyLength];
			hexStringToBytes(paramValue, mEncryptKey, mEncryptKeyLength);
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
		tempBuffer.clear();
		// 首先去掉所有的空格和制表符
		FOR(lineString.length())
		{
			const char c = lineString[i];
			if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && !tempBuffer.add(c))
			{
				break;
			}
		}
		const string newString(tempBuffer.str());
		// 如果该行是空的,或者是注释,则不进行处理
		if (!newString.empty() && !startWith(newString, "//"))
		{
			ArrayList<2, string> valueArray;
			if (splitFull(newString, "=", valueArray))
			{
				valueList.emplace(valueArray[0], valueArray[1]);
			}
		}
	}
	return valueList;
}