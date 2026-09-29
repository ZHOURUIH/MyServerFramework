#pragma once

#include "UsingSTD.h"
#include "FrameClassDeclare.h"

// 为了避免回调函数的定义过于分散,所以将一些不需要包含头文件的回调函数定义放在这个文件中,需要包含头文件的回调函数定义放在FrameCallback.h中

template<typename Callback, typename... Args>
void CALL(Callback&& cb, Args&&... args)
{
	if (cb != nullptr)
	{
		forward<Callback>(cb)(forward<Args>(args)...);
	}
}

typedef void (*KeyFrameCallback)(ComponentKeyframe* component, bool breakTrembling, void* userData);
typedef void (*TrackCallback)(ComponentTrackTarget* component, bool breakTrack, void* userData);
typedef void (*OnObjectDestroy)(MovableObject* obj, void* userData);
typedef void (*EventCallback)(GameEvent* param, IEventListener* listener);
typedef void (*SQLiteParseFunction)(void* ptr, SQLiteDataReader* reader, int index);
typedef void (*SQLiteInsertFunction)(char* str, int size, void* ptr, bool notLastOne);
typedef void (*SQLiteUpdateFunction)(char* str, int size, const char* col, void* ptr, bool notLastOne);
typedef void (*CommandCallback)(GameCommand* cmd, void* user_data);
typedef void (*ServerCheckPingCallback)(TCPServerClient* client, int index);
typedef void (*WebServerCheckPingCallback)(WebSocketServerClient* client, int index);
typedef void (*FreezeAccountCallback)(llong accountGUID, llong timeSecond, const char* reason);
#ifdef _LIBEVENT
typedef void (*HttpCallback)(const string& uri, string& response, void* userData);
#endif

typedef function<void()> VoidCallback;
typedef function<void(void* ptr)> VoidPtrCallback;
typedef function<void(bool result)> BoolCallback;
typedef function<void(int value)> IntCallback;
typedef function<void(float value)> FloatCallback;
typedef function<void(llong dataCount)> LLongCallback;
typedef function<void(const string& value)> StringCallback;
typedef function<void(MySQLData* data)> MySQLDataCallback;
typedef function<void(CharacterState* state)> CharacterStateCallback;
typedef function<void(TCPServerClient* client)> TCPServerClientCallback;
typedef function<void(WebSocketServerClient* client)> WebSocketServerClientCallback;
typedef function<void(string&& log, string&& file)> ErrorProfileEvent;
typedef function<void(llong accountGUID, llong unfreezeTime, const string& reason)> FreezeAccountEvent;
// 带返回值的回调
typedef function<bool()> BoolFunction;
template <typename T>
using BoolFunctionT = function<bool(const T&)>;
template <typename T0, typename T1>
using BoolFunctionT2 = function<bool(T0, T1)>;
template <typename T>
using QueryDataCallbackT = function<void(T* data)>;
template <typename T>
using PtrCallbackT = function<void(T*)>;
template <typename T>
using CallbackT = function<void(T)>;