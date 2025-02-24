#ifndef _SERVER_DEFINE_H_
#define _SERVER_DEFINE_H_

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID PLATFORM_LINUX

// 正在运行的平台标识
#ifdef WINDOWS
#define RUN_PLATFORM PLATFORM_WINDOWS
#endif

#ifdef LINUX
#define RUN_PLATFORM PLATFORM_LINUX
#endif

#ifndef RUN_PLATFORM
#define RUN_PLATFORM -1
#error "wrong platform!"
#endif

// 最大并发连接数为64,需要在winsock.h之前进行定义
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 64

// 由于需要简化代码,并且尽量不会引入第三方库,所以全局开放std命名空间
using namespace std;

#if RUN_PLATFORM == PLATFORM_WINDOWS
// 链接静态库
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")
#ifdef _MYSQL
#pragma comment(lib, "libmysql.lib")
#endif
#pragma warning(disable: 4005)
// libevent的头文件只能在windows库文件之前包含,否则会有定义冲突的报错
// 部分平台未安装libevent,所以需要使用宏来判断是否需要编译libevent相关代码
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <winsock.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <shlwapi.h>
#endif
#if RUN_PLATFORM == PLATFORM_LINUX
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <locale.h>
#include <execinfo.h>
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <fcntl.h>
#include <atomic>
#include <array>

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型简化定义
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 宏定义
#if RUN_PLATFORM == PLATFORM_WINDOWS
#define MY_THREAD HANDLE
#define MY_SOCKET SOCKET
#define NULL_THREAD NULL
#define THREAD_CALLBACK_DECLEAR(func) static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func) DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args) thread = CreateThread(NULL, 0, func, args, 0, NULL)
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	TerminateThread(thread, 0);	\
	CloseHandle(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) closesocket(socket);
#define CLASS_NAME(T) string(typeid(T).name()).substr(strlen("class "))
#define SPRINTF(buffer, bufferSize, ...) sprintf_s(buffer, bufferSize, __VA_ARGS__)
#define MEMCPY(dest, bufferSize, src, count) memcpy_s((void*)(dest), bufferSize, (void*)(src), count)
// 获取不同平台下的字面常量字符串的UTF8编码字符串,只能处理字面常量,也就是在代码中写死的字符串
// windows下需要由GB2312转换为UTF8,而linux则直接就是UTF8的
#define UNIFIED_UTF8(constantString) StringUtility::ANSIToUTF8(constantString).c_str()
#elif RUN_PLATFORM == PLATFORM_LINUX
#define MY_THREAD pthread_t
#define MY_SOCKET unsigned int
#define NULL_THREAD 0
#define SOCKADDR_IN sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func) static void* func(void* args)
#define THREAD_CALLBACK(class, func) void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args) pthread_create(&thread, NULL, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	pthread_cancel(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket) close(socket);
#ifdef __GNUC__
#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"
#define LC_NAME_zh_CN   "zh_CN"
#endif
#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK
#define CLASS_NAME(T) removePreNumber(typeid(T).name())
#define SPRINTF(buffer, bufferSize, ...) sprintf(buffer, __VA_ARGS__)
#define MEMCPY(dest, bufferSize, src, count) memcpy((void*)(dest), (void*)(src), count)
#define UNIFIED_UTF8(constantString) constantString
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (unsigned int)~0
#endif

#ifndef NULL
#define NULL 0
#endif

#define NOT_FIND static_cast<size_t>(-1)

#ifdef _DEBUG
#define CAST dynamic_cast
#else
#define CAST static_cast
#endif

#define ZERO_MEMORY(buffer, size) memset(buffer, 0, size)
#define STR(t) #t
#define LINE_STR(v) STR(v)
// 设置value的指定位置pos的字节的值为byte,并且不影响其他字节
#define SET_BYTE(value, b, pos) value = (value & ~(0x000000FF << (8 * pos))) | (b << (8 * pos))
// 获得value的指定位置pos的字节的值
#define GET_BYTE(value, pos) (value & (0x000000FF << (8 * pos))) >> (8 * pos)
#define GET_BIT(value, pos) (((value & (1 << (pos))) >> (pos)) & 1)
#define SET_BIT(value, pos, bit) value = (value & ~(1 << (pos))) | ((bit) << (pos))
#define GET_HIGHEST_BIT(value) GET_BIT(value, sizeof(value) * 8 - 1)
#define SET_HIGHEST_BIT(value, bit) SET_BIT(value, sizeof(value) * 8 - 1, bit)
#define GET_LOWEST_BIT(value) GET_BIT(value, 0)
#define SET_LOWEST_BIT(value, bit) SET_BIT(value, 0, bit)
#define HAS_MASK(value, bitMask) ((value) & (1 << (bitMask))) != 0
#define _FILE_LINE_ "File : " + string(__FILE__) + ", Line : " + LINE_STR(__LINE__)
// 生成静态字符串常量的名字
#define NAME(name) STR_##name
// 创建一个消息对象
#define PACKET(classType, packet) classType* packet = mNetServer->createPacket(packet, NAME(classType))
// 创建一个事件参数对象
#define EVENT(classType, eventParam) classType* eventParam = mEventSystem->createEvent(eventParam, NAME(classType))
// 注册一个系统组件
#define REGISTE_SYSTEM(type)							\
{														\
	type* component = NEW(type, component);				\
	component->setName(STR(type));						\
	mFrameComponentVector.push_back(component);			\
	mFrameComponentMap.insert(STR(type), component);	\
}
// 将类的基类重命名为base,方便使用
#define BASE_CLASS(baseClass) typedef baseClass base
// 从对象池中创建一个MySQLData对象
#define MYSQL_DATA(className, var) auto var = mMySQLDataBase->createData<className>(NAME(className))

//--------------------------------------------------------------------------------------------------------------------------------------------
// 再次封装后的容器的遍历宏
#ifdef _DEBUG
// 需要在循环结束后添加END
#define FOR(stl, expression) stl.lock(STL_LOCK::WRITE, __FILE__, __LINE__);for (expression)
#define END(stl) stl.unlock(STL_LOCK::WRITE);
#else
#define FOR(stl, expression)for (expression)
#define END(stl)
#endif

// 使用迭代器遍历列表,要在循环结束后添加END
#define FOREACH(iter, stl)\
auto iter = stl.begin();\
auto iter##End = stl.end();\
FOR(stl, ; iter != iter##End; ++iter)

// 使用迭代器遍历列表,不需要在循环结束后添加END
#define FOREACH_CONST(iter, stl)\
auto iter = stl.cbegin();\
auto iter##End = stl.cend();\
for(; iter != iter##End; ++iter)

// 使用下标遍历列表,需要在循环结束后添加END
#define FOR_VECTOR(stl) uint stl##Count = stl.size(); FOR(stl, uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_J(stl) uint stl##Count = stl.size(); FOR(stl, uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_K(stl) uint stl##Count = stl.size(); FOR(stl, uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_INVERSE(stl) uint stl##Count = stl.size(); FOR(stl, int i = stl##Count - 1; i >= 0; --i)
// 使用下标遍历常量列表,不需要在循环结束后添加END
#define FOR_VECTOR_CONST(stl) uint stl##Count = stl.size(); for(uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_CONST_J(stl) uint stl##Count = stl.size(); for(uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_CONST_K(stl) uint stl##Count = stl.size(); for(uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_CONST_INVERSE(stl) uint stl##Count = stl.size(); for(stl, int i = stl##Count - 1; i >= 0; --i)
// 简单的for循环
#define FOR_I(count) for (uint i = 0; i < count; ++i)
#define FOR_J(count) for (uint j = 0; j < count; ++j)
#define FOR_K(count) for (uint k = 0; k < count; ++k)
#define FOR_INVERSE_I(count) for (int i = count - 1; i >= 0; --i)
#define FOR_INVERSE_J(count) for (int j = count - 1; j >= 0; --j)
#define FOR_INVERSE_K(count) for (int k = count - 1; k >= 0; --k)
#define FOR_ONCE for(byte tempI = 0; tempI < 1; ++tempI)

//--------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型转字符串
// 以_STR结尾的是构造一个char[]类型的字符串
#define INT_STR(strBuffer, value) \
char strBuffer[16];\
StringUtility::intToString(strBuffer, 16, value);

#define FLOAT_STR(strBuffer, value) \
char strBuffer[16];\
StringUtility::floatToString(strBuffer, 16, value);

#define ULLONG_STR(strBuffer, value)\
 char strBuffer[32];\
StringUtility::ullongToString(strBuffer, 32, value);

#define INTS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[16 * bufferCount];\
StringUtility::intsToString(strBuffer, 16 * bufferCount, valueArray, count);

#define FLOATS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[16 * bufferCount];\
StringUtility::floatsToString(strBuffer, 16 * bufferCount, valueArray, count);

#define ULLONGS_STR(strBuffer, valueArray, bufferCount, count) \
char strBuffer[20 * bufferCount];\
StringUtility::ullongsToString(strBuffer, 20 * bufferCount, valueArray, count);

// 以_CHARS结尾的构造出array<char, SIZE>类型的字符串
#define INT_CHARS(strBuffer, value) \
array<char, 16> strBuffer{0};\
StringUtility::intToString(strBuffer, value);

#define FLOAT_CHARS(strBuffer, value) \
array<char, 16> strBuffer{0};\
StringUtility::floatToString(strBuffer, value);

#define ULLONG_CHARS(strBuffer, value) \
array<char, 32> strBuffer{0};\
StringUtility::ullongToString(strBuffer, value);

#define INTS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 16 * bufferCount> strBuffer{0};\
StringUtility::intsToString(strBuffer, valueArray, count);

#define FLOATS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 16 * bufferCount> strBuffer{0};\
StringUtility::floatsToString(strBuffer, valueArray, count);

#define ULLONGS_CHARS(strBuffer, valueArray, bufferCount, count) \
array<char, 32 * bufferCount> strBuffer{0};\
StringUtility::ullongsToString(strBuffer, valueArray, count);

// 字符串拼接,将str0,str1等字符串拼接后放入charArray中,会覆盖charArray中的内容
// charArray为array<char, int>类型
#define STRCAT2(charArray, str0, str1)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);

#define STRCAT3(charArray, str0, str1, str2)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);

#define STRCAT4(charArray, str0, str1, str2, str3)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);\
StringUtility::strcat_s(charArray, str3);

#define STRCAT5(charArray, str0, str1, str2, str3, str4)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);\
StringUtility::strcat_s(charArray, str3);\
StringUtility::strcat_s(charArray, str4);

#define STRCAT6(charArray, str0, str1, str2, str3, str4, str5)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);\
StringUtility::strcat_s(charArray, str3);\
StringUtility::strcat_s(charArray, str4);\
StringUtility::strcat_s(charArray, str5);

#define STRCAT7(charArray, str0, str1, str2, str3, str4, str5, str6)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);\
StringUtility::strcat_s(charArray, str3);\
StringUtility::strcat_s(charArray, str4);\
StringUtility::strcat_s(charArray, str5);\
StringUtility::strcat_s(charArray, str6);

#define STRCAT8(charArray, str0, str1, str2, str3, str4, str5, str6, str7)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, str0);\
StringUtility::strcat_s(charArray, str1);\
StringUtility::strcat_s(charArray, str2);\
StringUtility::strcat_s(charArray, str3);\
StringUtility::strcat_s(charArray, str4);\
StringUtility::strcat_s(charArray, str5);\
StringUtility::strcat_s(charArray, str6);\
StringUtility::strcat_s(charArray, str7);

// 字符串拼接,将str0,str1等字符串拼接后放入charArray中,会覆盖charArray中的内容
// charArray为char[]类型,_N表示普通数组
#define STRCAT2_N(charArray, size, str0, str1)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);

#define STRCAT3_N(charArray, size, str0, str1, str2)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);

#define STRCAT4_N(charArray, size, str0, str1, str2, str3)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);\
StringUtility::strcat_s(charArray, size, str3);

#define STRCAT5_N(charArray, size, str0, str1, str2, str3, str4)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);\
StringUtility::strcat_s(charArray, size, str3);\
StringUtility::strcat_s(charArray, size, str4);

#define STRCAT6_N(charArray, size, str0, str1, str2, str3, str4, str5)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);\
StringUtility::strcat_s(charArray, size, str3);\
StringUtility::strcat_s(charArray, size, str4);\
StringUtility::strcat_s(charArray, size, str5);

#define STRCAT7_N(charArray, size, str0, str1, str2, str3, str4, str5, str6)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);\
StringUtility::strcat_s(charArray, size, str3);\
StringUtility::strcat_s(charArray, size, str4);\
StringUtility::strcat_s(charArray, size, str5);\
StringUtility::strcat_s(charArray, size, str6);

#define STRCAT8_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7)\
charArray[0] = '\0';\
StringUtility::strcat_s(charArray, size, str0);\
StringUtility::strcat_s(charArray, size, str1);\
StringUtility::strcat_s(charArray, size, str2);\
StringUtility::strcat_s(charArray, size, str3);\
StringUtility::strcat_s(charArray, size, str4);\
StringUtility::strcat_s(charArray, size, str5);\
StringUtility::strcat_s(charArray, size, str6);\
StringUtility::strcat_s(charArray, size, str7);

// 字符串拼接,将str0,str1等字符串拼接在charArray中的字符串后面,不会覆盖charArray的内容
// charArray为array<char, int>类型
#define STR_APPEND1(charArray, str0)\
StringUtility::strcat_s(charArray, str0);

#define STR_APPEND2(charArray, str0, str1)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1);

#define STR_APPEND3(charArray, str0, str1, str2)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2);

#define STR_APPEND4(charArray, str0, str1, str2, str3)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2); \
StringUtility::strcat_s(charArray, str3);

#define STR_APPEND5(charArray, str0, str1, str2, str3, str4)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2); \
StringUtility::strcat_s(charArray, str3); \
StringUtility::strcat_s(charArray, str4);

#define STR_APPEND6(charArray, str0, str1, str2, str3, str4, str5)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2); \
StringUtility::strcat_s(charArray, str3); \
StringUtility::strcat_s(charArray, str4); \
StringUtility::strcat_s(charArray, str5);

#define STR_APPEND7(charArray, str0, str1, str2, str3, str4, str5, str6)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2); \
StringUtility::strcat_s(charArray, str3); \
StringUtility::strcat_s(charArray, str4); \
StringUtility::strcat_s(charArray, str5); \
StringUtility::strcat_s(charArray, str6);

#define STR_APPEND8(charArray, str0, str1, str2, str3, str4, str5, str6, str7)\
StringUtility::strcat_s(charArray, str0); \
StringUtility::strcat_s(charArray, str1); \
StringUtility::strcat_s(charArray, str2); \
StringUtility::strcat_s(charArray, str3); \
StringUtility::strcat_s(charArray, str4); \
StringUtility::strcat_s(charArray, str5); \
StringUtility::strcat_s(charArray, str6); \
StringUtility::strcat_s(charArray, str7);

// 字符串拼接,将str0,str1等字符串拼接在charArray中的字符串后面,不会覆盖charArray的内容
// charArray为char[]类型,_N表示普通数组
#define STR_APPEND1_N(charArray, size, str0)\
StringUtility::strcat_s(charArray, size, str0);

#define STR_APPEND2_N(charArray, size, str0, str1)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1);

#define STR_APPEND3_N(charArray, size, str0, str1, str2)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2);

#define STR_APPEND4_N(charArray, size, str0, str1, str2, str3)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2); \
StringUtility::strcat_s(charArray, size, str3);

#define STR_APPEND5_N(charArray, size, str0, str1, str2, str3, str4)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2); \
StringUtility::strcat_s(charArray, size, str3); \
StringUtility::strcat_s(charArray, size, str4);

#define STR_APPEND6_N(charArray, size, str0, str1, str2, str3, str4, str5)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2); \
StringUtility::strcat_s(charArray, size, str3); \
StringUtility::strcat_s(charArray, size, str4); \
StringUtility::strcat_s(charArray, size, str5);

#define STR_APPEND7_N(charArray, size, str0, str1, str2, str3, str4, str5, str6)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2); \
StringUtility::strcat_s(charArray, size, str3); \
StringUtility::strcat_s(charArray, size, str4); \
StringUtility::strcat_s(charArray, size, str5); \
StringUtility::strcat_s(charArray, size, str6);

#define STR_APPEND8_N(charArray, size, str0, str1, str2, str3, str4, str5, str6, str7)\
StringUtility::strcat_s(charArray, size, str0); \
StringUtility::strcat_s(charArray, size, str1); \
StringUtility::strcat_s(charArray, size, str2); \
StringUtility::strcat_s(charArray, size, str3); \
StringUtility::strcat_s(charArray, size, str4); \
StringUtility::strcat_s(charArray, size, str5); \
StringUtility::strcat_s(charArray, size, str6); \
StringUtility::strcat_s(charArray, size, str7);

//--------------------------------------------------------------------------------------------------------------------------------------------
// 日志打印相关宏
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(info)	cout << "error: " << (info + string(" | ") + _FILE_LINE_) << endl
#define LOG(info)	cout << info << endl

//--------------------------------------------------------------------------------------------------------------------------------------------
// 线程锁相关宏
#ifdef LOCK
#undef LOCK
#endif
#define LOCK(lock) \
(lock).waitForUnlock(__FILE__, __LINE__);\
try\
{

#ifdef UNLOCK
#undef UNLOCK
#endif
#define UNLOCK(lock) \
}catch(...){}\
(lock).unlock()
//--------------------------------------------------------------------------------------------------------------------------------------------

//内存相关宏定义
//---------------------------------------------------------------------------------------------------------------------
#ifdef CHECK_MEMORY
// 带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)	\
NULL;									\
ptr = new className(__VA_ARGS__);		\
if(ptr != NULL)							\
{										\
	MemoryCheck::usePtr(ptr);			\
}										\
else									\
{										\
	ERROR(string("can not alloc memory! className :") + STR(className));\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)		\
NULL;												\
if(count <= 0)										\
{													\
	ERROR("无法申请大小为0的数组");					\
}													\
ptr = new className[count];							\
if (ptr != NULL)									\
{													\
	MemoryCheck::usePtr(ptr);						\
}													\
else												\
{													\
	ERROR(string("can not alloc memory array! className : ") + STR(className) + ", count : " + StringUtility::intToString(count));\
}

#define NORMAL_DELETE(ptr)			\
if (ptr != NULL)					\
{									\
	MemoryCheck::unusePtr(ptr);	\
	delete ptr;						\
	ptr = NULL;						\
}

#define NORMAL_DELETE_ARRAY(ptr)	\
if (ptr != NULL)					\
{									\
	MemoryCheck::unusePtr(ptr);	\
	delete[] ptr;					\
	ptr = NULL;						\
}
#else
// 不带内存合法检测的常规内存申请和释放
#define NORMAL_NEW(className, ptr, ...)			\
NULL;											\
ptr = new className(__VA_ARGS__);				\
if(ptr == NULL)									\
{												\
	ERROR(string("can not alloc memory! className : ") + STR(className));\
}

#define NORMAL_NEW_ARRAY(className, count, ptr)		\
NULL;												\
if(count <= 0)										\
{													\
	ERROR("无法申请大小为0的数组");					\
}													\
ptr = new className[count];							\
if(ptr == NULL)										\
{													\
	ERROR(string("can not alloc memory array! className : ") + STR(className) + ", count : " + StringUtility::intToString(count));\
}

#define NORMAL_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

#define NORMAL_DELETE_ARRAY(ptr)\
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}
#endif

#ifdef NEW
#undef NEW
#endif

#ifdef DELETE
#undef DELETE
#endif

#ifdef NEW_ARRAY
#undef NEW_ARRAY
#endif

#ifdef DELETE_ARRAY
#undef DELETE_ARRAY
#endif

#ifdef TRACE_MEMORY
// 申请无参或者带参构造类的内存
#define NEW(className, ptr, ...)			\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	MemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// 申请无参构造的类或者基础数据类型数组内存
#define NEW_ARRAY(className, count, ptr)		\
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// 释放TRACE_NEW申请的内存
#define DELETE(ptr)					\
MemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// 释放TRACE_NEW_ARRAY申请的内存
#define DELETE_ARRAY(ptr)			\
MemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE_ARRAY(ptr)
#else
#define NEW(className, ptr, ...)			NORMAL_NEW(className, ptr, __VA_ARGS__)
#define NEW_ARRAY(className, count, ptr)	NORMAL_NEW_ARRAY(className, count, ptr)
#define DELETE(ptr)							NORMAL_DELETE(ptr)
#define DELETE_ARRAY(ptr)					NORMAL_DELETE_ARRAY(ptr)
#endif

//------------------------------------------------------------------------------------------------------------------------------------
// 自定义的基础头文件,因为这些头文件中可能会用到上面的宏,所以放在下面
#include "myVector.h"
#include "myMap.h"
#include "mySet.h"
#include "myStack.h"
#include "myQueue.h"
#include "mySafeVector.h"
#include "mySafeMap.h"
#include "mySafeSet.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------
class ServerDefine
{
public:
};

#endif