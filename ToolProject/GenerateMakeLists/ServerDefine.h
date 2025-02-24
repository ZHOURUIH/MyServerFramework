#ifndef _SERVER_DEFINE_H_
#define _SERVER_DEFINE_H_

// ƽ̨��ʶ��
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID PLATFORM_LINUX

// �������е�ƽ̨��ʶ
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

// ��󲢷�������Ϊ64,��Ҫ��winsock.h֮ǰ���ж���
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 64

// ������Ҫ�򻯴���,���Ҿ������������������,����ȫ�ֿ���std�����ռ�
using namespace std;

#if RUN_PLATFORM == PLATFORM_WINDOWS
// ���Ӿ�̬��
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")
#ifdef _MYSQL
#pragma comment(lib, "libmysql.lib")
#endif
#pragma warning(disable: 4005)
// libevent��ͷ�ļ�ֻ����windows���ļ�֮ǰ����,������ж����ͻ�ı���
// ����ƽ̨δ��װlibevent,������Ҫʹ�ú����ж��Ƿ���Ҫ����libevent��ش���
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
// �����������ͼ򻯶���
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

//-----------------------------------------------------------------------------------------------------------------------------------------------
// �궨��
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
// ��ȡ��ͬƽ̨�µ����泣���ַ�����UTF8�����ַ���,ֻ�ܴ������泣��,Ҳ�����ڴ�����д�����ַ���
// windows����Ҫ��GB2312ת��ΪUTF8,��linux��ֱ�Ӿ���UTF8��
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
// ����value��ָ��λ��pos���ֽڵ�ֵΪbyte,���Ҳ�Ӱ�������ֽ�
#define SET_BYTE(value, b, pos) value = (value & ~(0x000000FF << (8 * pos))) | (b << (8 * pos))
// ���value��ָ��λ��pos���ֽڵ�ֵ
#define GET_BYTE(value, pos) (value & (0x000000FF << (8 * pos))) >> (8 * pos)
#define GET_BIT(value, pos) (((value & (1 << (pos))) >> (pos)) & 1)
#define SET_BIT(value, pos, bit) value = (value & ~(1 << (pos))) | ((bit) << (pos))
#define GET_HIGHEST_BIT(value) GET_BIT(value, sizeof(value) * 8 - 1)
#define SET_HIGHEST_BIT(value, bit) SET_BIT(value, sizeof(value) * 8 - 1, bit)
#define GET_LOWEST_BIT(value) GET_BIT(value, 0)
#define SET_LOWEST_BIT(value, bit) SET_BIT(value, 0, bit)
#define HAS_MASK(value, bitMask) ((value) & (1 << (bitMask))) != 0
#define _FILE_LINE_ "File : " + string(__FILE__) + ", Line : " + LINE_STR(__LINE__)
// ���ɾ�̬�ַ�������������
#define NAME(name) STR_##name
// ����һ����Ϣ����
#define PACKET(classType, packet) classType* packet = mNetServer->createPacket(packet, NAME(classType))
// ����һ���¼���������
#define EVENT(classType, eventParam) classType* eventParam = mEventSystem->createEvent(eventParam, NAME(classType))
// ע��һ��ϵͳ���
#define REGISTE_SYSTEM(type)							\
{														\
	type* component = NEW(type, component);				\
	component->setName(STR(type));						\
	mFrameComponentVector.push_back(component);			\
	mFrameComponentMap.insert(STR(type), component);	\
}
// ����Ļ���������Ϊbase,����ʹ��
#define BASE_CLASS(baseClass) typedef baseClass base
// �Ӷ�����д���һ��MySQLData����
#define MYSQL_DATA(className, var) auto var = mMySQLDataBase->createData<className>(NAME(className))

//--------------------------------------------------------------------------------------------------------------------------------------------
// �ٴη�װ��������ı�����
#ifdef _DEBUG
// ��Ҫ��ѭ�����������END
#define FOR(stl, expression) stl.lock(STL_LOCK::WRITE, __FILE__, __LINE__);for (expression)
#define END(stl) stl.unlock(STL_LOCK::WRITE);
#else
#define FOR(stl, expression)for (expression)
#define END(stl)
#endif

// ʹ�õ����������б�,Ҫ��ѭ�����������END
#define FOREACH(iter, stl)\
auto iter = stl.begin();\
auto iter##End = stl.end();\
FOR(stl, ; iter != iter##End; ++iter)

// ʹ�õ����������б�,����Ҫ��ѭ�����������END
#define FOREACH_CONST(iter, stl)\
auto iter = stl.cbegin();\
auto iter##End = stl.cend();\
for(; iter != iter##End; ++iter)

// ʹ���±�����б�,��Ҫ��ѭ�����������END
#define FOR_VECTOR(stl) uint stl##Count = stl.size(); FOR(stl, uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_J(stl) uint stl##Count = stl.size(); FOR(stl, uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_K(stl) uint stl##Count = stl.size(); FOR(stl, uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_INVERSE(stl) uint stl##Count = stl.size(); FOR(stl, int i = stl##Count - 1; i >= 0; --i)
// ʹ���±���������б�,����Ҫ��ѭ�����������END
#define FOR_VECTOR_CONST(stl) uint stl##Count = stl.size(); for(uint i = 0; i < stl##Count; ++i)
#define FOR_VECTOR_CONST_J(stl) uint stl##Count = stl.size(); for(uint j = 0; j < stl##Count; ++j)
#define FOR_VECTOR_CONST_K(stl) uint stl##Count = stl.size(); for(uint k = 0; k < stl##Count; ++k)
#define FOR_VECTOR_CONST_INVERSE(stl) uint stl##Count = stl.size(); for(stl, int i = stl##Count - 1; i >= 0; --i)
// �򵥵�forѭ��
#define FOR_I(count) for (uint i = 0; i < count; ++i)
#define FOR_J(count) for (uint j = 0; j < count; ++j)
#define FOR_K(count) for (uint k = 0; k < count; ++k)
#define FOR_INVERSE_I(count) for (int i = count - 1; i >= 0; --i)
#define FOR_INVERSE_J(count) for (int j = count - 1; j >= 0; --j)
#define FOR_INVERSE_K(count) for (int k = count - 1; k >= 0; --k)
#define FOR_ONCE for(byte tempI = 0; tempI < 1; ++tempI)

//--------------------------------------------------------------------------------------------------------------------------------------------
// ������������ת�ַ���
// ��_STR��β���ǹ���һ��char[]���͵��ַ���
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

// ��_CHARS��β�Ĺ����array<char, SIZE>���͵��ַ���
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

// �ַ���ƴ��,��str0,str1���ַ���ƴ�Ӻ����charArray��,�Ḳ��charArray�е�����
// charArrayΪarray<char, int>����
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

// �ַ���ƴ��,��str0,str1���ַ���ƴ�Ӻ����charArray��,�Ḳ��charArray�е�����
// charArrayΪchar[]����,_N��ʾ��ͨ����
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

// �ַ���ƴ��,��str0,str1���ַ���ƴ����charArray�е��ַ�������,���Ḳ��charArray������
// charArrayΪarray<char, int>����
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

// �ַ���ƴ��,��str0,str1���ַ���ƴ����charArray�е��ַ�������,���Ḳ��charArray������
// charArrayΪchar[]����,_N��ʾ��ͨ����
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
// ��־��ӡ��غ�
#ifdef ERROR
#undef ERROR
#endif
#define ERROR(info)	cout << "error: " << (info + string(" | ") + _FILE_LINE_) << endl
#define LOG(info)	cout << info << endl

//--------------------------------------------------------------------------------------------------------------------------------------------
// �߳�����غ�
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

//�ڴ���غ궨��
//---------------------------------------------------------------------------------------------------------------------
#ifdef CHECK_MEMORY
// ���ڴ�Ϸ����ĳ����ڴ�������ͷ�
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
	ERROR("�޷������СΪ0������");					\
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
// �����ڴ�Ϸ����ĳ����ڴ�������ͷ�
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
	ERROR("�޷������СΪ0������");					\
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
// �����޲λ��ߴ��ι�������ڴ�
#define NEW(className, ptr, ...)			\
NORMAL_NEW(className, ptr, __VA_ARGS__)		\
if(ptr != NULL)								\
{											\
	MemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className), __FILE__, __LINE__, typeid(className).name())); \
}

// �����޲ι��������߻����������������ڴ�
#define NEW_ARRAY(className, count, ptr)		\
NORMAL_NEW_ARRAY(className, count, ptr)			\
if(ptr != NULL)									\
{												\
	txMemoryTrace::insertPtr(ptr, MemoryInfo(sizeof(className)* count, __FILE__, __LINE__, typeid(className).name())); \
}

// �ͷ�TRACE_NEW������ڴ�
#define DELETE(ptr)					\
MemoryTrace::erasePtr((void*)ptr);\
NORMAL_DELETE(ptr)

// �ͷ�TRACE_NEW_ARRAY������ڴ�
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
// �Զ���Ļ���ͷ�ļ�,��Ϊ��Щͷ�ļ��п��ܻ��õ�����ĺ�,���Է�������
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