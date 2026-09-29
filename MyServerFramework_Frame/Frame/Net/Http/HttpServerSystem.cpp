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
	mPort = mFrameConfigSystem->getHttpPort();
	if (mPort == 0)
	{
		return;
	}

	// 证书文件为空时使用普通HTTP,否则使用HTTPS
	const string& certificationFile = mFrameConfigSystem->getSSLCertificationFile();
	const bool useSSL = !certificationFile.empty();

	// libevent线程支持
#if WINDOWS
	if (evthread_use_windows_threads() == -1)
	{
		ERROR("evthread_use_windows_threads failed");
		return;
	}
#else
	if (evthread_use_pthreads() == -1)
	{
		ERROR("evthread_use_pthreads failed");
		return;
	}
#endif
	event_config* cfg = event_config_new();
	mEventBase = event_base_new_with_config(cfg);
	event_config_free(cfg);
	if (mEventBase == nullptr)
	{
		ERROR("event_base_new_with_config failed");
		return;
	}
	mHttpServer = evhttp_new(mEventBase);
	if (mHttpServer == nullptr)
	{
		ERROR("evhttp_new failed");
		return;
	}

	// 只有配置了证书时才启用SSL
	if (useSSL)
	{
		// 初始化OpenSSL
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSL_load_error_strings();

		// 创建SSL上下文
		mSSLContext = SSL_CTX_new(TLS_server_method());
		if (mSSLContext == nullptr)
		{
			ERROR("SSL_CTX_new failed");
			return;
		}
		// 设置TLS协议版本和加密套件
		SSL_CTX_set_min_proto_version(mSSLContext, TLS1_2_VERSION);
		SSL_CTX_set_options(mSSLContext,
			SSL_OP_NO_SSLv3 |
			SSL_OP_NO_TLSv1 |
			SSL_OP_NO_TLSv1_1 |
			SSL_OP_NO_COMPRESSION |
			SSL_OP_CIPHER_SERVER_PREFERENCE |
			SSL_OP_NO_RENEGOTIATION
		);

		SSL_CTX_set_cipher_list(
			mSSLContext,
			"HIGH:!aNULL:!eNULL:!EXPORT:!DES:!MD5:!PSK:!RC4");

		// 加载证书
		if (SSL_CTX_use_certificate_file(mSSLContext, certificationFile.c_str(), SSL_FILETYPE_PEM) != 1)
		{
			ERROR("证书加载失败");
			SSL_CTX_free(mSSLContext);
			mSSLContext = nullptr;
			return;
		}

		// 加载私钥
		const string privateKeyFile = mFrameConfigSystem->getSSLPrivateKeyFile();
		if (SSL_CTX_use_PrivateKey_file(mSSLContext, privateKeyFile.c_str(), SSL_FILETYPE_PEM) != 1)
		{
			ERROR("私钥加载失败");
			SSL_CTX_free(mSSLContext);
			mSSLContext = nullptr;
			return;
		}
		if (SSL_CTX_check_private_key(mSSLContext) != 1)
		{
			ERROR("证书与私钥不匹配");
			SSL_CTX_free(mSSLContext);
			mSSLContext = nullptr;
			return;
		}

		// HTTPS连接使用SSL bufferevent
		evhttp_set_bevcb(mHttpServer, [](struct event_base* base, void* arg) -> struct bufferevent*
		{
			SSL_CTX* sslContext = (SSL_CTX*)arg;
			SSL* ssl = SSL_new(sslContext);
			if (ssl == nullptr)
			{
				return nullptr;
			}
			return bufferevent_openssl_socket_new(base, -1, ssl, BUFFEREVENT_SSL_ACCEPTING, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		}, mSSLContext);
	}

	evhttp_set_gencb(mHttpServer, httpResponse, this);
	// 绑定端口
	evhttp_bound_socket* handle = evhttp_bind_socket_with_handle(mHttpServer, "0.0.0.0", mPort);
	if (handle == nullptr)
	{
		ERROR("无法绑定端口: " + IToS(mPort));
		return;
	}
	// 获取监听地址信息
	sockaddr_storage ss;
	ev_socklen_t socklen = sizeof(ss);
	evutil_socket_t fd = evhttp_bound_socket_get_fd(handle);
	if (getsockname(fd, (struct sockaddr*)&ss, &socklen) != 0)
	{
		ERROR("getsockname failed");
		return;
	}

	char addrbuf[128];
	const char* addr = "未知地址";
	int gotPort = -1;
	if (ss.ss_family == AF_INET)
	{
		auto sin = reinterpret_cast<sockaddr_in*>(&ss);
		gotPort = ntohs(sin->sin_port);
		addr = evutil_inet_ntop(AF_INET, &sin->sin_addr, addrbuf, sizeof(addrbuf));
	}
	else if (ss.ss_family == AF_INET6)
	{
		auto sin6 = reinterpret_cast<sockaddr_in6*>(&ss);
		gotPort = ntohs(sin6->sin6_port);
		addr = evutil_inet_ntop(AF_INET6, &sin6->sin6_addr, addrbuf, sizeof(addrbuf));
	}
	if (addr == nullptr)
	{
		ERROR("无法解析地址");
	}
	else
	{
		LOG(string("成功监听 ") + (useSSL ? "HTTPS" : "HTTP") + " 端口: " + addr + ":" + IToS(gotPort));
	}
}

void HttpServerSystem::update(float elapsedTime)
{
	if (mEventBase != nullptr)
	{
		event_base_loop(mEventBase, EVLOOP_NONBLOCK | EVLOOP_NO_EXIT_ON_EMPTY);
	}
}

void HttpServerSystem::listenHttp(const string& uri, HttpCallback callback, void* userData)
{
	HttpListenInfo* info = new HttpListenInfo();
	info->mCallback = callback;
	info->mUserData = userData;
	mCallbackList.add(uri, info);
}

void HttpServerSystem::doResponse(evhttp_request* req, const string& response, int code, const string& contentType)
{
	evbuffer* retbuff = evbuffer_new();
	if (retbuff == nullptr)
	{
		ERROR("retbuff is null");
		return;
	}
	const char* contentTypeStr = !contentType.empty() ? contentType.c_str() : "application/json;charset=UTF-8";
	evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", contentTypeStr);
	evbuffer_add_printf(retbuff, "%s", response.c_str());
	evhttp_send_reply(req, code, "Client", retbuff);
	evbuffer_free(retbuff);
	mRequestHeaders.remove(req);
}

void HttpServerSystem::destroy()
{
	if (mHttpServer != nullptr)
	{
		evhttp_free(mHttpServer);
		mHttpServer = nullptr;
	}
	if (mSSLContext != nullptr)
	{
		SSL_CTX_free(mSSLContext);
		mSSLContext = nullptr;
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
	if (params == nullptr || query_char == nullptr)
	{
		ERROR("input params is null.");
		return nullptr;
	}
	char* query = findHttpQuery(req);
	if (query == nullptr)
	{
		ERROR("evhttp_uri_get_query return null");
		return nullptr;
	}

	// 查询指定参数的值
	evhttp_parse_query_str(query, params);
	return (char*)evhttp_find_header(params, query_char);
}

char* HttpServerSystem::findHttpQuery(struct evhttp_request* req)
{
	if (req == nullptr)
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
	return (char*)evhttp_uri_get_query(decoded);
}

const string& HttpServerSystem::getHttpHeader(struct evhttp_request* req, const string& key)
{
	HashMap<string, string>* listPtr = mRequestHeaders.getPtr(req);
	if (listPtr == nullptr)
	{
		HashMap<string, string> tempHeaderList;
		ArrayList<2, string> tempPair;
		Vector<string> strs = split(findHttpQuery(req), '&');
		for (const string& str : strs)
		{
			if (splitFull(str.c_str(), "=", tempPair))
			{
				tempHeaderList.add(tempPair[0], tempPair[1]);
			}
		}
		mRequestHeaders.add(req, move(tempHeaderList));
		listPtr = mRequestHeaders.getPtr(req);
	}
	return listPtr->get(key);
}

string HttpServerSystem::getPostParam(struct evhttp_request* req)
{
	// 获取输入缓冲区（存储了POST数据体）
	struct evbuffer* input_buf = evhttp_request_get_input_buffer(req);
	// 获取数据长度
	const size_t data_len = evbuffer_get_length(input_buf);
	// 分配内存并读取数据
	char* data = new char[data_len + 1];
	evbuffer_remove(input_buf, data, data_len);
	data[data_len] = '\0'; // 字符串结尾（如果是二进制数据可省略）
	string ret(data);
	DELETE_ARRAY(data);
	return ret;
}

void HttpServerSystem::httpResponse(evhttp_request* req, void* arg)
{
	if (req == nullptr)
	{
		return;
	}

	const char* uri = evhttp_request_get_uri(req);
	evbuffer* retbuff = evbuffer_new();
	if (retbuff == nullptr)
	{
		ERROR("retbuff is null");
		return;
	}

	string response;
	HttpListenInfo* listenInfo = ((HttpServerSystem*)arg)->mCallbackList.get(uri, nullptr);
	if (listenInfo != nullptr)
	{
		listenInfo->mCallback(uri, response, listenInfo->mUserData);
		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", listenInfo->mContentType.c_str());
	}
	evbuffer_add_printf(retbuff, "%s", response.c_str());

	// CORS 关键头
	evhttp_add_header(evhttp_request_get_output_headers(req), "Access-Control-Allow-Origin", "*");
	evhttp_add_header(evhttp_request_get_output_headers(req), "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
	evhttp_add_header(evhttp_request_get_output_headers(req), "Access-Control-Allow-Headers", "Content-Type");

	evhttp_send_reply(req, HTTP_OK, "Client", retbuff);
	evbuffer_free(retbuff);
}

#endif