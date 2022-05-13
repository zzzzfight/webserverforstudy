#include "HttpData.h"
#include "Channel.h"

#include <unistd.h>
#include <functional>

#include <iostream>
#include "EventLoop.h"
#include <string.h>
#include "./base/Util.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::_mime;

// const unsigned int DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;

char favicon[555] = {
	'\x89',
	'P',
	'N',
	'G',
	'\xD',
	'\xA',
	'\x1A',
	'\xA',
	'\x0',
	'\x0',
	'\x0',
	'\xD',
	'I',
	'H',
	'D',
	'R',
	'\x0',
	'\x0',
	'\x0',
	'\x10',
	'\x0',
	'\x0',
	'\x0',
	'\x10',
	'\x8',
	'\x6',
	'\x0',
	'\x0',
	'\x0',
	'\x1F',
	'\xF3',
	'\xFF',
	'a',
	'\x0',
	'\x0',
	'\x0',
	'\x19',
	't',
	'E',
	'X',
	't',
	'S',
	'o',
	'f',
	't',
	'w',
	'a',
	'r',
	'e',
	'\x0',
	'A',
	'd',
	'o',
	'b',
	'e',
	'\x20',
	'I',
	'm',
	'a',
	'g',
	'e',
	'R',
	'e',
	'a',
	'd',
	'y',
	'q',
	'\xC9',
	'e',
	'\x3C',
	'\x0',
	'\x0',
	'\x1',
	'\xCD',
	'I',
	'D',
	'A',
	'T',
	'x',
	'\xDA',
	'\x94',
	'\x93',
	'9',
	'H',
	'\x3',
	'A',
	'\x14',
	'\x86',
	'\xFF',
	'\x5D',
	'b',
	'\xA7',
	'\x4',
	'R',
	'\xC4',
	'm',
	'\x22',
	'\x1E',
	'\xA0',
	'F',
	'\x24',
	'\x8',
	'\x16',
	'\x16',
	'v',
	'\xA',
	'6',
	'\xBA',
	'J',
	'\x9A',
	'\x80',
	'\x8',
	'A',
	'\xB4',
	'q',
	'\x85',
	'X',
	'\x89',
	'G',
	'\xB0',
	'I',
	'\xA9',
	'Q',
	'\x24',
	'\xCD',
	'\xA6',
	'\x8',
	'\xA4',
	'H',
	'c',
	'\x91',
	'B',
	'\xB',
	'\xAF',
	'V',
	'\xC1',
	'F',
	'\xB4',
	'\x15',
	'\xCF',
	'\x22',
	'X',
	'\x98',
	'\xB',
	'T',
	'H',
	'\x8A',
	'd',
	'\x93',
	'\x8D',
	'\xFB',
	'F',
	'g',
	'\xC9',
	'\x1A',
	'\x14',
	'\x7D',
	'\xF0',
	'f',
	'v',
	'f',
	'\xDF',
	'\x7C',
	'\xEF',
	'\xE7',
	'g',
	'F',
	'\xA8',
	'\xD5',
	'j',
	'H',
	'\x24',
	'\x12',
	'\x2A',
	'\x0',
	'\x5',
	'\xBF',
	'G',
	'\xD4',
	'\xEF',
	'\xF7',
	'\x2F',
	'6',
	'\xEC',
	'\x12',
	'\x20',
	'\x1E',
	'\x8F',
	'\xD7',
	'\xAA',
	'\xD5',
	'\xEA',
	'\xAF',
	'I',
	'5',
	'F',
	'\xAA',
	'T',
	'\x5F',
	'\x9F',
	'\x22',
	'A',
	'\x2A',
	'\x95',
	'\xA',
	'\x83',
	'\xE5',
	'r',
	'9',
	'd',
	'\xB3',
	'Y',
	'\x96',
	'\x99',
	'L',
	'\x6',
	'\xE9',
	't',
	'\x9A',
	'\x25',
	'\x85',
	'\x2C',
	'\xCB',
	'T',
	'\xA7',
	'\xC4',
	'b',
	'1',
	'\xB5',
	'\x5E',
	'\x0',
	'\x3',
	'h',
	'\x9A',
	'\xC6',
	'\x16',
	'\x82',
	'\x20',
	'X',
	'R',
	'\x14',
	'E',
	'6',
	'S',
	'\x94',
	'\xCB',
	'e',
	'x',
	'\xBD',
	'\x5E',
	'\xAA',
	'U',
	'T',
	'\x23',
	'L',
	'\xC0',
	'\xE0',
	'\xE2',
	'\xC1',
	'\x8F',
	'\x0',
	'\x9E',
	'\xBC',
	'\x9',
	'A',
	'\x7C',
	'\x3E',
	'\x1F',
	'\x83',
	'D',
	'\x22',
	'\x11',
	'\xD5',
	'T',
	'\x40',
	'\x3F',
	'8',
	'\x80',
	'w',
	'\xE5',
	'3',
	'\x7',
	'\xB8',
	'\x5C',
	'\x2E',
	'H',
	'\x92',
	'\x4',
	'\x87',
	'\xC3',
	'\x81',
	'\x40',
	'\x20',
	'\x40',
	'g',
	'\x98',
	'\xE9',
	'6',
	'\x1A',
	'\xA6',
	'g',
	'\x15',
	'\x4',
	'\xE3',
	'\xD7',
	'\xC8',
	'\xBD',
	'\x15',
	'\xE1',
	'i',
	'\xB7',
	'C',
	'\xAB',
	'\xEA',
	'x',
	'\x2F',
	'j',
	'X',
	'\x92',
	'\xBB',
	'\x18',
	'\x20',
	'\x9F',
	'\xCF',
	'3',
	'\xC3',
	'\xB8',
	'\xE9',
	'N',
	'\xA7',
	'\xD3',
	'l',
	'J',
	'\x0',
	'i',
	'6',
	'\x7C',
	'\x8E',
	'\xE1',
	'\xFE',
	'V',
	'\x84',
	'\xE7',
	'\x3C',
	'\x9F',
	'r',
	'\x2B',
	'\x3A',
	'B',
	'\x7B',
	'7',
	'f',
	'w',
	'\xAE',
	'\x8E',
	'\xE',
	'\xF3',
	'\xBD',
	'R',
	'\xA9',
	'd',
	'\x2',
	'B',
	'\xAF',
	'\x85',
	'2',
	'f',
	'F',
	'\xBA',
	'\xC',
	'\xD9',
	'\x9F',
	'\x1D',
	'\x9A',
	'l',
	'\x22',
	'\xE6',
	'\xC7',
	'\x3A',
	'\x2C',
	'\x80',
	'\xEF',
	'\xC1',
	'\x15',
	'\x90',
	'\x7',
	'\x93',
	'\xA2',
	'\x28',
	'\xA0',
	'S',
	'j',
	'\xB1',
	'\xB8',
	'\xDF',
	'\x29',
	'5',
	'C',
	'\xE',
	'\x3F',
	'X',
	'\xFC',
	'\x98',
	'\xDA',
	'y',
	'j',
	'P',
	'\x40',
	'\x0',
	'\x87',
	'\xAE',
	'\x1B',
	'\x17',
	'B',
	'\xB4',
	'\x3A',
	'\x3F',
	'\xBE',
	'y',
	'\xC7',
	'\xA',
	'\x26',
	'\xB6',
	'\xEE',
	'\xD9',
	'\x9A',
	'\x60',
	'\x14',
	'\x93',
	'\xDB',
	'\x8F',
	'\xD',
	'\xA',
	'\x2E',
	'\xE9',
	'\x23',
	'\x95',
	'\x29',
	'X',
	'\x0',
	'\x27',
	'\xEB',
	'n',
	'V',
	'p',
	'\xBC',
	'\xD6',
	'\xCB',
	'\xD6',
	'G',
	'\xAB',
	'\x3D',
	'l',
	'\x7D',
	'\xB8',
	'\xD2',
	'\xDD',
	'\xA0',
	'\x60',
	'\x83',
	'\xBA',
	'\xEF',
	'\x5F',
	'\xA4',
	'\xEA',
	'\xCC',
	'\x2',
	'N',
	'\xAE',
	'\x5E',
	'p',
	'\x1A',
	'\xEC',
	'\xB3',
	'\x40',
	'9',
	'\xAC',
	'\xFE',
	'\xF2',
	'\x91',
	'\x89',
	'g',
	'\x91',
	'\x85',
	'\x21',
	'\xA8',
	'\x87',
	'\xB7',
	'X',
	'\x7E',
	'\x7E',
	'\x85',
	'\xBB',
	'\xCD',
	'N',
	'N',
	'b',
	't',
	'\x40',
	'\xFA',
	'\x93',
	'\x89',
	'\xEC',
	'\x1E',
	'\xEC',
	'\x86',
	'\x2',
	'H',
	'\x26',
	'\x93',
	'\xD0',
	'u',
	'\x1D',
	'\x7F',
	'\x9',
	'2',
	'\x95',
	'\xBF',
	'\x1F',
	'\xDB',
	'\xD7',
	'c',
	'\x8A',
	'\x1A',
	'\xF7',
	'\x5C',
	'\xC1',
	'\xFF',
	'\x22',
	'J',
	'\xC3',
	'\x87',
	'\x0',
	'\x3',
	'\x0',
	'K',
	'\xBB',
	'\xF8',
	'\xD6',
	'\x2A',
	'v',
	'\x98',
	'I',
	'\x0',
	'\x0',
	'\x0',
	'\x0',
	'I',
	'E',
	'N',
	'D',
	'\xAE',
	'B',
	'\x60',
	'\x82',
};

void MimeType::Init()
{
	_mime[".html"] = "text/html";
	_mime[".avi"] = "video/x-msvideo";
	_mime[".bmp"] = "image/bmp";
	_mime[".c"] = "text/plain";
	_mime[".doc"] = "application/msword";
	_mime[".gif"] = "image/gif";
	_mime[".gz"] = "application/x-gzip";
	_mime[".htm"] = "text/html";
	_mime[".ico"] = "image/x-icon";
	_mime[".jpg"] = "image/jpeg";
	_mime[".png"] = "image/png";
	_mime[".txt"] = "text/plain";
	_mime[".mp3"] = "audio/mp3";
	_mime["default"] = "text/html";
}
std::string MimeType::GetMime(const std::string &suffix)
{
	pthread_once(&once_control, MimeType::Init);
	if (_mime.find(suffix) == _mime.end())
	{
		return _mime["default"];
	}
	else
	{
		return _mime[suffix];
	}
}
HttpData::HttpData(EventLoop *loop, int fd)
	: _channel(new Channel(loop, fd)),
	  _loop(loop),
	  _fd(fd),
	  _Connection(H_CONNECTED),
	  STATEMENT(PARSEREQUESTLINE)
{
	// CallBack SetReadCb(CallBack&& funcb);
	// CallBack SetWriteCb(CallBack&& funcb);
	// CallBack SetConnCb(CallBack&& funcb);
	std::cout << "HTTPDATA:构造" << std::endl;
	// _channel->SetReadCb(std::bind(&HttpData::DealWithRead, this));
	// _channel->SetWriteCb(std::bind(&HttpData::DealWithWrite, this));
	// _channel->SetConnCb(std::bind(&HttpData::DealWithConn, this));
}
// HttpData::HttpData()
// {
// 	_channel->
// }
void HttpData::Init()
{
	_channel->SetReadCb(std::bind(&HttpData::DealWithRead, this));
	_channel->SetWriteCb(std::bind(&HttpData::DealWithWrite, this));
	_channel->SetConnCb(std::bind(&HttpData::DealWithConn, this));
}
void HttpData::reset()
{
	inbuffer.clear();
	url.clear();
	version.clear();
	// method.clear();
	_method = GET;
	STATEMENT = PARSEREQUESTLINE;

	_headvalue.clear();
	_error = false;

	// outbuffer.clear();
	// _iskeepalive = false;
	// nowReadPos_ = 0;
	// hState_ = H_START;

	// headers_.clear();
	if (_timer.lock())
	{
		shared_ptr<TimerNode> my_timer(_timer.lock());
		my_timer->ClearReq();
		_timer.reset();
	}
}

LINE_STATUS HttpData::handlehead()
{
	// size_t pos;
	if (STATEMENT == PARSEREQUESTLINE)
	{
		// string temp = inbuffer
		string &str = inbuffer;
		size_t pos = inbuffer.find('\r');
		if (pos < 0)
		{
			return LINE_OPEN;
		}
		string requestline = inbuffer.substr(0, pos);
		if (str.size() > pos + 2)
		{
			str = str.substr(pos + 2);
		}
		else
		{
			str.clear();
		}
		pos = requestline.find(' ');
		string method = requestline.substr(0, pos);
		if (!method.compare("GET"))
		{
			_method = GET;
		}
		else if (!method.compare("POST"))
		{
			_method = POST;
		}
		else if (!method.compare("HEAD"))
		{
			_method = HEAD;
		}
		requestline = requestline.substr(pos + 1);
		pos = requestline.find(' ');
		if (pos == string::npos)
		{
			return LINE_BAD;
		}
		url = requestline.substr(0, pos);
		if (0 == url.compare("/"))
		{
			url = "root" + url + "log.html";
		}
		else
		{
			// url = url.substr(1);
			url = "root" + url;
		}
		// size_t pos1 = url.find('?');
		// if (pos1 != string::npos)
		// {
		// 	url = url.substr(0, pos1);
		// }

		requestline = requestline.substr(pos + 1);
		version = requestline;
		cout << "-------" << endl;
		cout << method << endl;
		cout << url << endl;
		cout << version << endl;
		cout << "-------" << endl;
	}
	return LINE_OK;
}

HEADSTATE HttpData::ParseHeaders()
{
	string &str = inbuffer;
	// size_t key_start = -1, key_end = -1, value_start = -1, value_end = -1;
	// int now_read_line_begin = 0;
	// bool notfinish = true;
	// size_t i = 0;
	// bool headana = true;
	while (true)
	{
		size_t pos = str.find('\n');
		if (pos == string::npos)
		{
			if (str.size())
			{
				// headana = false;
				// cout << "endl" << endl;
				return PARSE_HEADER_AGAIN;
			}
		}
		if (str[0] == '\r' || str.size() == 0)
		{
			// STATEMENT = PARSEBODY;
			if (str.size() > 2)
			{
				str = str.substr(2);
			}
			else
			{
				str.clear();
			}
			break;
			// return PARSE_HEADER_SUCCESS;
		}
		string headline = str.substr(0, pos);
		if (str.size() > pos + 1)
			str = str.substr(pos + 1);
		else
			str.clear();

		// string value1 = str.substr(0, pos);
		pos = headline.find_first_of(':');
		if (pos == string::npos)
		{
			return PARSE_HEADER_ERROR;
		}
		string head(headline.begin(), headline.begin() + pos);
		string value(headline.begin() + pos + 2, headline.end() - 1);
		_headvalue[head] = value;
		// cout << head << ": " << value << endl;
	}

	return PARSE_HEADER_SUCCESS;
}

ANALYSISSTATE HttpData::AnalysisRequest()
{
	if (_method == POST)
	{
	}
	else if (_method == GET || _method == HEAD)
	{
		string header;
		header += "HTTP/1.1 200 OK\r\n";
		if (_headvalue.find("Connection") != _headvalue.end() && (_headvalue["Connection"] == "keep-alive") || (_headvalue["Connection"] == "Keep-Alive"))
		{
			_iskeepalive = true;
			header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" + to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
		}
		int dot_pos = url.find('.');
		string filetype;
		if (dot_pos == string::npos)
		{
			filetype = MimeType::GetMime("default");
		}
		else
		{
			filetype = MimeType::GetMime(url.substr(dot_pos));
		}
		cout << filetype << endl;
		if (url == "hello")
		{
			outbuffer = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
			return ANALYSISSUCESS;
		}
		if (url == "favicon.ico")
		{
			header += "Content-Type: image/png\r\n";
			header += "Content-Length: " + to_string(sizeof favicon) + "\r\n";
			header += "Server: ZHF's Web Server\r\n";
			header += "\r\n";
			outbuffer += header;
			outbuffer += string(favicon, favicon + sizeof favicon);
			return ANALYSISSUCESS;
		}
		struct stat sbuf;
		if (stat(url.c_str(), &sbuf) < 0)
		{
			header.clear();
			HandleError(_fd, 404, "Not Fount!");
			return ANALYSISERROR;
		}
		header += "Content-Type: " + filetype + "\r\n";
		header += "Content-Length: " + to_string(sbuf.st_size) + "\r\n";
		header += "Server: ZHF's web server\r\n";
		header += "\r\n";
		outbuffer += header;
		if (_method == HEAD)
			return ANALYSISSUCESS;

		int src_fd = open(url.c_str(), O_RDONLY, 0);
		if (src_fd < 0)
		{
			outbuffer.clear();
			// handleError();
			// _error = true;
			HandleError(_fd, 404, "Not Fount!");
			return ANALYSISERROR;
		}

		void *mmapret = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
		close(src_fd);
		if (mmapret == (void *)-1)
		{
			munmap(mmapret, sbuf.st_size);
			outbuffer.clear();
			// handleError;
			HandleError(_fd, 404, "Not Fount!");
			return ANALYSISERROR;
		}
		char *src_addr = static_cast<char *>(mmapret);
		// outbuffer.append()
		outbuffer += string(src_addr, src_addr + sbuf.st_size);
		munmap(mmapret, sbuf.st_size);
		return ANALYSISSUCESS;
	}
	return ANALYSISERROR;
}

void HttpData::DealWithRead()
{

	unsigned int &events = _channel->GetEvents();
	do
	{
		bool zero = false;
		int readnum = readn(_fd, inbuffer, zero);
		// if(readnum==0)
		if (_Connection == H_DISCONNECTING)
		{
			inbuffer.clear();
			break;
		}
		if (readnum < 0)
		{
			_error = true;
			HandleError(_fd, 400, "Bad Request");
			break;
		}
		else if (zero)
		{
			_Connection == H_DISCONNECTING;
			if (readnum == 0)
			{
				break;
			}
		}

		if (STATEMENT == PARSEREQUESTLINE)
		{
			LINE_STATUS flag = this->handlehead();
			if (flag == LINE_OPEN)
			{
				break;
			}
			else if (flag == LINE_BAD)
			{
				inbuffer.clear();
				_error = true;
				HandleError(_fd, 400, "Bad Request");
				// handleError();
				break;
			}
			else
			{
				STATEMENT = PARSEHEAD;
			}
		}
		if (STATEMENT == PARSEHEAD)
		{
			HEADSTATE flag = this->ParseHeaders();
			if (flag == PARSE_HEADER_AGAIN)
			{
				break;
			}
			else if (flag == PARSE_HEADER_ERROR)
			{
				_error = true;
				HandleError(_fd, 400, "Bad Request");
				//  HandleError()
				break;
			}
		}
		if (_method == GET || _method == HEAD)
		{
			STATEMENT = PARSEREQUEST;
		}
		else if (_method == POST)
		{
			STATEMENT = PARSEBODY;
		}
		if (STATEMENT == PARSEBODY)
		{
			//解析请求实体/post方法
		}

		if (STATEMENT == PARSEREQUEST)
		{
			ANALYSISSTATE flag = this->AnalysisRequest();
			if (flag == ANALYSISSUCESS)
			{
				STATEMENT = STATE_FINISH;
				break;
			}
			else
			{
				_error = true;
				break;
			}
		}
	} while (false);
	if (!_error)
	{
		if (outbuffer.size() > 0)
		{
			cout << "-------" << endl;
			cout << outbuffer << endl;
			cout << "-------" << endl;
			DealWithWrite();
		}
		if (!_error && STATEMENT == STATE_FINISH)
		{
			this->reset();
			if (inbuffer.size() > 0)
			{
				if (_Connection != H_DISCONNECTING)
					DealWithRead();
			}
		}
		else if (!_error && _Connection != H_DISCONNECTED)
		{
			events |= EPOLLIN;
		}
		cout << "EVETNS&EPOLLIN:" << (events & EPOLLIN) << endl;
	}

	// char buff[1024];

	// std::cout << "hello"; // int ret = read(_fd, _buff, 1024);
	// readn(_fd, buffer);

	// if (ret > 0)
	// {
	// 	std::cout << "recv something" << std::endl;
	// 	std::cout << _buff << std::endl;
	// 	// close(_fd);
	// }
	// if (write(_fd, _buff, 1024) != 0)
	// {
	// 	unsigned int &events = _channel->GetEvents();
	// 	events = EPOLLIN | EPOLLET;
	// 	_loop->UpdatetoPoll(_channel, 0);
	// 	return;
	// }
	// _channel->SetEvents(EPOLLOUT | EPOLLET);
	// std::cout << "_channel:" << _channel << std::endl;
	// std::cout << "events:" << events << std::endl;
	// std::cout << "getrevents:" << _channel->GetREvents() << std::endl;
	// events = EPOLLOUT | EPOLLET;
	// std::cout << "EPOLLIN|EPOLLOUT" << (EPOLLOUT | EPOLLET) << std::endl;
	// _loop->UpdatetoPoll(_channel, 0);
	// std::cout << "hello";
	// return;
}

void HttpData::DealWithConn()
{
	// // _channel->SetEvents(DEFAULT_EVENT);
	// const char *_buff = buffer.c_str();
	// int ret = write(_fd, _buff, buffer.size());
	// assert(ret > 0);
	// unsigned int &events = _channel->GetEvents();
	// events = EPOLLIN | EPOLLET;
	// _loop->UpdatetoPoll(_channel, 0);
	// return;
	SeperatrTimer();
	unsigned int &events = _channel->GetEvents();
	if (!_error && _Connection == H_CONNECTED)
	{
		if (events != 0)
		{
			int timeout = DEFAULT_EXPIRED_TIME;
			if (_iskeepalive)
				timeout = DEFAULT_KEEP_ALIVE_TIME;
			if ((events & EPOLLIN) && (events & EPOLLOUT))
			{
				events = __uint32_t(0);
				events |= EPOLLOUT;
			}
			events |= EPOLLET;
			_loop->UpdatetoPoll(_channel, timeout);
		}
		else if (_iskeepalive)
		{
			events |= (EPOLLIN | EPOLLET);
			int timeout = (DEFAULT_KEEP_ALIVE_TIME);
			_loop->UpdatetoPoll(_channel, timeout);
		}
		else
		{
			// _loop->ShutDown(_channel);
			// _loop->RunInLoop(std::bind(&HttpData::CloseConn, shared_from_this()));

			events |= (EPOLLIN | EPOLLET);
			int timeout = (DEFAULT_KEEP_ALIVE_TIME >> 1);
			_loop->UpdatetoPoll(_channel, timeout);
		}
	}
	else if (!_error && _Connection == H_DISCONNECTING && (events & EPOLLOUT))
	{
		events = (EPOLLOUT | EPOLLET);
	}
	else
	{
		cout << "dealwitherror" << endl;
		_loop->RunInLoop(std::bind(&HttpData::CloseConn, shared_from_this()));
	}
	cout << "EVETNS&EPOLLIN:" << (events & EPOLLIN) << endl;
}

void HttpData::DealWithWrite()
{
	if (!_error && _Connection != H_DISCONNECTED)
	{
		unsigned int &events = _channel->GetEvents();
		if (writen(_fd, outbuffer) < 0)
		{
			perror("writen");
			events = 0;
			_error = true;
		}
		if (outbuffer.size() > 0)
			events |= EPOLLOUT;
	}
}

void HttpData::CloseConn()
{
	_Connection = H_DISCONNECTED;
	shared_ptr<HttpData> guard(shared_from_this());
	_loop->RemovePoll(_channel);
	// _loop
}

void HttpData::NewConn()
{
	// _channel-
	_channel->SetEvents(DEFAULT_EVENT);
	_loop->AddtoPoll(_channel, DEFAULT_EXPIRED_TIME);
}

void HttpData::SeperatrTimer()
{
	if (_timer.lock())
	{
		auto timer = _timer.lock();
		timer->ClearReq();
		timer.reset();
	}
}

void HttpData::HandleError(int fd, int errnum, std::string shortmsg)
{
	shortmsg = " " + shortmsg;
	char send_buff[4096];
	string body_buff, header_buff;
	body_buff += "<html><title>哎~出错了</title>";
	body_buff += "<body bgcolor=\"ffffff\">";
	body_buff += to_string(errnum) + shortmsg;
	body_buff += "<hr><em> LinYa's Web Server</em>\n</body></html>";

	header_buff += "HTTP/1.1 " + to_string(errnum) + shortmsg + "\r\n";
	header_buff += "Content-Type: text/html\r\n";
	header_buff += "Connection: Close\r\n";
	header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
	header_buff += "Server: LinYa's Web Server\r\n";
	;
	header_buff += "\r\n";
	// 错误处理不考虑writen不完的情况
	sprintf(send_buff, "%s", header_buff.c_str());
	writen(fd, send_buff, strlen(send_buff));
	sprintf(send_buff, "%s", body_buff.c_str());
	writen(fd, send_buff, strlen(send_buff));
}