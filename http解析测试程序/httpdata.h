/*

	2022-5-4
	一个http协议解析的实现
	主从状态机

*/
#include <unistd.h>
#include <sys/fcntl.h>
#include <iostream>
#include <string.h>
#include <unordered_map>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/mman.h>
using namespace std;
class MimeType
{
private:
	static void init();
	static std::unordered_map<std::string, std::string> mime;
	MimeType();
	MimeType(const MimeType &m);

public:
	static std::string getMime(const std::string &suffix);

private:
	static pthread_once_t once_control;
};
pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;

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

void MimeType::init()
{
	mime[".html"] = "text/html";
	mime[".avi"] = "video/x-msvideo";
	mime[".bmp"] = "image/bmp";
	mime[".c"] = "text/plain";
	mime[".doc"] = "application/msword";
	mime[".gif"] = "image/gif";
	mime[".gz"] = "application/x-gzip";
	mime[".htm"] = "text/html";
	mime[".ico"] = "image/x-icon";
	mime[".jpg"] = "image/jpeg";
	mime[".png"] = "image/png";
	mime[".txt"] = "text/plain";
	mime[".mp3"] = "audio/mp3";
	mime["default"] = "text/html";
	mime[".mp4"] = "video/mp4";
}

std::string MimeType::getMime(const std::string &suffix)
{
	pthread_once(&once_control, MimeType::init);
	if (mime.find(suffix) == mime.end())
		return mime["default"];
	else
		return mime[suffix];
}

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
class HttpData
{
public:
	HttpData()
		: STATEMENT(PARSEREQUESTLINE)
	{
	}
	// enum ANA{ANS_BAD_0,ANS_OK=1};

public:
	string inbuffer;
	string outbuffer;
	string method;
	string url;
	string version;
	string _Host;
	METHOD _method;
	bool _iskeepalive;
	ConnectionState _Connection;
	// STATUS statement = REQUESTLINE;
	PROCESSSTATE STATEMENT;
	// string
	unordered_map<string, string> _headvalue;
	//每次只传入一行string
	LINE_STATUS handlehead()
	{
		// size_t pos;
		if (STATEMENT == PARSEREQUESTLINE)
		{
			// string temp = inbuffer
			string &str = inbuffer;
			// string cop = str;

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
			method = requestline.substr(0, pos);
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
			size_t pos1 = url.find('?');
			if (pos1 != string::npos)
			{
				url = url.substr(0, pos1);
			}

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

	enum H_STATE
	{
		state1 = 0,
		state2,
		state3
	};
	H_STATE H_STATE;
	HEADSTATE ParseHeaders()
	{
		string &str = inbuffer;
		// size_t key_start = -1, key_end = -1, value_start = -1, value_end = -1;
		// int now_read_line_begin = 0;
		// bool notfinish = true;
		// size_t i = 0;
		bool headana = true;
		while (headana)
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
				// break;
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
			if (pos < 0)
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
	ANALYSISSTATE AnalysisRequest()
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
				header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" + to_string(2000) + "\r\n";
			}
			int dot_pos = url.find('.');
			string filetype;
			if (dot_pos == string::npos)
			{
				filetype = MimeType::getMime("default");
			}
			else
			{
				filetype = MimeType::getMime(url.substr(dot_pos));
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
				// HandleError();
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
				return ANALYSISERROR;
			}

			void *mmapret = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
			close(src_fd);
			if (mmapret == (void *)-1)
			{
				munmap(mmapret, sbuf.st_size);
				outbuffer.clear();
				// handleError;
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
	void RequestLineAna()
	{
		// if (_Connection =)
		do
		{
			if (STATEMENT == PARSEREQUESTLINE)
			{
				LINE_STATUS flag = this->handlehead();
				if (flag == LINE_BAD)
				{
					// HandleError();
					break;
				}
				else if (flag == LINE_OPEN)
				{
					break;
				}
				else
				{
					STATEMENT = PARSEHEAD;
				}
			}
			if (STATEMENT == PARSEHEAD)
			{
				HEADSTATE flag = ParseHeaders();
				if (flag == PARSE_HEADER_AGAIN)
				{
					break;
				}
				else if (flag == PARSE_HEADER_ERROR)
				{
				}
				if (_method == GET)
				{
					STATEMENT = PARSEREQUEST;
				}
				else if (_method == HEAD)
				{
					STATEMENT = PARSEREQUEST;
				}
				else if (_method == POST)
				{
					STATEMENT = PARSEBODY;
				}
			}
			if (STATEMENT == PARSEBODY)
			{
			}
			if (STATEMENT == PARSEREQUEST)
			{
				ANALYSISSTATE flag = this->AnalysisRequest();
			}
		} while (false);
	}
};
// void handleError(int fd, int err_num, string short_msg)
// {
// 	short_msg = " " + short_msg;
// 	char send_buff[4096];
// 	string body_buff, header_buff;
// 	body_buff += "<html><title>哎~出错了</title>";
// 	body_buff += "<body bgcolor=\"ffffff\">";
// 	body_buff += to_string(err_num) + short_msg;
// 	body_buff += "<hr><em> LinYa's Web Server</em>\n</body></html>";

// 	header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
// 	header_buff += "Content-Type: text/html\r\n";
// 	header_buff += "Connection: Close\r\n";
// 	header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
// 	header_buff += "Server: LinYa's Web Server\r\n";
// 	;
// 	header_buff += "\r\n";
// 	// 错误处理不考虑writen不完的情况
// 	sprintf(send_buff, "%s", header_buff.c_str());
// 	writen(fd, send_buff, strlen(send_buff));
// 	sprintf(send_buff, "%s", body_buff.c_str());
// 	writen(fd, send_buff, strlen(send_buff));
// }