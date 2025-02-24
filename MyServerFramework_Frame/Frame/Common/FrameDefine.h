#pragma once

#include "FrameMacro.h"

// 只开放部分std的内容,避免不必要的命名冲突
using std::vector;
using std::map;
using std::set;
using std::stack;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::wstring;
using std::atomic_flag;
using std::make_pair;
using std::exception;
using std::move;
using std::forward;
using std::is_same;
#ifdef WINDOWS
using std::conjunction;
#endif
using std::decay;
using std::function;
using std::atomic;
using std::enable_if_t;
using std::tuple;
using std::remove_cv;
using std::remove_reference;
using std::initializer_list;
using std::array;
using std::is_enum;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型简化定义
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long long llong;

// 自定义的基础头文件,因为这些头文件中可能会用到上面的宏,所以放在下面
#include "FrameEnum.h"
#include "FrameClassDeclare.h"
#include "Vector2.h"
#include "Vector2Int.h"
#include "Vector2UInt.h"
#include "Vector2Short.h"
#include "Vector2UShort.h"
#include "Vector4.h"
#include "Vector3.h"
#include "ArrayList.h"
#include "Array.h"
#include "Vector3Int.h"
#include "Vector4Int.h"
#include "Quaternion.h"
#include "Color.h"
#include "FrameStringDefine.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------
class MICRO_LEGEND_FRAME_API FrameDefine
{
public:
	// 常量数字定义
	static constexpr int UDP_MAX_PACKET_SIZE = 64 * 1024;			// udp单个消息包最大的大小,64KB
	static constexpr int CLIENT_MAX_PACKET_SIZE = 128 * 1024;		// 客户端临时缓冲区大小,应该不小于单个消息包最大的大小,128KB
	static constexpr int CLIENT_SEND_BUFFER = 256 * 1024;			// 客户端发送数据缓冲区大小,128KB
	static constexpr int CLIENT_RECV_BUFFER = 256 * 1024;			// 客户端接收数据缓冲区大小,128KB
	static constexpr int PACKET_TYPE_SIZE = sizeof(ushort);			// 包头中的包类型的变量类型大小
	static constexpr int PACKET_CRC_SIZE = sizeof(ushort);			// 消息包中的包CRC校验码的变量类型大小
	static constexpr int MIN_PARSE_COUNT = 5;						// 只有成功解析5个消息包以后的客户端才认为是有效客户端,当无效客户端接收到错误消息时直接断开连接并且不报错
	static constexpr int NOT_FIND = -1;								// 字符串查找未找到时的返回值
	static constexpr int ENCRYPT_KEY_LENGTH = 1 << 8;				// 网络消息加密密钥的长度
	static byte ENCRYPT_KEY[ENCRYPT_KEY_LENGTH];					// 网络消息加密密钥,使用对称加密的方式
	// 常量字符串定义
	static const string MEDIA_PATH;
	static const string MAP_PATH;
	static const string CONFIG_PATH;
	static const string LOG_PATH;
	static const string SQLITE_PATH;
	static const string EXCEL_PATH;
	static const string EMPTY;
	static const ullong FULL_MYSQL_FLAG = 0xFFFFFFFFFFFFFFFF;		// mysql缓存数据的位标识
	static const ullong FULL_FIELD_FLAG = 0xFFFFFFFFFFFFFFFF;		// 完全的网络消息标识位
};