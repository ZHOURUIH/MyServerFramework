#pragma once

// 最大并发连接数为128,需要在winsock.h之前进行定义
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 128

// 由于下面定义的部分宏容易在系统头文件中被定义,从而造成编译无法通关,所以先包含系统头文件,然后再定义自己的宏
#ifdef WINDOWS
// 链接静态库
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#ifdef _MYSQL
#pragma comment(lib, "libmysql.lib")
#endif
#pragma warning(disable: 4005)
#pragma warning(disable: 4251)
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
#endif
#ifdef LINUX
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
// linux上包含的libevent的头文件并不是项目中的文件,而是linux系统中的库文件
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
#include <functional>
#include <type_traits>
#include <fstream>
#include <array>
// 部分平台未安装mysql,所以需要使用宏来判断是否需要编译mysql相关代码
#ifdef _MYSQL
#include <mysql.h>
#endif

#include "sqlite3.h"
#include "md5.h"
// json只能带相对路径,否则在linux上会编译报错
#include "json/json.h"
#include "lz4.h"
#ifdef BUILDING_LIBCURL
#include "curl.h"
#endif

#ifdef WINDOWS
	#ifdef MICROLEGENDSERVERFRAME_EXPORTS
		#define MICRO_LEGEND_FRAME_API __declspec(dllexport)
	#else
		#define MICRO_LEGEND_FRAME_API __declspec(dllimport)
	#endif
#elif defined(__GNUC__)
	#define MICRO_LEGEND_FRAME_API __attribute__((visibility("default")))
#else
	#define MICRO_LEGEND_FRAME_API
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 宏定义
#ifdef WINDOWS
#define MY_THREAD							HANDLE
#define MY_SOCKET							SOCKET
#define NULL_THREAD							nullptr
#define THREAD_CALLBACK_DECLEAR(func)		static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func)		DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args)	thread = CreateThread(nullptr, 0, func, args, 0, nullptr)
#define CLOSE_THREAD(thread)		\
if (thread != NULL_THREAD)			\
{									\
	TerminateThread(thread, 0);		\
	CloseHandle(thread);			\
	thread = NULL_THREAD;			\
}
#define CLOSE_SOCKET(socket)						closesocket(socket);
#define SPRINTF(buffer, bufferSize, ...)			sprintf_s(buffer, bufferSize, __VA_ARGS__)
#define MEMCPY(dest, bufferSize, src, count)		memcpy_s((void*)(dest), (bufferSize), (void*)(src), (count))
#define MEMMOV(dest, bufferSize, src, count)		memmove_s((void*)(dest), (bufferSize), (void*)(src), (count))
// 获取不同平台下的字面常量字符串的UTF8编码字符串,只能处理字面常量,也就是在代码中写死的字符串
// windows下需要由GB2312转换为UTF8,而linux则直接就是UTF8的
// 将一个字面常量字符串转换为UTF8后存储为变量
#define UNIFIED_UTF8(var, size, constantString)		MyString<size> var; ANSIToUTF8(constantString, var.toBuffer(), size, false)
#define UNIFIED_UTF8_STRING(var, constantString)	string var = ANSIToUTF8(constantString, false)
#elif defined LINUX
#define MY_THREAD							pthread_t
#define MY_SOCKET							unsigned int
#define NULL_THREAD							0
#define SOCKADDR_IN							sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func)		static void* func(void* args)
#define THREAD_CALLBACK(class, func)		void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args)	pthread_create(&thread, nullptr, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	pthread_cancel(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket)				close(socket);
#ifdef __GNUC__
#define CSET_GBK							"GBK"
#define CSET_UTF8							"UTF-8"
#define LC_NAME_zh_CN						"zh_CN"
#endif
#define LC_NAME_zh_CN_GBK					LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8					LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT				LC_NAME_zh_CN_GBK
#define SPRINTF(buffer, bufferSize, ...)	snprintf(buffer, bufferSize, __VA_ARGS__)
// 因为bufferSize在windows下是需要的,linux下不需要,只是为了让宏与windows下的统一
#define MEMCPY(dest, bufferSize, src, count) memcpy((void*)(dest), (void*)(src), (count))
#define MEMMOV(dest, bufferSize, src, count) memmove((void*)(dest), (void*)(src), (count))
// 将一个字面常量字符串转换为UTF8后存储为变量
#define UNIFIED_UTF8(var, size, constantString)		MyString<size> var;	var.setString(constantString)
#define UNIFIED_UTF8_STRING(var, constantString)	string var = constantString
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (unsigned int)~0
#endif

#define STR(t)			#t
#define LINE_STR(v)		STR(v)
#define _FILE_LINE_		"File : " + string(__FILE__) + ", Line : " + LINE_STR(__LINE__)
// 通过定义多个宏的方式,改变宏的展开顺序,从而使__LINE__宏先展开,再进行拼接,达到自动根据行号定义一个唯一标识符的功能
#define MAKE_LABEL2(label, L) label##L
#define MAKE_LABEL1(label, L) MAKE_LABEL2(label, L)
#define UNIQUE_IDENTIFIER(label) MAKE_LABEL1(label, __LINE__)

//--------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型转字符串
// 以_STR结尾的是构造一个MyString<Length>类型的字符串
#define INT_STR(strBuffer, value)					\
MyString<16> strBuffer;								\
IToS(strBuffer, value)

#define UINT_STR(strBuffer, value)					\
MyString<16> strBuffer;								\
UIToS(strBuffer, value)

#define FLOAT_STR(strBuffer, value)					\
MyString<16> strBuffer;								\
FToS(strBuffer, value)

#define LLONG_STR(strBuffer, value)					\
MyString<32> strBuffer;								\
LLToS(strBuffer, value)

#define ULLONG_STR(strBuffer, value)				\
MyString<32> strBuffer;								\
ULLToS(strBuffer, value)

#define USHORTS_STR(strBuffer, valueArray, bufferCount, count)	\
MyString<16 * bufferCount> strBuffer;								\
USsToS(strBuffer, valueArray, count)

#define INTS_STR(strBuffer, valueArray, bufferCount, count)		\
MyString<16 * bufferCount> strBuffer;								\
IsToS(strBuffer, valueArray, count)

#define UINTS_STR(strBuffer, valueArray, bufferCount, count)	\
MyString<16 * bufferCount> strBuffer;								\
UIsToS(strBuffer, valueArray, count)

#define FLOATS_STR(strBuffer, valueArray, bufferCount, count)	\
MyString<16 * bufferCount> strBuffer;								\
FsToS(strBuffer, valueArray, count)

#define ULLONGS_STR(strBuffer, valueArray, bufferCount, count)	\
MyString<20 * bufferCount> strBuffer;								\
ULLsToS(strBuffer, valueArray, count)

#define LLONGS_STR(strBuffer, valueArray, bufferCount, count)	\
MyString<20 * bufferCount> strBuffer;								\
LLsToS(strBuffer, valueArray, count)

//--------------------------------------------------------------------------------------------------------------------------------------------
// Scope
// 线程锁相关宏
#ifdef THREAD_LOCK
#undef THREAD_LOCK
#endif
#define THREAD_LOCK(lock) ThreadLockScope UNIQUE_IDENTIFIER(scope)(lock, __FILE__, __LINE__);

// SafeSetScope
#define SAFE_SET_SCOPE(safeList, readList)																														\
		auto& UNIQUE_IDENTIFIER(temp) = safeList;																												\
		SafeSetScope<remove_cv<remove_reference<decltype(UNIQUE_IDENTIFIER(temp))>::type>::type::ValueType> UNIQUE_IDENTIFIER(a)(UNIQUE_IDENTIFIER(temp));		\
		auto& readList = UNIQUE_IDENTIFIER(temp).startForeach()

// SafeVectorScope
#define SAFE_VECTOR_SCOPE(safeList, readList)																													\
		auto& UNIQUE_IDENTIFIER(temp) = safeList;																												\
		SafeVectorScope<remove_cv<remove_reference<decltype(UNIQUE_IDENTIFIER(temp))>::type>::type::ValueType> UNIQUE_IDENTIFIER(a)(UNIQUE_IDENTIFIER(temp));	\
		auto& readList = UNIQUE_IDENTIFIER(temp).startForeach()

// SafeHashMapScope
#define SAFE_HASHMAP_SCOPE(safeList, readList)																													\
		auto& UNIQUE_IDENTIFIER(temp) = safeList;																												\
		typedef remove_cv<remove_reference<decltype(UNIQUE_IDENTIFIER(temp))>::type>::type::KeyType UNIQUE_IDENTIFIER(KeyType);									\
		typedef remove_cv<remove_reference<decltype(UNIQUE_IDENTIFIER(temp))>::type>::type::ValueType UNIQUE_IDENTIFIER(ValueType);								\
		SafeHashMapScope<UNIQUE_IDENTIFIER(KeyType), UNIQUE_IDENTIFIER(ValueType)> UNIQUE_IDENTIFIER(a)(UNIQUE_IDENTIFIER(temp));								\
		auto& readList = UNIQUE_IDENTIFIER(temp).startForeach()

//--------------------------------------------------------------------------------------------------------------------------------------------
// 日志打印相关宏
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(info)									GameLogWrap::logError(info + string(" | ") + _FILE_LINE_, 0, true)
#define ERROR_NO_WRITE(info)						GameLogWrap::logError(info + string(" | ") + _FILE_LINE_, 0, false)
#ifdef _MYSQL
// info应该传入一个char*类型的字符串
#define ERROR_PROFILE(content)  UNIFIED_UTF8_STRING(UNIQUE_IDENTIFIER(info), content); FrameMySQLUtility::errorProfile(move(UNIQUE_IDENTIFIER(info)), __FILE__)
#else
#define ERROR_PROFILE(content)
#endif
// 以字符串格式化的形式打印日志,总长度不能超过1024个字节
#define LOG_F(...)										\
char UNIQUE_IDENTIFIER(str)[1024];						\
SPRINTF(UNIQUE_IDENTIFIER(str), 1024, __VA_ARGS__);		\
GameLogWrap::logInfo(UNIQUE_IDENTIFIER(str), 0, true, true)
#define LOG(info)									GameLogWrap::logInfo(info, 0, true, true)
#define LOG_NO_WRITE(info)							GameLogWrap::logInfo(info, 0, true, false)
#define LOG_NO_PRINT(info)							GameLogWrap::logInfo(info, 0, false, true)
#define PLAYER_ERROR(info, playerGUID)				GameLogWrap::logError(info + string(" | ") + _FILE_LINE_, playerGUID, true)
#define PLAYER_LOG(info, playerGUID)				GameLogWrap::logInfo(info, playerGUID, true, true)
#define PLAYER_LOG_NO_PRINT(info, playerGUID)		GameLogWrap::logInfo(info, playerGUID, false, true)

//---------------------------------------------------------------------------------------------------------------------
#define CMD_THREAD(classType, cmd)						\
auto* cmd = mCommandThreadPool->newClass<classType>();	\
cmd->setThreadCommand(true)

#define CMD_DELAY(classType, cmd)						\
auto* cmd = mCommandPool->newClass<classType>();		\
cmd->setDelayCommand(true)

#define CMD_THREAD_DELAY(classType, cmd)				\
auto* cmd = mCommandThreadPool->newClass<classType>();	\
cmd->setThreadCommand(true);							\
cmd->setDelayCommand(true)

// 为了简化代码书写而添加的宏
//--------------------------------------------------------------------------------------------------------------------------------------------
// 使用下标遍历列表
#define FOR_VECTOR(stl)			const int UNIQUE_IDENTIFIER(Count) = (stl).size(); for(int i = 0; i < UNIQUE_IDENTIFIER(Count); ++i)
#define FOR_VECTOR_J(stl)		const int UNIQUE_IDENTIFIER(Count) = (stl).size(); for(int j = 0; j < UNIQUE_IDENTIFIER(Count); ++j)
#define FOR_VECTOR_K(stl)		const int UNIQUE_IDENTIFIER(Count) = (stl).size(); for(int k = 0; k < UNIQUE_IDENTIFIER(Count); ++k)
#define FOR_VECTOR_INVERSE(stl) for(int i = (stl).size() - 1; i >= 0; --i)

// 简单的for循环
#define FOR_I(count)			const int UNIQUE_IDENTIFIER(Count) = (int)count; for (int i = 0; i < UNIQUE_IDENTIFIER(Count); ++i)
#define FOR_J(count)			const int UNIQUE_IDENTIFIER(Count) = (int)count; for (int j = 0; j < UNIQUE_IDENTIFIER(Count); ++j)
#define FOR_INVERSE_I(count)	for (int i = (int)count - 1; i >= 0; --i)
#define FOR_ONCE				for (int UNIQUE_IDENTIFIER(temp) = 0; UNIQUE_IDENTIFIER(temp) < 1; ++UNIQUE_IDENTIFIER(temp))

// 将将当前类重命名为This,类的基类重命名为base,方便使用
#define BASE(thisType, baseClass)	typedef thisType This;typedef baseClass base
#define THIS(thisType)				typedef thisType This

// 用于构造有一些类所需要的Param类名,比如预先定义#define ThisClass TestClass,在后面代码中就可以使用ThisClassParam来代替TestClassParam
// 目的是为了统一代码的编写,避免某些时候因为拷贝代码修改遗漏而造成的错误
#define ThisParamT0(T, Suffix) T##Suffix
#define ThisParamT1(T, Suffix) ThisParamT0(T, Suffix)
#define ThisClassParam ThisParamT1(ThisClass, Param)

// 调用一个回调函数,只是为了简化代码,减少大括号的书写
#define CALL(callback, ...) if (callback != nullptr){ callback(__VA_ARGS__); }

// 在update中执行,每隔interval秒执行一次逻辑,由于使用了静态变量,只适合用于单例
#define TICK_LOOP(elapsedTime, interval)									\
		static float UNIQUE_IDENTIFIER(timer) = 0.0f;						\
		if (tickTimerLoop(UNIQUE_IDENTIFIER(timer), elapsedTime, interval))

// 声明事件处理类
#define EVENT_PROCESS_DECLARE(listenerType, eventType)									\
template<>																				\
class EventProcess<eventType, listenerType> : public EventProcessBase					\
{																						\
public:																					\
	static EventProcess<eventType, listenerType>* mProcess;								\
	void onEventInternal(GameEvent* event, IEventListener* listener) override			\
	{																					\
		onEvent(static_cast<eventType*>(event), static_cast<listenerType*>(listener));	\
	}																					\
	void onEvent(eventType* event, listenerType* listener);								\
};
// 定义事件处理类
#define EVENT_PROCESS_IMPL(listenerType, eventType)\
EventProcess<eventType, listenerType>* EventProcess<eventType, listenerType>::mProcess = new EventProcess<eventType, listenerType>();\
void EventProcess<eventType, listenerType>::onEvent(eventType* event, listenerType* listener)
// 使用示例
// 头文件中,类外添加EVENT_PROCESS_DECLARE(ClassName, EventWillHitOther);
// 源文件中,代码添加EVENT_PROCESS_IMPL(ClassName, EventWillHitOther){}
