#pragma once
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <string.h>

#include "Timer.h"

#include <unordered_map>
// #include""
class Channel;
class EventLoop;
// class HttpData;
#include <iostream>
#include <pthread.h>
using namespace std;

class MimeType
{
private:
	static void Init();
	static std::unordered_map<std::string, std::string> _mime;
	MimeType();
	MimeType(const MimeType &m);

public:
	static std::string GetMime(const std::string &suffix);

private:
	static pthread_once_t once_control;
};

enum METHOD
{
	GET = 0,
	POST,
	HEAD

};
enum STATUS
{
	REQUESTLINE = 0,
	STATE_HEADER,
	STATE_CONTENT,

};
enum PROCESSSTATE
{
	PARSEREQUESTLINE = 0,
	PARSEHEAD,
	PARSEBODY,
	PARSEREQUEST,
	STATE_FINISH,
};
enum LINE_STATUS
{
	LINE_OK = 0,
	LINE_BAD,
	LINE_OPEN
};
enum ConnectionState
{
	H_CONNECTED = 0,
	H_DISCONNECTING,
	H_DISCONNECTED
};
enum HEADSTATE
{
	PARSE_HEADER_SUCCESS = 1,
	PARSE_HEADER_AGAIN,
	PARSE_HEADER_ERROR
};
enum ANALYSISSTATE
{
	ANALYSISSUCESS = 0,
	ANALYSISERROR
};

const unsigned int DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const size_t DEFAULTALIVETIME = 2000;
class HttpData : public std::enable_shared_from_this<HttpData>
{
	// using CallBack = std::function<void()>;

public:
	HttpData(EventLoop *loop, int fd);
	~HttpData()
	{
		cout << "HttpData析构" << endl;
	}
	void Init();
	// HttpData()
	void reset();
	void DealWithRead();
	void DealWithWrite();
	void DealWithConn();
	void CloseConn();
	void NewConn();
	void SeperatrTimer();
	void HandleError(int fd, int errnum, string shortmsg);
	std::shared_ptr<Channel> GetChannel()
	{
		return _channel;
	}

	void LinkTimer(std::shared_ptr<TimerNode> mtimer)
	{
		_timer = mtimer;
	}

	// std::shared_ptr<Channel> &GetChannel() { return _channel; }

	// LINE_STATUS handlehead();

	// HEADSTATE ParseHeaders();

public:
	string inbuffer;
	string outbuffer;
	string method;
	string url;
	string version;
	METHOD _method;
	// string _Host;
	bool _error;
	bool _iskeepalive;
	ConnectionState _Connection;
	PROCESSSTATE STATEMENT;
	unordered_map<std::string, std::string> _headvalue;

	LINE_STATUS handlehead();
	HEADSTATE ParseHeaders();

	ANALYSISSTATE AnalysisRequest();
	void RequestLineAna();
	// void HandleError(int fd, int errnum, std::string shortmesg);

private:
	std::shared_ptr<Channel> _channel;
	// std::weak_ptr<HttpData> _httpdata;
	int _fd;
	EventLoop *_loop;
	std::weak_ptr<TimerNode> _timer;

private:
	// char _buff[1024];
	// std::string buffer;
};