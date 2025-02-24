#ifdef _LIBEVENT
#include "FrameHeader.h"

void HttpServerSystem::init()
{
#ifndef WINDOWS
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		return;
	}
#endif
	mPort = mFrameConfigSystem->getConfigInt(FRAME_CONFIG::HTTP_PORT);
	if (mPort == 0)
	{
		return;
	}

	event_config* cfg = event_config_new();
	mEventBase = event_base_new_with_config(cfg);
	event_config_free(cfg);
	if (mEventBase == nullptr)
	{
		return;
	}

	mHttpServer = evhttp_new(mEventBase);
	if (mHttpServer == nullptr)
	{
		ERROR("couldn't create evhttp. Exiting.");
		return;
	}

	evhttp_set_gencb(mHttpServer, httpResponse, this);

	evhttp_bound_socket* handle = evhttp_bind_socket_with_handle(mHttpServer, "0.0.0.0", mPort);
	if (handle == nullptr)
	{
		ERROR("couldn't bind to port " + IToS(mPort) + ". Exiting.");
		return;
	}

	if (displayListenSock(handle) != 0)
	{
		return;
	}
}

void HttpServerSystem::update(float elapsedTime)
{
	event_base_loop(mEventBase, EVLOOP_NONBLOCK);
}

void HttpServerSystem::listenHttp(const string& uri, const HttpCallback& callback, void* userData, const string& contentType)
{
	HttpListenInfo* info = new HttpListenInfo();
	info->mURI = uri;
	info->mCallback = callback;
	info->mUserData = userData;
	info->mContentType = contentType.size() > 0 ? contentType : "application/json;charset=UTF-8";
	mCallbackList.insert(uri, info);
}

void HttpServerSystem::destroy()
{
	if (mHttpServer != nullptr)
	{
		evhttp_free(mHttpServer);
		mHttpServer = nullptr;
	}
	if (mEventBase != nullptr)
	{
		event_base_free(mEventBase);
		mEventBase = nullptr;
	}
}

// 解析http头，主要用于get请求时解析uri和请求参数
char* HttpServerSystem::findHttpHeader(struct evhttp_request* req, struct evkeyvalq* params, const char* query_char)
{
	if (req == nullptr || params == nullptr || query_char == nullptr)
	{
		ERROR("input params is null.");
		return nullptr;
	}

	// 获取请求uri
	const char* uri = evhttp_request_get_uri(req);
	if (uri == nullptr)
	{
		ERROR("evhttp_request_get_uri return null");
		return nullptr;
	}

	// 解码uri
	evhttp_uri* decoded = evhttp_uri_parse(uri);
	if (decoded == nullptr)
	{
		ERROR("It's not a good URI. Sending BADREQUEST");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return nullptr;
	}

	// 获取uri中的参数部分
	char* query = (char*)evhttp_uri_get_query(decoded);
	if (query == nullptr)
	{
		ERROR("evhttp_uri_get_query return null");
		return nullptr;
	}

	// 查询指定参数的值
	evhttp_parse_query_str(query, params);
	return (char*)evhttp_find_header(params, query_char);
}

void HttpServerSystem::httpResponse(evhttp_request* req, void* arg)
{
	if (req == nullptr)
	{
		return;
	}

	const char* uri = evhttp_request_get_uri(req);

	HttpListenInfo* listenInfo = ((HttpServerSystem*)arg)->mCallbackList.tryGet(uri);
	if (listenInfo != nullptr)
	{
		const string& contentType = listenInfo->mContentType;
		listenInfo->mCallback(uri, getRequestParam(req), [req, contentType](const string& response)
		{
			doResponse(req, response, contentType);
		}, listenInfo->mUserData);
	}
	else
	{
		doResponse(req, "", "");
	}
}

string HttpServerSystem::getRequestParam(evhttp_request* req)
{
	evbuffer* buf = evhttp_request_get_input_buffer(req);
	const int len = (int)evbuffer_get_length(buf);
	const int length = getGreaterPower2(len + 1);
	char* params = ArrayPool::newCharArray(length);
	params[len] = '\0';
	evbuffer_remove(buf, params, len);
	const string paramStr = params;
	ArrayPool::deleteCharArray(params, length);
	return paramStr;
}

void HttpServerSystem::doResponse(evhttp_request* req, const string& response, const string& contentType)
{
	evbuffer* retbuff = evbuffer_new();
	if (retbuff == nullptr)
	{
		ERROR("retbuff is null");
		return;
	}
	if (contentType.length() > 0)
	{
		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", contentType.c_str());
	}
	evbuffer_add_printf(retbuff, "%s", response.c_str());
	evhttp_send_reply(req, HTTP_OK, "Client", retbuff);
	evbuffer_free(retbuff);
}

int HttpServerSystem::displayListenSock(struct evhttp_bound_socket* handle)
{
	sockaddr_storage ss;
	ev_socklen_t socklen = sizeof(ss);
	evutil_socket_t fd = evhttp_bound_socket_get_fd(handle);
	memset(&ss, 0, sizeof(ss));
	if (getsockname(fd, (struct sockaddr*)&ss, &socklen) != 0)
	{
		ERROR("getsockname() failed");
		return 1;
	}

	void* inaddr = nullptr;
	int got_port = -1;
	if (ss.ss_family == AF_INET)
	{
		got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
		inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
	}
	else if (ss.ss_family == AF_INET6)
	{
		got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
		inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
	}
	else
	{
		ERROR("Weird address family " + IToS(ss.ss_family));
		return 1;
	}

	char addrbuf[128];
	const char* addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
	if (addr == nullptr)
	{
		ERROR("evutil_inet_ntop failed");
		return 1;
	}
	LOG("Listening on " + string(addr) + ":" + IToS(got_port));
	char uri_root[512];
	evutil_snprintf(uri_root, sizeof(uri_root), "http://%s:%d", addr, got_port);
	return 0;
}
#endif