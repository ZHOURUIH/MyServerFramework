#pragma once

#include "FrameComponent.h"
#include "HttpListenInfo.h"

class MICRO_LEGEND_FRAME_API HttpServerSystem : public FrameComponent
{
	BASE(HttpServerSystem, FrameComponent);
public:
#ifdef _LIBEVENT
	virtual ~HttpServerSystem() { destroy(); }
	void init() override;
	void destroy();
	void update(float elapsedTime) override;
	void listenHttp(const string& uri, const HttpCallback& callback, void* userData, const string& contentType = FrameDefine::EMPTY);
protected:
	static void doResponse(evhttp_request* req, const string& response, const string& contentType);
	static char* findHttpHeader(struct evhttp_request* req, struct evkeyvalq* params, const char* query_char);
	static int displayListenSock(struct evhttp_bound_socket* handle);
	static void httpResponse(evhttp_request* req, void* arg);
	static string getRequestParam(evhttp_request* req);
protected:
	static constexpr int mBufferSize = 1024 * 32;
	HashMap<string, HttpListenInfo*> mCallbackList;
	event_base* mEventBase = nullptr;
	evhttp* mHttpServer = nullptr;
	int mPort = 0;
#endif
};